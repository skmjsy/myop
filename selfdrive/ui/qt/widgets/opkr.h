#pragma once

#include <QPushButton>
#include <QSoundEffect>

#include "selfdrive/hardware/hw.h"
#include "selfdrive/ui/qt/widgets/controls.h"
#include "selfdrive/ui/ui.h"
#include <QComboBox>
#include <QAbstractItemView>

class SwitchOpenpilot : public ButtonControl {
  Q_OBJECT

public:
  SwitchOpenpilot();

private:
  Params params;

  QString githubid;
  QString githubrepo;
  QString githubbranch;

  void refresh();
  void getUserID(const QString &userid);
  void getRepoID(const QString &repoid);
  void getBranchID(const QString &branchid);
};

class SshLegacyToggle : public ToggleControl {
  Q_OBJECT

public:
  SshLegacyToggle() : ToggleControl("Use Legacy SSH Key", "Public KEY (0.8.2 or less) is used when accessing SSH.", "", Params().getBool("OpkrSSHLegacy")) {
    QObject::connect(this, &SshLegacyToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("OpkrSSHLegacy", status);
    });
  }
};

class AutoResumeToggle : public ToggleControl {
  Q_OBJECT

public:
  AutoResumeToggle() : ToggleControl("Use Auto Resume at Stop", "It uses the automatic departure function when stopping while using SCC.", "../assets/offroad/icon_shell.png", Params().getBool("OpkrAutoResume")) {
    QObject::connect(this, &AutoResumeToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("OpkrAutoResume", status);
    });
  }
};

class VariableCruiseToggle : public ToggleControl {
  Q_OBJECT

public:
  VariableCruiseToggle() : ToggleControl("Use Cruise Button Spamming", "Use the cruise button while using SCC to assist in acceleration and deceleration.", "../assets/offroad/icon_shell.png", Params().getBool("OpkrVariableCruise")) {
    QObject::connect(this, &VariableCruiseToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("OpkrVariableCruise", status);
    });
  }
};

class CruiseGapAdjustToggle : public ToggleControl {
  Q_OBJECT

public:
  CruiseGapAdjustToggle() : ToggleControl("Change Cruise Gap at Stop", "For a quick start when stopping, the cruise gap will be changed to 1 step, and after departure, it will return to the original cruise gap according to certain conditions.", "../assets/offroad/icon_shell.png", Params().getBool("CruiseGapAdjust")) {
    QObject::connect(this, &CruiseGapAdjustToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("CruiseGapAdjust", status);
    });
  }
};

class AutoEnabledToggle : public ToggleControl {
  Q_OBJECT

public:
  AutoEnabledToggle() : ToggleControl("Use Auto Engagement", "If the cruise button status is standby (CRUISE indication only and speed is not specified) in the Disengagement state, activate the automatic Engagement.", "../assets/offroad/icon_shell.png", Params().getBool("AutoEnable")) {
    QObject::connect(this, &AutoEnabledToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("AutoEnable", status);
    });
  }
};

class CruiseAutoResToggle : public ToggleControl {
  Q_OBJECT

public:
  CruiseAutoResToggle() : ToggleControl("Use Auto RES while Driving", "If the brake is applied while using the SCC and the standby mode is changed (CANCEL is not applicable), set it back to the previous speed when the brake pedal is released/accelerated pedal is operated. It operates when the cruise speed is set and the vehicle speed is more than 30 km/h or the car in front is recognized.", "../assets/offroad/icon_shell.png", Params().getBool("CruiseAutoRes")) {
    QObject::connect(this, &CruiseAutoResToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("CruiseAutoRes", status);
    });
  }
};

class BatteryChargingControlToggle : public ToggleControl {
  Q_OBJECT

public:
  BatteryChargingControlToggle() : ToggleControl("Enable Battery Charging Control", "It uses the battery charge control function.", "../assets/offroad/icon_shell.png", Params().getBool("OpkrBatteryChargingControl")) {
    QObject::connect(this, &BatteryChargingControlToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("OpkrBatteryChargingControl", status);
    });
  }
};

class BlindSpotDetectToggle : public ToggleControl {
  Q_OBJECT

public:
  BlindSpotDetectToggle() : ToggleControl("Show BSM Status", "If a car is detected in the rear, it will be displayed on the screen.", "../assets/offroad/icon_shell.png", Params().getBool("OpkrBlindSpotDetect")) {
    QObject::connect(this, &BlindSpotDetectToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("OpkrBlindSpotDetect", status);
      if (state) {
        QUIState::ui_state.scene.nOpkrBlindSpotDetect = true;
      } else {
        QUIState::ui_state.scene.nOpkrBlindSpotDetect = false;
      }
    });
  }
};

class MadModeEnabledToggle : public ToggleControl {
  Q_OBJECT

public:
  MadModeEnabledToggle() : ToggleControl("MainSwitch Openpilot ON/OFF", "Activate the open pilot using the cruise Main Switch.", "../assets/offroad/icon_shell.png", Params().getBool("MadModeEnabled")) {
    QObject::connect(this, &MadModeEnabledToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("MadModeEnabled", status);
    });
  }
};

class WhitePandaSupportToggle : public ToggleControl {
  Q_OBJECT

public:
  WhitePandaSupportToggle() : ToggleControl("Support WhitePanda", "Turn on this function if you use WhitePanda.", "../assets/offroad/icon_shell.png", Params().getBool("WhitePandaSupport")) {
    QObject::connect(this, &WhitePandaSupportToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("WhitePandaSupport", status);
    });
  }
};

class SteerWarningFixToggle : public ToggleControl {
  Q_OBJECT

public:
  SteerWarningFixToggle() : ToggleControl("Ignore of Steering Warning", "Turn on the function when a steering error occurs in the vehicle and the open pilot cannot be executed (some vehicles only). Do not turn on the function if it occurs in a normal error environment while driving.", "../assets/offroad/icon_shell.png", Params().getBool("SteerWarningFix")) {
    QObject::connect(this, &SteerWarningFixToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("SteerWarningFix", status);
    });
  }
};

class SteerWindDownToggle : public ToggleControl {
  Q_OBJECT

public:
  SteerWindDownToggle() : ToggleControl("Steer Wind Down", "Slowly lower the torq when Steer Warning is performed. In some vehicles, steering angle limit lamps may appear, so turn off the function to use the maximum steering angle regardless of error.", "../assets/offroad/icon_shell.png", Params().getBool("SteerWindDown")) {
    QObject::connect(this, &SteerWindDownToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("SteerWindDown", status);
    });
  }
};

class LiveSteerRatioToggle : public ToggleControl {
  Q_OBJECT

public:
  LiveSteerRatioToggle() : ToggleControl("Use Live SteerRatio", "Live SteerRatio is used instead of variable/fixed SteerRatio.", "../assets/offroad/icon_shell.png", Params().getBool("OpkrLiveSteerRatio")) {
    QObject::connect(this, &LiveSteerRatioToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("OpkrLiveSteerRatio", status);
    });
  }
};

class VariableSteerMaxToggle : public ToggleControl {
  Q_OBJECT

public:
  VariableSteerMaxToggle() : ToggleControl("Use variable SteerMax", "Use the variable SteerMax by curvature.", "../assets/offroad/icon_shell.png", Params().getBool("OpkrVariableSteerMax")) {
    QObject::connect(this, &VariableSteerMaxToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("OpkrVariableSteerMax", status);
    });
  }
};

class VariableSteerDeltaToggle : public ToggleControl {
  Q_OBJECT

public:
  VariableSteerDeltaToggle() : ToggleControl("Use variable SteerDelta", "Use variable SteerDelta by curvature. It changes from default to 5(DeltaUp) and 10(DeltaDown).", "../assets/offroad/icon_shell.png", Params().getBool("OpkrVariableSteerDelta")) {
    QObject::connect(this, &VariableSteerDeltaToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("OpkrVariableSteerDelta", status);
    });
  }
};

class DrivingRecordToggle : public ToggleControl {
  Q_OBJECT

public:
  DrivingRecordToggle() : ToggleControl("Use Auto Screen Record", "Automatically record/stop the screen while driving. Recording begins after departure, and recording ends when the vehicle stops.", "../assets/offroad/icon_shell.png", Params().getBool("OpkrDrivingRecord")) {
    QObject::connect(this, &DrivingRecordToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("OpkrDrivingRecord", status);
      if (state) {
        QUIState::ui_state.scene.driving_record = true;
      } else {
        QUIState::ui_state.scene.driving_record = false;
      }
    });
  }
};

class TurnSteeringDisableToggle : public ToggleControl {
  Q_OBJECT

public:
  TurnSteeringDisableToggle() : ToggleControl("Stop Steer Assist on Turn Signals", "When driving below the lane change speed, the automatic steering is temporarily paused while the turn signals on.", "../assets/offroad/icon_shell.png", Params().getBool("OpkrTurnSteeringDisable")) {
    QObject::connect(this, &TurnSteeringDisableToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("OpkrTurnSteeringDisable", status);
    });
  }
};

class HotspotOnBootToggle : public ToggleControl {
  Q_OBJECT

public:
  HotspotOnBootToggle() : ToggleControl("HotSpot on Boot", "It automatically runs a hotspot when booting.", "", Params().getBool("OpkrHotspotOnBoot")) {
    QObject::connect(this, &HotspotOnBootToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("OpkrHotspotOnBoot", status);
    });
  }
};

class CruiseOverMaxSpeedToggle : public ToggleControl {
  Q_OBJECT

public:
  CruiseOverMaxSpeedToggle() : ToggleControl("Reset MaxSpeed Over CurrentSpeed", "If the current speed exceeds the set speed, synchronize the set speed with the current speed.", "../assets/offroad/icon_shell.png", Params().getBool("CruiseOverMaxSpeed")) {
    QObject::connect(this, &CruiseOverMaxSpeedToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("CruiseOverMaxSpeed", status);
    });
  }
};

class DebugUiOneToggle : public ToggleControl {
  Q_OBJECT

public:
  DebugUiOneToggle() : ToggleControl("DEBUG UI 1", "", "../assets/offroad/icon_shell.png", Params().getBool("DebugUi1")) {
    QObject::connect(this, &DebugUiOneToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("DebugUi1", status);
      if (state) {
        QUIState::ui_state.scene.nDebugUi1 = true;
      } else {
        QUIState::ui_state.scene.nDebugUi1 = false;
      }
    });
  }
};

class DebugUiTwoToggle : public ToggleControl {
  Q_OBJECT

public:
  DebugUiTwoToggle() : ToggleControl("DEBUG UI 2", "", "../assets/offroad/icon_shell.png", Params().getBool("DebugUi2")) {
    QObject::connect(this, &DebugUiTwoToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("DebugUi2", status);
      if (state) {
        QUIState::ui_state.scene.nDebugUi2 = true;
      } else {
        QUIState::ui_state.scene.nDebugUi2 = false;
      }
    });
  }
};

class LongLogToggle : public ToggleControl {
  Q_OBJECT

public:
  LongLogToggle() : ToggleControl("Show LongControl LOG", "Display logs for long tuning debugs instead of variable cruise logs on the screen.", "../assets/offroad/icon_shell.png", Params().getBool("LongLogDisplay")) {
    QObject::connect(this, &LongLogToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("LongLogDisplay", status);
    });
  }
};

class PrebuiltToggle : public ToggleControl {
  Q_OBJECT

public:
  PrebuiltToggle() : ToggleControl("Use Smart Prebuilt", "Create a Prebuilt file and speed up booting. When this function is turned on, the booting speed is accelerated using the cache, and if you press the update button in the menu after modifying the code, or if you rebooted with the 'gi' command in the command window, remove it automatically and compile it.", "../assets/offroad/icon_shell.png", Params().getBool("PutPrebuiltOn")) {
    QObject::connect(this, &PrebuiltToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("PutPrebuiltOn", status);
    });
  }
};

class LDWSToggle : public ToggleControl {
  Q_OBJECT

public:
  LDWSToggle() : ToggleControl("Set LDWS Vehicles", "", "../assets/offroad/icon_shell.png", Params().getBool("LdwsCarFix")) {
    QObject::connect(this, &LDWSToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("LdwsCarFix", status);
    });
  }
};

class FPTwoToggle : public ToggleControl {
  Q_OBJECT

public:
  FPTwoToggle() : ToggleControl("Use FingerPrint 2.0", "Activate Fingerprint 2.0. Activate the vehicle with ECU recognition.", "../assets/offroad/icon_shell.png", Params().getBool("FingerprintTwoSet")) {
    QObject::connect(this, &FPTwoToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("FingerprintTwoSet", status);
    });
  }
};

class GearDToggle : public ToggleControl {
  Q_OBJECT

public:
  GearDToggle() : ToggleControl("Set DriverGear by Force", "It is used when the gear recognition problem. Basically, CABANA data should be analyzed, but it is temporarily resolved.", "../assets/offroad/icon_shell.png", Params().getBool("JustDoGearD")) {
    QObject::connect(this, &GearDToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("JustDoGearD", status);
    });
  }
};

class ComIssueToggle : public ToggleControl {
  Q_OBJECT

public:
  ComIssueToggle() : ToggleControl("Turn Off Communication Issue Alarm", "Turn this option on to disable the communication issue alarm.", "../assets/offroad/icon_shell.png", Params().getBool("ComIssueGone")) {
    QObject::connect(this, &ComIssueToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("ComIssueGone", status);
    });
  }
};

class RunNaviOnBootToggle : public ToggleControl {
  Q_OBJECT

public:
  RunNaviOnBootToggle() : ToggleControl("Run Navigation on Boot", "Automatically execute the navigation (waze) when switching to the driving screen after booting.", "../assets/offroad/icon_shell.png", Params().getBool("OpkrRunNaviOnBoot")) {
    QObject::connect(this, &RunNaviOnBootToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("OpkrRunNaviOnBoot", status);
    });
  }
};

class BattLessToggle : public ToggleControl {
  Q_OBJECT

public:
  BattLessToggle() : ToggleControl("Set BatteryLess Eon", "This is a toggle for batteryless EON. Related settings apply.", "../assets/offroad/icon_shell.png", Params().getBool("OpkrBattLess")) {
    QObject::connect(this, &BattLessToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("OpkrBattLess", status);
    });
  }
};

class LiveTunePanelToggle : public ToggleControl {
  Q_OBJECT

public:
  LiveTunePanelToggle() : ToggleControl("Use LiveTune and Show UI", "Display the UI related to live tuning on the screen. Various tuning values can be adjusted live on the driving screen. It is reflected in the parameter when adjusting, and the value is maintained even after turning off the toggle and rebooting.", "../assets/offroad/icon_shell.png", Params().getBool("OpkrLiveTunePanelEnable")) {
    QObject::connect(this, &LiveTunePanelToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("OpkrLiveTunePanelEnable", status);
      if (state) {
        QUIState::ui_state.scene.live_tune_panel_enable = true;
        QUIState::ui_state.scene.opkr_livetune_ui = true;
      } else {
        QUIState::ui_state.scene.live_tune_panel_enable = false;
        QUIState::ui_state.scene.opkr_livetune_ui = false;
      }
    });
  }
};

class KRDateToggle : public ToggleControl {
  Q_OBJECT

public:
  KRDateToggle() : ToggleControl("Display Date on Screen", "Display the current date on the driving screen.", "../assets/offroad/icon_shell.png", Params().getBool("KRDateShow")) {
    QObject::connect(this, &KRDateToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("KRDateShow", status);
      if (state) {
        QUIState::ui_state.scene.kr_date_show = true;
      } else {
        QUIState::ui_state.scene.kr_date_show = false;
      }
    });
  }
};

class KRTimeToggle : public ToggleControl {
  Q_OBJECT

public:
  KRTimeToggle() : ToggleControl("Display Time on Screen", "Display the current time on the driving screen.", "../assets/offroad/icon_shell.png", Params().getBool("KRTimeShow")) {
    QObject::connect(this, &KRTimeToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("KRTimeShow", status);
      if (state) {
        QUIState::ui_state.scene.kr_time_show = true;
      } else {
        QUIState::ui_state.scene.kr_time_show = false;
      }
    });
  }
};

class GitPullOnBootToggle : public ToggleControl {
  Q_OBJECT

public:
  GitPullOnBootToggle() : ToggleControl("Git Pull On Boot", "If there is an update after the boot, run Git Pull automatically and reboot.", "", Params().getBool("GitPullOnBoot")) {
    QObject::connect(this, &GitPullOnBootToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("GitPullOnBoot", status);
    });
  }
};

class StoppingDistAdjToggle : public ToggleControl {
  Q_OBJECT

public:
  StoppingDistAdjToggle() : ToggleControl("Adjust Stopping Distance", "Stop a little further ahead than the radar stop distance. If you approach the car in front of you at a high speed, it may sometimes be difficult to stop enough, so if you are uncomfortable, turn off the function.", "../assets/offroad/icon_shell.png", Params().getBool("StoppingDistAdj")) {
    QObject::connect(this, &StoppingDistAdjToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("StoppingDistAdj", status);
    });
  }
};

class ShowErrorToggle : public ToggleControl {
  Q_OBJECT

public:
  ShowErrorToggle() : ToggleControl("Show TMUX Error", "Display the error on the EON screen when a process error occurs while driving or off-road.", "../assets/offroad/icon_shell.png", Params().getBool("ShowError")) {
    QObject::connect(this, &ShowErrorToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("ShowError", status);
      if (state) {
        QUIState::ui_state.scene.show_error = true;
      } else {
        QUIState::ui_state.scene.show_error = false;
      }
    });
  }
};

class StockNaviSpeedToggle : public ToggleControl {
  Q_OBJECT

public:
  StockNaviSpeedToggle() : ToggleControl("Use Stock SafetyCAM Speed", "When decelerating the safety section, use the safety speed from the vehicle navigation system (limited to some vehicles with the corresponding data).", "../assets/offroad/icon_shell.png", Params().getBool("StockNaviSpeedEnabled")) {
    QObject::connect(this, &StockNaviSpeedToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("StockNaviSpeedEnabled", status);
    });
  }
};

class E2ELongToggle : public ToggleControl {
  Q_OBJECT

public:
  E2ELongToggle() : ToggleControl("Enable E2E Long", "Activate E2E Long. It may work unexpectedly. Be careful.", "../assets/offroad/icon_shell.png", Params().getBool("E2ELong")) {
    QObject::connect(this, &E2ELongToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("E2ELong", status);
    });
  }
};

class GoogleMapEnabledToggle : public ToggleControl {
  Q_OBJECT

public:
  GoogleMapEnabledToggle() : ToggleControl("Use GoogleMap for Mapbox", "Use GoogleMap when you search a destination.", "../assets/offroad/icon_shell.png", Params().getBool("GoogleMapEnabled")) {
    QObject::connect(this, &GoogleMapEnabledToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("GoogleMapEnabled", status);
    });
  }
};

class OSMSpeedLimitEnabledToggle : public ToggleControl {
  Q_OBJECT

public:
  OSMSpeedLimitEnabledToggle() : ToggleControl("Enable OSM SpeedLimit", "This enables OSM speedlimit.", "../assets/offroad/icon_shell.png", Params().getBool("OSMSpeedLimitEnable")) {
    QObject::connect(this, &OSMSpeedLimitEnabledToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("OSMSpeedLimitEnable", status);
    });
  }
};

class IgnoreCanErroronISGToggle : public ToggleControl {
  Q_OBJECT

public:
  IgnoreCanErroronISGToggle() : ToggleControl("Ignore Can Error on ISG", "Turn this on, if can error occurs on ISG operation.", "../assets/offroad/icon_shell.png", Params().getBool("IgnoreCANErroronISG")) {
    QObject::connect(this, &IgnoreCanErroronISGToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("IgnoreCANErroronISG", status);
    });
  }
};

class StockLKASEnabledatDisenagedStatusToggle : public ToggleControl {
  Q_OBJECT

public:
  StockLKASEnabledatDisenagedStatusToggle() : ToggleControl("StockLKAS Enabled at Disengagement", "Turn this on, if you want to use Stock LKAS at OP disengaged status. Seems this related to cluster error when OP active because Stock CAN messages over PANDA or not.", "../assets/offroad/icon_shell.png", Params().getBool("StockLKASEnabled")) {
    QObject::connect(this, &StockLKASEnabledatDisenagedStatusToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("StockLKASEnabled", status);
    });
  }
};

class FCA11MessageToggle : public ToggleControl {
  Q_OBJECT

public:
  FCA11MessageToggle() : ToggleControl("Enable FCA11 Message", "Turn this on, if you get an error with forward collision warning.(reboot required)", "../assets/offroad/icon_shell.png", Params().getBool("FCA11Message")) {
    QObject::connect(this, &FCA11MessageToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("FCA11Message", status);
    });
  }
};

class StandstillResumeAltToggle : public ToggleControl {
  Q_OBJECT

public:
  StandstillResumeAltToggle() : ToggleControl("Standstill Resume Alternative", "Turn this on, if auto resume doesn't work at standstill. some cars only(ex. GENESIS). before enable, try to adjust RES message counts above.(reboot required)", "../assets/offroad/icon_shell.png", Params().getBool("StandstillResumeAlt")) {
    QObject::connect(this, &StandstillResumeAltToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("StandstillResumeAlt", status);
    });
  }
};

class MapboxEnabledToggle : public ToggleControl {
  Q_OBJECT

public:
  MapboxEnabledToggle() : ToggleControl("Enable Mapbox", "If you want to use Mapbox, turn on and then connect to device using web browser http://<device ip>:8082  Mapbox setting will show up and type mapbox pk and sk token(you can created this on mapbox.com website). If you want to search destinations with googlemap, first, you should create google api key and enable Enable GoogleMap for Mapbox", "../assets/offroad/icon_shell.png", Params().getBool("MapboxEnabled")) {
    QObject::connect(this, &MapboxEnabledToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("MapboxEnabled", status);
    });
  }
};

// openpilot preview
class OpenpilotView : public AbstractControl {
  Q_OBJECT

public:
  OpenpilotView();

private:
  QPushButton btn;
  Params params;
  
  void refresh();
};

class CarSelectCombo : public AbstractControl 
{
  Q_OBJECT

public:
  CarSelectCombo();

private:
  QPushButton btn;
  QComboBox combobox;
  Params params;

  void refresh();
};

class TimeZoneSelectCombo : public AbstractControl 
{
  Q_OBJECT

public:
  TimeZoneSelectCombo();

private:
  QPushButton btn;
  QComboBox combobox;
  Params params;

  void refresh();
};


// UI
class AutoShutdown : public AbstractControl {
  Q_OBJECT

public:
  AutoShutdown();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class ForceShutdown : public AbstractControl {
  Q_OBJECT

public:
  ForceShutdown();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class AutoScreenOff : public AbstractControl {
  Q_OBJECT

public:
  AutoScreenOff();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class VolumeControl : public AbstractControl {
  Q_OBJECT

public:
  VolumeControl();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  QSoundEffect effect;
  
  void refresh();
  void playsound();
};

class BrightnessControl : public AbstractControl {
  Q_OBJECT

public:
  BrightnessControl();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class BrightnessOffControl : public AbstractControl {
  Q_OBJECT

public:
  BrightnessOffControl();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};


class ChargingMin : public AbstractControl {
  Q_OBJECT

public:
  ChargingMin();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};
class ChargingMax : public AbstractControl {
  Q_OBJECT

public:
  ChargingMax();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};


// Driving
class CruisemodeSelInit : public AbstractControl {
  Q_OBJECT

public:
  CruisemodeSelInit();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class VariableCruiseProfile : public AbstractControl {
  Q_OBJECT

public:
  VariableCruiseProfile();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class LaneChangeSpeed : public AbstractControl {
  Q_OBJECT

public:
  LaneChangeSpeed();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class LaneChangeDelay : public AbstractControl {
  Q_OBJECT

public:
  LaneChangeDelay();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class LeftCurvOffset : public AbstractControl {
  Q_OBJECT

public:
  LeftCurvOffset();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};
class RightCurvOffset : public AbstractControl {
  Q_OBJECT

public:
  RightCurvOffset();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class MaxAngleLimit : public AbstractControl {
  Q_OBJECT

public:
  MaxAngleLimit();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class SpeedLimitOffset : public AbstractControl {
  Q_OBJECT

public:
  SpeedLimitOffset();

private:
  QPushButton btn;
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

// 튜닝 설정
class CameraOffset : public AbstractControl {
  Q_OBJECT

public:
  CameraOffset();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class PathOffset : public AbstractControl {
  Q_OBJECT

public:
  PathOffset();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class SRBaseControl : public AbstractControl {
  Q_OBJECT

public:
  SRBaseControl();

private:
  QPushButton btndigit;
  QPushButton btnminus;
  QPushButton btnplus;
  QPushButton btndirect;
  QLabel label;
  Params params;
  float digit = 0.01;
  
  void refresh();
};
class SRMaxControl : public AbstractControl {
  Q_OBJECT

public:
  SRMaxControl();

private:
  QPushButton btndigit;
  QPushButton btnminus;
  QPushButton btnplus;
  QLabel label;
  Params params;
  float digit = 0.01;
  
  void refresh();
};

class SteerActuatorDelay : public AbstractControl {
  Q_OBJECT

public:
  SteerActuatorDelay();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class SteerRateCost : public AbstractControl {
  Q_OBJECT

public:
  SteerRateCost();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class SteerLimitTimer : public AbstractControl {
  Q_OBJECT

public:
  SteerLimitTimer();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class TireStiffnessFactor : public AbstractControl {
  Q_OBJECT

public:
  TireStiffnessFactor();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class SteerMaxBase : public AbstractControl {
  Q_OBJECT

public:
  SteerMaxBase();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class SteerMaxMax : public AbstractControl {
  Q_OBJECT

public:
  SteerMaxMax();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class SteerMaxv : public AbstractControl {
  Q_OBJECT

public:
  SteerMaxv();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class SteerDeltaUpBase : public AbstractControl {
  Q_OBJECT

public:
  SteerDeltaUpBase();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class SteerDeltaUpMax : public AbstractControl {
  Q_OBJECT

public:
  SteerDeltaUpMax();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class SteerDeltaDownBase : public AbstractControl {
  Q_OBJECT

public:
  SteerDeltaDownBase();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class SteerDeltaDownMax : public AbstractControl {
  Q_OBJECT

public:
  SteerDeltaDownMax();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};


// control
class LateralControl : public AbstractControl {
  Q_OBJECT

public:
  LateralControl();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class PidKp : public AbstractControl {
  Q_OBJECT

public:
  PidKp();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class PidKi : public AbstractControl {
  Q_OBJECT

public:
  PidKi();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class PidKd : public AbstractControl {
  Q_OBJECT

public:
  PidKd();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class PidKf : public AbstractControl {
  Q_OBJECT

public:
  PidKf();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class IgnoreZone : public AbstractControl {
  Q_OBJECT

public:
  IgnoreZone();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class OuterLoopGain : public AbstractControl {
  Q_OBJECT

public:
  OuterLoopGain();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class InnerLoopGain : public AbstractControl {
  Q_OBJECT

public:
  InnerLoopGain();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class TimeConstant : public AbstractControl {
  Q_OBJECT

public:
  TimeConstant();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class ActuatorEffectiveness : public AbstractControl {
  Q_OBJECT

public:
  ActuatorEffectiveness();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class Scale : public AbstractControl {
  Q_OBJECT

public:
  Scale();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class LqrKi : public AbstractControl {
  Q_OBJECT

public:
  LqrKi();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class DcGain : public AbstractControl {
  Q_OBJECT

public:
  DcGain();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class SteerAngleCorrection : public AbstractControl {
  Q_OBJECT

public:
  SteerAngleCorrection();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class MaxSteer : public AbstractControl {
  Q_OBJECT

public:
  MaxSteer();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class MaxRTDelta : public AbstractControl {
  Q_OBJECT

public:
  MaxRTDelta();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class MaxRateUp : public AbstractControl {
  Q_OBJECT

public:
  MaxRateUp();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class MaxRateDown : public AbstractControl {
  Q_OBJECT

public:
  MaxRateDown();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class SteerThreshold : public AbstractControl {
  Q_OBJECT

public:
  SteerThreshold();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class RecordCount : public AbstractControl {
  Q_OBJECT

public:
  RecordCount();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class RecordQuality : public AbstractControl {
  Q_OBJECT

public:
  RecordQuality();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class GitHash : public AbstractControl {
  Q_OBJECT

public:
  GitHash();

private:
  QLabel local_hash;
  QLabel remote_hash;
  Params params;
};

class RESChoice : public AbstractControl {
  Q_OBJECT

public:
  RESChoice();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class MonitoringMode : public AbstractControl {
  Q_OBJECT

public:
  MonitoringMode();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class MonitorEyesThreshold : public AbstractControl {
  Q_OBJECT

public:
  MonitorEyesThreshold();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class NormalEyesThreshold : public AbstractControl {
  Q_OBJECT

public:
  NormalEyesThreshold();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class BlinkThreshold : public AbstractControl {
  Q_OBJECT

public:
  BlinkThreshold();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class FanSpeedGain : public AbstractControl {
  Q_OBJECT

public:
  FanSpeedGain();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class CruiseGapTR : public AbstractControl {
  Q_OBJECT

public:
  CruiseGapTR();

private:
  QPushButton btn1;
  QPushButton btn2;
  QPushButton btn3;
  QPushButton btn4;
  QLabel label1;
  QLabel label2;
  QLabel label3;
  QLabel label4;
  QLabel label1a;
  QLabel label2a;
  QLabel label3a;
  QLabel label4a;
  Params params;
  
  void refresh1();
  void refresh2();
  void refresh3();
  void refresh4();
};

class DynamicTR : public AbstractControl {
  Q_OBJECT

public:
  DynamicTR();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class LCTimingFactor : public AbstractControl {
  Q_OBJECT

public:
  LCTimingFactor();

private:
  QPushButton btn1;
  QPushButton btn2;
  QPushButton btn3;
  QPushButton btn4;
  QLabel label1;
  QLabel label2;
  QLabel label3;
  QLabel label4;
  QLabel label1a;
  QLabel label2a;
  QLabel label3a;
  QLabel label4a;
  Params params;

  void refresh1();
  void refresh2();
  void refresh3();
  void refresh4();
};

class LCTimingFactorUD : public AbstractControl {
  Q_OBJECT

public:
  LCTimingFactorUD();

private:
  QPushButton btn;
  QPushButton btn2;
  Params params;
  
  void refresh();
  void refresh2();
};

class AutoResCondition : public AbstractControl {
  Q_OBJECT

public:
  AutoResCondition();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class AutoResLimitTime : public AbstractControl {
  Q_OBJECT

public:
  AutoResLimitTime();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class AutoEnableSpeed : public AbstractControl {
  Q_OBJECT

public:
  AutoEnableSpeed();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class CamDecelDistAdd : public AbstractControl {
  Q_OBJECT

public:
  CamDecelDistAdd();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class LiveSRPercent : public AbstractControl {
  Q_OBJECT

public:
  LiveSRPercent();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class VCurvSpeed : public AbstractControl {
  Q_OBJECT

public:
  VCurvSpeed();

private:
  QPushButton btn1;
  QPushButton btn2;
  QPushButton btn3;
  QPushButton btn4;
  QLabel label1;
  QLabel label2;
  QLabel label3;
  QLabel label4;
  QLabel label1a;
  QLabel label2a;
  QLabel label3a;
  QLabel label4a;
  Params params;

  void refresh1();
  void refresh2();
  void refresh3();
  void refresh4();
};

class VCurvSpeedUD : public AbstractControl {
  Q_OBJECT

public:
  VCurvSpeedUD();

private:
  QPushButton btn;
  Params params;
  
  void refresh();
};

class OCurvOffset : public AbstractControl {
  Q_OBJECT

public:
  OCurvOffset();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class GetOffAlert : public AbstractControl {
  Q_OBJECT

public:
  GetOffAlert();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class OPKRNaviSelect : public AbstractControl {
  Q_OBJECT

public:
  OPKRNaviSelect();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class OPKRServerSelect : public AbstractControl {
  Q_OBJECT

public:
  OPKRServerSelect();

private:
  QPushButton btn1;
  QPushButton btn2;
  QPushButton btn3;
  Params params;
  
  void refresh();
};

class OPKRServerAPI : public AbstractControl {
  Q_OBJECT

public:
  OPKRServerAPI();

private:
  QLabel label;
  QPushButton btn;
  Params params;

  void refresh();
};

class OPKRMapboxStyle : public AbstractControl {
  Q_OBJECT

public:
  OPKRMapboxStyle();

private:
  QPushButton btn1;
  QPushButton btn2;
  QPushButton btn3;
  Params params;
  
  void refresh();
};

class RESCountatStandstill : public AbstractControl {
  Q_OBJECT

public:
  RESCountatStandstill();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class SpeedLimitSignType : public AbstractControl {
  Q_OBJECT

public:
  SpeedLimitSignType();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class RadarLongHelperOption : public AbstractControl {
  Q_OBJECT

public:
  RadarLongHelperOption();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class CurvDecelSelect : public AbstractControl {
  Q_OBJECT

public:
  CurvDecelSelect();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class AutoRESDelay : public AbstractControl {
  Q_OBJECT

public:
  AutoRESDelay();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};