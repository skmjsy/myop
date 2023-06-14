#include <QMap>
#include <QSoundEffect>
#include <QString>

#include "selfdrive/hardware/hw.h"
#include "selfdrive/ui/ui.h"

const std::tuple<AudibleAlert, QString, int> sound_list[] = {
  // AudibleAlert, file name, loop count
  {AudibleAlert::ENGAGE, "engage.wav", 0},
  {AudibleAlert::DISENGAGE, "disengage.wav", 0},
  {AudibleAlert::REFUSE, "refuse.wav", 0},

  {AudibleAlert::PROMPT, "prompt.wav", 0},
  {AudibleAlert::PROMPT_REPEAT, "prompt.wav", QSoundEffect::Infinite},
  {AudibleAlert::PROMPT_DISTRACTED, "prompt_distracted.wav", QSoundEffect::Infinite},

  {AudibleAlert::WARNING_SOFT, "warning_soft.wav", QSoundEffect::Infinite},
  //{AudibleAlert::WARNING_IMMEDIATE, "warning_immediate.wav", QSoundEffect::Infinite},
  {AudibleAlert::WARNING_IMMEDIATE, "warning_immediate.wav", 2},

  {AudibleAlert::LONG_ENGAGED, "prompt.wav", 0},
  {AudibleAlert::LONG_DISENGAGED, "prompt.wav", 0},
  {AudibleAlert::TRAFFIC_SIGN_GREEN, "prompt.wav", 0},
  {AudibleAlert::TRAFFIC_SIGN_CHANGED, "prompt.wav", 0},
  {AudibleAlert::TRAFFIC_ERROR, "prompt.wav", 0},
  {AudibleAlert::BSD_WARNING, "prompt.wav", 0},
  {AudibleAlert::LANE_CHANGE, "prompt.wav", 0},
  {AudibleAlert::STOP_STOP, "prompt.wav", 0},
  {AudibleAlert::STOPPING, "prompt.wav", 0},
  {AudibleAlert::AUTO_HOLD, "prompt.wav", 0},
  {AudibleAlert::ENGAGE2, "prompt.wav", 0},
  {AudibleAlert::DISENGAGE2, "prompt.wav", 0},
  {AudibleAlert::SPEED_DOWN, "prompt.wav", 0},
};

class Sound : public QObject {
public:
  explicit Sound(QObject *parent = 0);

protected:
  void update();
  void setAlert(const Alert &alert);

  Alert current_alert = {};
  QMap<AudibleAlert, QPair<QSoundEffect *, int>> sounds;
  SubMaster sm;
  uint64_t started_frame;
};
