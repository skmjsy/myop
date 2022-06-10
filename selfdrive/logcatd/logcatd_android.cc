#include <sys/time.h>
#include <sys/resource.h>

#include <android/log.h>
#include <log/logger.h>
#include <log/logprint.h>

#include "cereal/messaging/messaging.h"
#include "selfdrive/common/util.h"


// atom
typedef struct LiveNaviDataResult {
      int speedLimit;  // int;
      float speedLimitDistance;  // Float32;
      int safetySign;    // int;
      float roadCurvature;    // Float32;
      int turnInfo;    // int;
      float distanceToTurn;    // Float32;
      //bool  mapValid;    // bool;
      //int  mapEnable;    // bool;
      long  tv_sec;
      long  tv_nsec;
} LiveNaviDataResult;


int main() {
  setpriority(PRIO_PROCESS, 0, -15);
  long  nDelta_nsec = 0;
  long  tv_nsec;
  float tv_nsec2;
  bool  sBump = false;

  ExitHandler do_exit;
  PubMaster pm({"liveNaviData"});
  LiveNaviDataResult res;

  log_time last_log_time = {};
  logger_list *logger_list = android_logger_list_alloc(ANDROID_LOG_RDONLY | ANDROID_LOG_NONBLOCK, 0, 0);

  while (!do_exit) {
    // setup android logging
    if (!logger_list) {
      logger_list = android_logger_list_alloc_time(ANDROID_LOG_RDONLY | ANDROID_LOG_NONBLOCK, last_log_time, 0);
    }
    assert(logger_list);

    struct logger *main_logger = android_logger_open(logger_list, LOG_ID_MAIN);
    assert(main_logger);
   // struct logger *radio_logger = android_logger_open(logger_list, LOG_ID_RADIO);
   // assert(radio_logger);
   // struct logger *system_logger = android_logger_open(logger_list, LOG_ID_SYSTEM);
   // assert(system_logger);
   // struct logger *crash_logger = android_logger_open(logger_list, LOG_ID_CRASH);
   // assert(crash_logger);
   // struct logger *kernel_logger = android_logger_open(logger_list, (log_id_t)5); // LOG_ID_KERNEL
   // assert(kernel_logger);

    while (!do_exit) {
      log_msg log_msg;
      int err = android_logger_list_read(logger_list, &log_msg);
      if (err <= 0) break;

      AndroidLogEntry entry;
      err = android_log_processLogBuffer(&log_msg.entry_v1, &entry);
      if (err < 0) continue;
      last_log_time.tv_sec = entry.tv_sec;
      last_log_time.tv_nsec = entry.tv_nsec;

      tv_nsec2 = entry.tv_nsec / 1000000;
      tv_nsec =  entry.tv_sec * 1000ULL + long(tv_nsec2); // per 1000 = 1s

      MessageBuilder msg;
      auto framed = msg.initEvent().initLiveNaviData();

   //  opkrspdlimit, opkrspddist, opkrsigntype, opkrcurvangle

      // code based from atom
      nDelta_nsec = tv_nsec - res.tv_nsec;
      //nDelta = entry.tv_sec - res.tv_sec;

      if( strcmp( entry.tag, "opkrspddist" ) == 0 )
      {
        res.tv_sec = entry.tv_sec;
        res.tv_nsec = tv_nsec;
        res.speedLimitDistance = atoi( entry.message );
      }
      else if( strcmp( entry.tag, "opkrspdlimit" ) == 0 )
      {
        res.speedLimit = atoi( entry.message );
      }
      else if( strcmp( entry.tag, "opkrsigntype" ) == 0 )
      {
        res.tv_sec = entry.tv_sec;
        res.tv_nsec = tv_nsec;
        res.safetySign = atoi( entry.message );
        if (res.safetySign == 124) {
          sBump = true;
        }
      }
      else if( (res.speedLimitDistance > 1 && res.speedLimitDistance < 60) && (strcmp( entry.tag, "AudioFlinger" ) == 0) )  //   msm8974_platform
      {
        res.speedLimitDistance = 0;
        res.speedLimit = 0;
        res.safetySign = 0;
        //system("logcat -c &");
      }
      else if( strcmp( entry.tag, "opkrturninfo" ) == 0 )
      {
        res.turnInfo = atoi( entry.message );
      }
      else if( strcmp( entry.tag, "opkrdistancetoturn" ) == 0 )
      {
        res.distanceToTurn = atoi( entry.message );
      }
      else if( nDelta_nsec > 5000 )
      {
        if (res.safetySign == 197 && res.speedLimitDistance < 100) {
          res.speedLimitDistance = 0;
          res.speedLimit = 0;
          res.safetySign = 0;
        }
        else if ( res.safetySign == 124 && (!sBump) )
        {
          res.safetySign = 0;
        }
        else if (res.safetySign != 0 && res.safetySign != 124 && res.speedLimitDistance < 50 && res.speedLimitDistance > 0)
        {
          res.speedLimitDistance = 0;
          res.speedLimit = 0;
          res.safetySign = 0;
        }
        else if( nDelta_nsec > 10000 )
        {
          res.tv_sec = entry.tv_sec;
          res.tv_nsec = tv_nsec;
          res.speedLimitDistance = 0;
          res.speedLimit = 0;
          res.safetySign = 0;
        }
      }

      framed.setSpeedLimit( res.speedLimit );  // int;
      framed.setSpeedLimitDistance( res.speedLimitDistance );  // raw_target_speed_map_dist Float32;
      framed.setSafetySign( res.safetySign ); // int;
      // framed.setRoadCurvature( res.roadCurvature ); // road_curvature Float32;
      framed.setTurnInfo( res.turnInfo );  // int;
      framed.setDistanceToTurn( res.distanceToTurn );  // Float32;
      framed.setTs( res.tv_sec );
      //framed.setMapEnable( res.mapEnable );
      //framed.setMapValid( res.mapValid );

    /*
    signtype
    111 오른쪽 급커브
    112 왼쪽 급커브
    113 굽은도로
    118, 127 어린이보호구역
    122 좁아지는 도로
    124 과속방지턱
    129 주정차
    131 단속카메라(신호위반카메라)  
    135 고정식(버스단속구간)  - 호야
    150 경찰차(이동식단속구간)  - 호야
    165 구간단속    
    198 차선변경금지시작
    199 차선변경금지종료
    129 주정차금지구간
    123 철길건널목
    200 단속구간(고정형 이동식)
    231 단속(카메라, 신호위반)
    246 버스전용차로단속
    247 과적단속
    248 교통정보수집
    249 추월금지구간
    250 갓길단속
    251 적재불량단속
   */  


      pm.send("liveNaviData", msg);
    }

    android_logger_list_free(logger_list);
    logger_list = NULL;
    util::sleep_for(500);
  }

  if (logger_list) {
    android_logger_list_free(logger_list);
  }

  return 0;
}
