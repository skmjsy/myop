#pragma once

#include <QButtonGroup>
#include <QFileSystemWatcher>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>
#include <QStackedLayout>
#include <QComboBox>

#include "selfdrive/ui/qt/widgets/controls.h"
#include "selfdrive/ui/qt/widgets/groupWidget.h"
#include "selfdrive/ui/ui.h"

// ********** settings window + top-level panels **********
class SettingsWindow : public QFrame {
  Q_OBJECT

public:
  explicit SettingsWindow(QWidget *parent = 0);

protected:
  void hideEvent(QHideEvent *event) override;
  void showEvent(QShowEvent *event) override;

signals:
  void closeSettings();
  void reviewTrainingGuide();
  void showDriverView();

private:
  QPushButton *sidebar_alert_widget;
  QWidget *sidebar_widget;
  QButtonGroup *nav_btns;
  QStackedWidget *panel_widget;
};

class DevicePanel : public ListWidget {
  Q_OBJECT
public:
  explicit DevicePanel(SettingsWindow *parent);
signals:
  void reviewTrainingGuide();
  void showDriverView();
  void closeSettings();

private slots:
  void poweroff();
  void reboot();
  void updateCalibDescription();

private:
  Params params;
};

class TogglesPanel : public ListWidget {
  Q_OBJECT
public:
  explicit TogglesPanel(SettingsWindow *parent);
};

class SoftwarePanel : public ListWidget {
  Q_OBJECT
public:
  explicit SoftwarePanel(QWidget* parent = nullptr);

private:
  void showEvent(QShowEvent *event) override;
  void updateLabels();

  LabelControl *gitBranchLbl;
  LabelControl *gitCommitLbl;
  LabelControl *osVersionLbl;
  LabelControl *versionLbl;
  LabelControl *lastUpdateLbl;
  ButtonControl *updateBtn;

  Params params;
  QFileSystemWatcher *fs_watch;
};

class C2NetworkPanel: public QWidget {
  Q_OBJECT
public:
  explicit C2NetworkPanel(QWidget* parent = nullptr);

private:
  void showEvent(QShowEvent *event) override;
  QString getIPAddress();
  LabelControl *ipaddress;
};




class SelectCar : public QWidget {
  Q_OBJECT
public:
  explicit SelectCar(QWidget* parent = 0);

private:

signals:
  void backPress();
  void selectedCar();

};

class LateralControl : public QWidget {
  Q_OBJECT
public:
  explicit LateralControl(QWidget* parent = 0);

private:

signals:
  void backPress();
  void selected();

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

class CommunityPanel : public QWidget {
  Q_OBJECT

private:
  QStackedLayout* main_layout = nullptr;
  QWidget* homeScreen = nullptr;
  SelectCar* selectCar = nullptr;
  LateralControl* lateralControl = nullptr;

  QWidget* homeWidget;

public:
  explicit CommunityPanel(QWidget *parent = nullptr);
};

class CloseToRoadEdgeToggle : public ToggleControl {
  Q_OBJECT

public:
  CloseToRoadEdgeToggle() : ToggleControl(tr("Driving Close to RoadEdge"), tr("This will adjust the camera offset to get close to road edge if the car is on the first or last lane."), "../assets/offroad/icon_road.png", Params().getBool("CloseToRoadEdge")) {
    QObject::connect(this, &CloseToRoadEdgeToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("CloseToRoadEdge", status);
    });
  }
};

class OPKREdgeOffset : public AbstractControl {
  Q_OBJECT

public:
  OPKREdgeOffset();

private:
  QPushButton btnplusl;
  QPushButton btnminusl;
  QPushButton btnplusr;
  QPushButton btnminusr;
  QLabel labell1;
  QLabel labelr1;
  QLabel labell;
  QLabel labelr;
  Params params;
  
  void refreshl();
  void refreshr();
};

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

class LiveSteerRatioToggle : public ToggleControl {
  Q_OBJECT

public:
  LiveSteerRatioToggle() : ToggleControl(tr("Use Live SteerRatio"), tr("Live SteerRatio is used instead of variable/fixed SteerRatio."), "../assets/offroad/icon_openpilot.png", Params().getBool("OpkrLiveSteerRatio")) {
    QObject::connect(this, &LiveSteerRatioToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("OpkrLiveSteerRatio", status);
    });
  }
};

class UseNpilotManagerToggle : public ToggleControl {
  Q_OBJECT

public:
  UseNpilotManagerToggle() : ToggleControl(tr("Use nPilot Manager"), tr("Use nPilot Manager instead of UI Screen touch."), "../assets/offroad/icon_shell.png", Params().getBool("UseNpilotManager")) {
    QObject::connect(this, &UseNpilotManagerToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("UseNpilotManager", status);
    });
  }
};

class VariableSteerMaxToggle : public ToggleControl {
  Q_OBJECT

public:
  VariableSteerMaxToggle() : ToggleControl(tr("SteerMax/Variable SteerMax Toggle"), tr("Use the variable SteerMax by curvature. If this is off, runs only with base value below. STBase: SteerMax Default value. STMax: SteerMax Maximum value."), "../assets/offroad/icon_openpilot.png", Params().getBool("OpkrVariableSteerMax")) {
    QObject::connect(this, &VariableSteerMaxToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("OpkrVariableSteerMax", status);
    });
  }
};

class SRBaseControl : public AbstractControl {
  Q_OBJECT

public:
  SRBaseControl();

private:
  QPushButton btndigit;
  QPushButton btnminus;
  QPushButton btnplus;
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

class TorqueMaxLatAccel : public AbstractControl {
  Q_OBJECT

public:
  TorqueMaxLatAccel();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class TorqueFriction : public AbstractControl {
  Q_OBJECT

public:
  TorqueFriction();

private:
  QPushButton btnplus;
  QPushButton btnminus;
  QLabel label;
  Params params;
  
  void refresh();
};

class AutoEnabledToggle : public ToggleControl {
  Q_OBJECT

public:
  AutoEnabledToggle() : ToggleControl(tr("Use Auto Engagement"), tr("If the cruise button status is standby (CRUISE indication only and speed is not specified) in the Disengagement state, activate the automatic Engagement."), "../assets/offroad/icon_openpilot.png", Params().getBool("AutoEnable")) {
    QObject::connect(this, &AutoEnabledToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("AutoEnable", status);
    });
  }
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

class AutoCruiseSetToggle : public ToggleControl {
  Q_OBJECT

public:
  AutoCruiseSetToggle() : ToggleControl(tr("Use Auto Cruise "), "", "../assets/offroad/icon_road.png", Params().getBool("AutoCruiseSet")) {
    QObject::connect(this, &AutoCruiseSetToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("AutoCruiseSet", status);
    });
  }
};

class AutoCruiseSetDependsOnNdaToggle : public ToggleControl {
  Q_OBJECT

public:
  AutoCruiseSetDependsOnNdaToggle() : ToggleControl(tr("Auto Cruise Set Depends On Nda "), "", "../assets/offroad/icon_road.png", Params().getBool("AutoCruiseSetDependsOnNda")) {
    QObject::connect(this, &AutoCruiseSetDependsOnNdaToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("AutoCruiseSetDependsOnNda", status);
    });
  }
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

class UseBaseTorqueToggle : public ToggleControl {
  Q_OBJECT

public:
  UseBaseTorqueToggle() : ToggleControl(tr("Use Base Torque Values"), "", "../assets/offroad/icon_openpilot.png", Params().getBool("UseBaseTorqueValues")) {
    QObject::connect(this, &UseBaseTorqueToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("UseBaseTorqueValues", status);
    });
  }
};

class CTorqueControlGroup : public CGroupWidget 
{
  Q_OBJECT

public:
  explicit CTorqueControlGroup();

};

class UseLiveTorqueToggle : public ToggleControl {
  Q_OBJECT

public:
  UseLiveTorqueToggle() : ToggleControl(tr("Enable Live Torque"), "", "../assets/offroad/icon_openpilot.png", Params().getBool("IsLiveTorque")) {
    QObject::connect(this, &UseLiveTorqueToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("IsLiveTorque", status);
    });
  }
};

class HotspotOnBootToggle : public ToggleControl {
  Q_OBJECT

public:
  HotspotOnBootToggle() : ToggleControl(tr("HotSpot on Boot"), tr("It automatically runs a hotspot when booting."), "", Params().getBool("OpkrHotspotOnBoot")) {
    QObject::connect(this, &HotspotOnBootToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("OpkrHotspotOnBoot", status);
    });
  }
};

class LowSpeedFactorToggle : public ToggleControl {
  Q_OBJECT

public:
  LowSpeedFactorToggle() : ToggleControl(tr("Enable Low Speed Factor"), "", "../assets/offroad/icon_openpilot.png", Params().getBool("IsLowSpeedFactor")) {
    QObject::connect(this, &LowSpeedFactorToggle::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("IsLowSpeedFactor", status);
    });
  }
};

class DepartChimeAtResume : public ToggleControl {
  Q_OBJECT

public:
  DepartChimeAtResume() : ToggleControl(tr("Depart Chime at Resume"), tr("Use Chime for Resume. This can notify for you to get start while not using SCC."), "../assets/offroad/icon_shell.png", Params().getBool("DepartChimeAtResume")) {
    QObject::connect(this, &DepartChimeAtResume::toggleFlipped, [=](int state) {
      bool status = state ? true : false;
      Params().putBool("DepartChimeAtResume", status);
    });
  }
};

