#include "selfdrive/ui/qt/offroad/settings.h"

#include <cassert>
#include <cmath>
#include <string>

#include <QDebug>
#include <QProcess> // opkr
#include <QDateTime> // opkr
#include <QTimer> // opkr

#ifndef QCOM
#include "selfdrive/ui/qt/offroad/networking.h"
#endif

#ifdef ENABLE_MAPS
#include "selfdrive/ui/qt/maps/map_settings.h"
#endif

#include "selfdrive/common/params.h"
#include "selfdrive/common/util.h"
#include "selfdrive/hardware/hw.h"
#include "selfdrive/ui/qt/widgets/controls.h"
#include "selfdrive/ui/qt/widgets/input.h"
#include "selfdrive/ui/qt/widgets/scrollview.h"
#include "selfdrive/ui/qt/widgets/ssh_keys.h"
#include "selfdrive/ui/qt/widgets/toggle.h"
#include "selfdrive/ui/ui.h"
#include "selfdrive/ui/qt/util.h"
#include "selfdrive/ui/qt/qt_window.h"
#include "selfdrive/ui/qt/widgets/opkr.h"

TogglesPanel::TogglesPanel(SettingsWindow *parent) : ListWidget(parent) {
  // param, title, desc, icon
  std::vector<std::tuple<QString, QString, QString, QString>> toggles{
    {
      "OpenpilotEnabledToggle",
      "Enable openpilot",
      "Use the openpilot system for adaptive cruise control and lane keep driver assistance. Your attention is required at all times to use this feature. Changing this setting takes effect when the car is powered off.",
      "../assets/offroad/icon_openpilot.png",
    },
    {
      "IsLdwEnabled",
      "Enable Lane Departure Warnings",
      "Receive alerts to steer back into the lane when your vehicle drifts over a detected lane line without a turn signal activated while driving over 31mph (50kph).",
      "../assets/offroad/icon_warning.png",
    },
    {
      "IsRHD",
      "Enable Right-Hand Drive",
      "Allow openpilot to obey left-hand traffic conventions and perform driver monitoring on right driver seat.",
      "../assets/offroad/icon_openpilot_mirrored.png",
    },
    {
      "IsMetric",
      "Use Metric System",
      "Display speed in km/h instead of mph.",
      "../assets/offroad/icon_metric.png",
    },
    {
      "CommunityFeaturesToggle",
      "Enable Community Features",
      "Use features, such as community supported hardware, from the open source community that are not maintained or supported by comma.ai and have not been confirmed to meet the standard safety model. Be extra cautious when using these features",
      "../assets/offroad/icon_shell.png",
    },
    {
      "UploadRaw",
      "Upload Raw Logs",
      "Upload full logs and full resolution video by default while on Wi-Fi. If not enabled, individual logs can be marked for upload at useradmin.comma.ai.",
      "../assets/offroad/icon_network.png",
    },
    {
      "RecordFront",
      "Record and Upload Driver Camera",
      "Upload data from the driver facing camera and help improve the driver monitoring algorithm.",
      "../assets/offroad/icon_monitoring.png",
    },
    {
      "EndToEndToggle",
      "\U0001f96c Enable Lane selector Mode \U0001f96c",
      "Activate lane selection mode. Lane Mode/Lane Less/AUTO can be selected and switched on the screen.",
      "../assets/offroad/icon_road.png",
    },
#ifdef ENABLE_MAPS
    {
      "NavSettingTime24h",
      "Show ETA in 24h format",
      "Use 24h format instead of am/pm",
      "../assets/offroad/icon_metric.png",
    },
#endif

    {
      "OpkrEnableDriverMonitoring",
      "Enable Driver Monitoring",
      "Use the driver monitoring function.",
      "../assets/offroad/icon_shell.png",
    },
    {
      "OpkrEnableLogger",
      "Enable Driving Log Record",
      "Record the driving log locally for data analysis. Only loggers are activated and not uploaded to the server.",
      "../assets/offroad/icon_shell.png",
    },
    {
      "OpkrEnableUploader",
      "Enable Sending Log to Server",
      "Activate the upload process to transmit system logs and other driving data to the server. Upload it only off-road.",
      "../assets/offroad/icon_shell.png",
    },
    {
      "CommaStockUI",
      "Use Comma Stock UI",
      "Use the Stock UI of the comma on the driving screen. You can switch to real-time by pressing the box in the upper left corner of the driving screen.",
      "../assets/offroad/icon_shell.png",
    },
  };

  Params params;

  if (params.getBool("DisableRadar_Allow")) {
    toggles.push_back({
      "DisableRadar",
      "openpilot Longitudinal Control",
      "openpilot will disable the car's radar and will take over control of gas and brakes. Warning: this disables AEB!",
      "../assets/offroad/icon_speed_limit.png",
    });
  }

  for (auto &[param, title, desc, icon] : toggles) {
    auto toggle = new ParamControl(param, title, desc, icon, this);
    //bool locked = params.getBool((param + "Lock").toStdString());
    //toggle->setEnabled(true);
    //connect(parent, &SettingsWindow::offroadTransition, toggle, &ParamControl::setEnabled);
    addItem(toggle);
  }
}

DevicePanel::DevicePanel(SettingsWindow *parent) : ListWidget(parent) {
  setSpacing(50);
  addItem(new LabelControl("Dongle ID", getDongleId().value_or("N/A")));
  addItem(new LabelControl("Serial", params.get("HardwareSerial").c_str()));

  addItem(new OpenpilotView());

  // offroad-only buttons

  auto dcamBtn = new ButtonControl("Driver Camera", "PREVIEW",
                                   "Preview the driver facing camera to help optimize device mounting position for best driver monitoring experience. (vehicle must be off)");
  connect(dcamBtn, &ButtonControl::clicked, [=]() { emit showDriverView(); });
  addItem(dcamBtn);

  if (!params.getBool("Passive")) {
    auto retrainingBtn = new ButtonControl("Review Training Guide", "REVIEW", "Review the rules, features, and limitations of openpilot");
    connect(retrainingBtn, &ButtonControl::clicked, [=]() {
      if (ConfirmationDialog::confirm("Are you sure you want to review the training guide?", this)) {
        emit reviewTrainingGuide();
      }
    });
    addItem(retrainingBtn);
  }

  if (Hardware::TICI()) {
    auto regulatoryBtn = new ButtonControl("Regulatory", "VIEW", "");
    connect(regulatoryBtn, &ButtonControl::clicked, [=]() {
      const std::string txt = util::read_file("../assets/offroad/fcc.html");
      RichTextDialog::alert(QString::fromStdString(txt), this);
    });
    addItem(regulatoryBtn);
  }

  auto resetCalibBtn = new ButtonControl("Reset Calibration", "RESET", " ");
  connect(resetCalibBtn, &ButtonControl::showDescription, this, &DevicePanel::updateCalibDescription);
  connect(resetCalibBtn, &ButtonControl::clicked, [&]() {
    if (ConfirmationDialog::confirm("Are you sure you want to reset calibration? Device will be rebooted.", this)) {
      params.remove("CalibrationParams");
      params.remove("LiveParameters");
      QTimer::singleShot(1000, []() {
        Hardware::reboot();
      });
    }
  });
  addItem(resetCalibBtn);

  QObject::connect(parent, &SettingsWindow::offroadTransition, [=](bool offroad) {
    for (auto btn : findChildren<ButtonControl *>()) {
      btn->setEnabled(offroad);
    }
    resetCalibBtn->setEnabled(true);
  });

  // power buttons
  QHBoxLayout *power_layout = new QHBoxLayout();
  power_layout->setSpacing(30);

  QPushButton *reboot_btn = new QPushButton("Reboot");
  reboot_btn->setObjectName("reboot_btn");
  power_layout->addWidget(reboot_btn);
  QObject::connect(reboot_btn, &QPushButton::clicked, this, &DevicePanel::reboot);

  QPushButton *poweroff_btn = new QPushButton("Power Off");
  poweroff_btn->setObjectName("poweroff_btn");
  power_layout->addWidget(poweroff_btn);
  QObject::connect(poweroff_btn, &QPushButton::clicked, this, &DevicePanel::poweroff);

  setStyleSheet(R"(
    QPushButton {
      height: 120px;
      border-radius: 15px;
    }
    #reboot_btn { background-color: #393939; }
    #reboot_btn:pressed { background-color: #4a4a4a; }
    #poweroff_btn { background-color: #E22C2C; }
    #poweroff_btn:pressed { background-color: #FF2424; }
  )");
  addItem(power_layout);
}

void DevicePanel::updateCalibDescription() {
  QString desc =
      "openpilot requires the device to be mounted within 4° left or right and "
      "within 5° up or down. openpilot is continuously calibrating, resetting is rarely required.";
  std::string calib_bytes = Params().get("CalibrationParams");
  if (!calib_bytes.empty()) {
    try {
      AlignedBuffer aligned_buf;
      capnp::FlatArrayMessageReader cmsg(aligned_buf.align(calib_bytes.data(), calib_bytes.size()));
      auto calib = cmsg.getRoot<cereal::Event>().getLiveCalibration();
      if (calib.getCalStatus() != 0) {
        double pitch = calib.getRpyCalib()[1] * (180 / M_PI);
        double yaw = calib.getRpyCalib()[2] * (180 / M_PI);
        desc += QString(" Your device is pointed %1° %2 and %3° %4.")
                    .arg(QString::number(std::abs(pitch), 'g', 1), pitch > 0 ? "down" : "up",
                         QString::number(std::abs(yaw), 'g', 1), yaw > 0 ? "left" : "right");
      }
    } catch (kj::Exception) {
      qInfo() << "invalid CalibrationParams";
    }
  }
  qobject_cast<ButtonControl *>(sender())->setDescription(desc);
}

void DevicePanel::reboot() {
  if (QUIState::ui_state.status == UIStatus::STATUS_DISENGAGED) {
    if (ConfirmationDialog::confirm("Are you sure you want to reboot?", this)) {
      // Check engaged again in case it changed while the dialog was open
      if (QUIState::ui_state.status == UIStatus::STATUS_DISENGAGED) {
        Params().putBool("DoReboot", true);
      }
    }
  } else {
    ConfirmationDialog::alert("Disengage to Reboot", this);
  }
}

void DevicePanel::poweroff() {
  if (QUIState::ui_state.status == UIStatus::STATUS_DISENGAGED) {
    if (ConfirmationDialog::confirm("Are you sure you want to power off?", this)) {
      // Check engaged again in case it changed while the dialog was open
      if (QUIState::ui_state.status == UIStatus::STATUS_DISENGAGED) {
        Params().putBool("DoShutdown", true);
      }
    }
  } else {
    ConfirmationDialog::alert("Disengage to Power Off", this);
  }
}

SoftwarePanel::SoftwarePanel(QWidget* parent) : ListWidget(parent) {
  gitRemoteLbl = new LabelControl("Git Remote");
  gitBranchLbl = new LabelControl("Git Branch");
  gitCommitLbl = new LabelControl("Git Commit");
  osVersionLbl = new LabelControl("OS Version");
  versionLbl = new LabelControl("Version");
  lastUpdateLbl = new LabelControl("Last Updates Date", "", "");
  updateBtn = new ButtonControl("Check for Updates", "");
  connect(updateBtn, &ButtonControl::clicked, [=]() {
    if (params.getBool("IsOffroad")) {
      fs_watch->addPath(QString::fromStdString(params.getParamPath("LastUpdateTime")));
      fs_watch->addPath(QString::fromStdString(params.getParamPath("UpdateFailedCount")));
    }
    std::system("/data/openpilot/selfdrive/assets/addon/script/gitcommit.sh");
    std::system("date '+%F %T' > /data/params/d/LastUpdateTime");
    QString last_ping = QString::fromStdString(params.get("LastAthenaPingTime"));
    QString desc = "";
    QString commit_local = QString::fromStdString(Params().get("GitCommit").substr(0, 10));
    QString commit_remote = QString::fromStdString(Params().get("GitCommitRemote").substr(0, 10));
    QString empty = "";
    desc += QString("LOCAL: %1\nREMOT: %2%3%4\n").arg(commit_local, commit_remote, empty, empty);
    
    if (!last_ping.length()) {
      desc += QString("Network connection is missing or unstable. Check the connection.");
    } else if (commit_local == commit_remote) {
      desc += QString("Local and remote match. No update required.");
    } else {
      desc += QString("There's an update. Press the OK button to go.");
    }
    if (ConfirmationDialog::confirm(desc, this)) {
      std::system("/data/openpilot/selfdrive/assets/addon/script/gitpull.sh");
    }
  });


  auto uninstallBtn = new ButtonControl("Uninstall " + getBrand(), "UNINSTALL");
  connect(uninstallBtn, &ButtonControl::clicked, [&]() {
    if (ConfirmationDialog::confirm("Are you sure you want to uninstall?", this)) {
      params.putBool("DoUninstall", true);
    }
  });
  connect(parent, SIGNAL(offroadTransition(bool)), uninstallBtn, SLOT(setEnabled(bool)));

  QWidget *widgets[] = {osVersionLbl, versionLbl, gitRemoteLbl, gitBranchLbl, lastUpdateLbl, updateBtn};
  for (QWidget* w : widgets) {
    addItem(w);
  }

  addItem(new GitHash());
  addItem(new GitPullOnBootToggle());

  // preset1 buttons
  QHBoxLayout *presetone_layout = new QHBoxLayout();
  presetone_layout->setSpacing(50);

  QPushButton *presetoneload_btn = new QPushButton("Load Preset1");
  presetoneload_btn->setStyleSheet("height: 120px;border-radius: 15px;background-color: #393939;");
  presetone_layout->addWidget(presetoneload_btn);
  QObject::connect(presetoneload_btn, &QPushButton::clicked, [=]() {
    if (ConfirmationDialog::confirm("Do you want to load Preset1?", this)) {
      QProcess::execute("/data/openpilot/selfdrive/assets/addon/script/load_preset1.sh");
    }
  });

  QPushButton *presetonesave_btn = new QPushButton("Save Preset1");
  presetonesave_btn->setStyleSheet("height: 120px;border-radius: 15px;background-color: #393939;");
  presetone_layout->addWidget(presetonesave_btn);
  QObject::connect(presetonesave_btn, &QPushButton::clicked, [=]() {
    if (ConfirmationDialog::confirm("Do you want to save Preset1?", this)) {
      QProcess::execute("/data/openpilot/selfdrive/assets/addon/script/save_preset1.sh");
    }
  });

  // preset2 buttons
  QHBoxLayout *presettwo_layout = new QHBoxLayout();
  presettwo_layout->setSpacing(50);

  QPushButton *presettwoload_btn = new QPushButton("Load Preset2");
  presettwoload_btn->setStyleSheet("height: 120px;border-radius: 15px;background-color: #393939;");
  presettwo_layout->addWidget(presettwoload_btn);
  QObject::connect(presettwoload_btn, &QPushButton::clicked, [=]() {
    if (ConfirmationDialog::confirm("Do you want to load Preset2?", this)) {
      QProcess::execute("/data/openpilot/selfdrive/assets/addon/script/load_preset2.sh");
    }
  });

  QPushButton *presettwosave_btn = new QPushButton("Save Preset2");
  presettwosave_btn->setStyleSheet("height: 120px;border-radius: 15px;background-color: #393939;");
  presettwo_layout->addWidget(presettwosave_btn);
  QObject::connect(presettwosave_btn, &QPushButton::clicked, [=]() {
    if (ConfirmationDialog::confirm("Do you want to save Preset2?", this)) {
      QProcess::execute("/data/openpilot/selfdrive/assets/addon/script/save_preset2.sh");
    }
  });

  auto paraminit_btn = new ButtonControl("Parameters Init", "RUN");
  QObject::connect(paraminit_btn, &ButtonControl::clicked, [=]() {
    if (ConfirmationDialog::confirm("Initialize parameters. Changes in the EON menu are changed to the initial set value. Do you want to proceed?", this)){
      QProcess::execute("/data/openpilot/selfdrive/assets/addon/script/init_param.sh");
    }
  });

  addItem(presetone_layout);
  addItem(presettwo_layout);

  addItem(paraminit_btn);

  const char* git_reset = "/data/openpilot/selfdrive/assets/addon/script/git_reset.sh ''";
  auto gitresetbtn = new ButtonControl("Git Reset", "RUN");
  QObject::connect(gitresetbtn, &ButtonControl::clicked, [=]() {
    if (ConfirmationDialog::confirm("Apply the latest commitment details of Remote Git after forced initialization of local changes. Do you want to proceed?", this)){
      std::system(git_reset);
    }
  });
  addItem(gitresetbtn);

  const char* gitpull_cancel = "/data/openpilot/selfdrive/assets/addon/script/gitpull_cancel.sh ''";
  auto gitpullcanceltbtn = new ButtonControl("Cancel Git Pull", "RUN");
  QObject::connect(gitpullcanceltbtn, &ButtonControl::clicked, [=]() {
    if (ConfirmationDialog::confirm("Return to the state before GitPull. Do you want to proceed?", this)){
      std::system(gitpull_cancel);
    }
  });
  addItem(gitpullcanceltbtn);

  const char* panda_flashing = "/data/openpilot/selfdrive/assets/addon/script/panda_flashing.sh ''";
  auto pandaflashingtbtn = new ButtonControl("Panda Flashing", "RUN");
  QObject::connect(pandaflashingtbtn, &ButtonControl::clicked, [=]() {
    if (ConfirmationDialog::confirm("Panda's green LED blinks quickly during panda flashing. Never turn off or disconnect the device arbitrarily. Do you want to proceed?", this)) {
      std::system(panda_flashing);
    }
  });

  addItem(pandaflashingtbtn);
  addItem(new SwitchOpenpilot()); // opkr

  const char* open_settings = "am start -a android.intent.action.MAIN -n com.android.settings/.Settings";
  auto open_settings_btn = new ButtonControl("Open Android Settings", "RUN");
  QObject::connect(open_settings_btn, &ButtonControl::clicked, [=]() {
    emit closeSettings();
    std::system(open_settings);
  });
  addItem(open_settings_btn);
  const char* softkey = "am start com.gmd.hidesoftkeys/com.gmd.hidesoftkeys.MainActivity";
  auto softkey_btn = new ButtonControl("SoftKey RUN/SET", "RUN");
  QObject::connect(softkey_btn, &ButtonControl::clicked, [=]() {
    emit closeSettings();
    std::system(softkey);
  });
  addItem(softkey_btn);
  auto mixplorer_btn = new ButtonControl("RUN Mixplorer", "RUN");
  QObject::connect(mixplorer_btn, &ButtonControl::clicked, [=]() {
	  emit closeSettings();
    std::system("/data/openpilot/selfdrive/assets/addon/script/run_mixplorer.sh");
  });
  addItem(mixplorer_btn);
  addItem(uninstallBtn);
  fs_watch = new QFileSystemWatcher(this);
  QObject::connect(fs_watch, &QFileSystemWatcher::fileChanged, [=](const QString path) {
    if (path.contains("UpdateFailedCount") && std::atoi(params.get("UpdateFailedCount").c_str()) > 0) {
      lastUpdateLbl->setText("failed to fetch update");
      updateBtn->setText("CHECK");
      updateBtn->setEnabled(true);
    } else if (path.contains("LastUpdateTime")) {
      updateLabels();
    }
  });
}

void SoftwarePanel::showEvent(QShowEvent *event) {
  updateLabels();
}

void SoftwarePanel::updateLabels() {
  QString lastUpdate = "";
  QString tm = QString::fromStdString(params.get("LastUpdateTime").substr(0, 19));
  if (tm != "") {
    lastUpdate = timeAgo(QDateTime::fromString(tm, "yyyy-MM-dd HH:mm:ss"));
  }

  versionLbl->setText(getBrandVersion());
  lastUpdateLbl->setText(lastUpdate);
  updateBtn->setText("CHECK");
  updateBtn->setEnabled(true);
  gitRemoteLbl->setText(QString::fromStdString(params.get("GitRemote").substr(19)));
  gitBranchLbl->setText(QString::fromStdString(params.get("GitBranch")));
  gitCommitLbl->setText(QString::fromStdString(params.get("GitCommit")).left(10));
  osVersionLbl->setText(QString::fromStdString(Hardware::get_os_version()).trimmed());
}

C2NetworkPanel::C2NetworkPanel(QWidget *parent) : QWidget(parent) {
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(50, 0, 50, 0);

  ListWidget *list = new ListWidget();
  list->setSpacing(30);
  // wifi + tethering buttons
#ifdef QCOM
  auto wifiBtn = new ButtonControl("Wi-Fi Settings", "OPEN");
  QObject::connect(wifiBtn, &ButtonControl::clicked, [=]() { HardwareEon::launch_wifi(); });
  list->addItem(wifiBtn);

  auto tetheringBtn = new ButtonControl("Tethering Settings", "OPEN");
  QObject::connect(tetheringBtn, &ButtonControl::clicked, [=]() { HardwareEon::launch_tethering(); });
  list->addItem(tetheringBtn);
#endif
  ipaddress = new LabelControl("IP Address", "");
  list->addItem(ipaddress);

  list->addItem(new HotspotOnBootToggle());

  // SSH key management
  list->addItem(new SshToggle());
  list->addItem(new SshControl());
  list->addItem(new SshLegacyToggle());

  layout->addWidget(list);
  layout->addStretch(1);
}

void C2NetworkPanel::showEvent(QShowEvent *event) {
  ipaddress->setText(getIPAddress());
}

QString C2NetworkPanel::getIPAddress() {
  std::string result = util::check_output("ifconfig wlan0");
  if (result.empty()) return "";

  const std::string inetaddrr = "inet addr:";
  std::string::size_type begin = result.find(inetaddrr);
  if (begin == std::string::npos) return "";

  begin += inetaddrr.length();
  std::string::size_type end = result.find(' ', begin);
  if (end == std::string::npos) return "";

  return result.substr(begin, end - begin).c_str();
}

QWidget *network_panel(QWidget *parent) {
#ifdef QCOM
  return new C2NetworkPanel(parent);
#else
  return new Networking(parent);
#endif
}

UIPanel::UIPanel(QWidget *parent) : QFrame(parent) {
  QVBoxLayout *layout = new QVBoxLayout(this);

  layout->setContentsMargins(50, 0, 50, 0);
  layout->setSpacing(30);

  // OPKR
  layout->addWidget(new AutoShutdown());
  layout->addWidget(new ForceShutdown());
  layout->addWidget(new VolumeControl());
  layout->addWidget(new BrightnessControl());
  layout->addWidget(new AutoScreenOff());
  layout->addWidget(new BrightnessOffControl());
  layout->addWidget(new GetOffAlert());
  layout->addWidget(new BatteryChargingControlToggle());
  layout->addWidget(new ChargingMin());
  layout->addWidget(new ChargingMax());
  layout->addWidget(new DrivingRecordToggle());
  layout->addWidget(new RecordCount());
  layout->addWidget(new RecordQuality());
  const char* record_del = "rm -f /storage/emulated/0/videos/*";
  auto recorddelbtn = new ButtonControl("Delete All Recorded Files", "RUN");
  QObject::connect(recorddelbtn, &ButtonControl::clicked, [=]() {
    if (ConfirmationDialog::confirm("Delete all saved recorded files. Do you want to proceed?", this)){
      std::system(record_del);
    }
  });
  layout->addWidget(recorddelbtn);
  const char* realdata_del = "rm -rf /storage/emulated/0/realdata/*";
  auto realdatadelbtn = new ButtonControl("Delete All Driving Logs", "RUN");
  QObject::connect(realdatadelbtn, &ButtonControl::clicked, [=]() {
    if (ConfirmationDialog::confirm("Delete all saved driving logs. Do you want to proceed?", this)){
      std::system(realdata_del);
    }
  });
  layout->addWidget(realdatadelbtn);
  layout->addWidget(new MonitoringMode());
  layout->addWidget(new MonitorEyesThreshold());
  layout->addWidget(new NormalEyesThreshold());
  layout->addWidget(new BlinkThreshold());
  layout->addWidget(new OPKRNaviSelect());
  layout->addWidget(new RunNaviOnBootToggle());
  layout->addWidget(new KRDateToggle());
  layout->addWidget(new KRTimeToggle());
  layout->addWidget(new OPKRServerSelect());
  layout->addWidget(new OPKRServerAPI());
  layout->addWidget(new MapboxEnabledToggle());
  layout->addWidget(new OPKRMapboxStyle());
  layout->addWidget(new GoogleMapEnabledToggle());
}

DrivingPanel::DrivingPanel(QWidget *parent) : QFrame(parent) {
  QVBoxLayout *layout = new QVBoxLayout(this);

  layout->setContentsMargins(50, 0, 50, 0);
  layout->setSpacing(30);

  // OPKR
  layout->addWidget(new AutoResumeToggle());
  layout->addWidget(new RESCountatStandstill());
  layout->addWidget(new CruiseGapAdjustToggle());
  layout->addWidget(new StandstillResumeAltToggle());
  layout->addWidget(new VariableCruiseToggle());
  layout->addWidget(new CruisemodeSelInit());
  layout->addWidget(new LaneChangeSpeed());
  layout->addWidget(new LaneChangeDelay());
  layout->addWidget(new LCTimingFactorUD());
  layout->addWidget(new LCTimingFactor());
  layout->addWidget(new LeftCurvOffset());
  layout->addWidget(new RightCurvOffset());
  layout->addWidget(new BlindSpotDetectToggle());
  layout->addWidget(new MaxAngleLimit());
  layout->addWidget(new SteerAngleCorrection());
  layout->addWidget(new TurnSteeringDisableToggle());
  layout->addWidget(new CruiseOverMaxSpeedToggle());
  layout->addWidget(new OSMSpeedLimitEnabledToggle());
  layout->addWidget(new StockNaviSpeedToggle());
  layout->addWidget(new SpeedLimitOffset());
  layout->addWidget(new SpeedLimitSignType());
  layout->addWidget(new CamDecelDistAdd());
  layout->addWidget(new CurvDecelSelect());
  layout->addWidget(new VCurvSpeedUD());
  layout->addWidget(new VCurvSpeed());
  layout->addWidget(new OCurvOffset());
  layout->addWidget(new AutoEnabledToggle());
  layout->addWidget(new AutoEnableSpeed());
  layout->addWidget(new CruiseAutoResToggle());
  layout->addWidget(new RESChoice());
  layout->addWidget(new AutoResCondition());
  layout->addWidget(new AutoResLimitTime());
  layout->addWidget(new AutoRESDelay());
}

DeveloperPanel::DeveloperPanel(QWidget *parent) : QFrame(parent) {
  QVBoxLayout *layout = new QVBoxLayout(this);

  layout->setContentsMargins(50, 0, 50, 0);
  layout->setSpacing(30);

  // OPKR
  layout->addWidget(new DebugUiOneToggle());
  layout->addWidget(new DebugUiTwoToggle());
  layout->addWidget(new ShowErrorToggle());
  layout->addWidget(new LongLogToggle());
  layout->addWidget(new PrebuiltToggle());
  layout->addWidget(new FPTwoToggle());
  layout->addWidget(new WhitePandaSupportToggle());
  layout->addWidget(new BattLessToggle());
  layout->addWidget(new ComIssueToggle());
  layout->addWidget(new LDWSToggle());
  layout->addWidget(new GearDToggle());
  layout->addWidget(new SteerWarningFixToggle());
  layout->addWidget(new IgnoreCanErroronISGToggle());
  layout->addWidget(new FCA11MessageToggle());
  layout->addWidget(new SteerWindDownToggle());
  layout->addWidget(new MadModeEnabledToggle());
  layout->addWidget(new StockLKASEnabledatDisenagedStatusToggle());
  layout->addWidget(new TimeZoneSelectCombo());
  const char* cal_ok = "cp -f /data/openpilot/selfdrive/assets/addon/param/CalibrationParams /data/params/d/";
  auto calokbtn = new ButtonControl("Enable Calibration by Force", "RUN");
  QObject::connect(calokbtn, &ButtonControl::clicked, [=]() {
    if (ConfirmationDialog::confirm("Force calibration to be set. It's only for checking engagement, so please reset it when you actually drive.", this)){
      std::system(cal_ok);
    }
  });
  layout->addWidget(calokbtn);

  layout->addWidget(horizontal_line());
  layout->addWidget(new CarSelectCombo());

  layout->addWidget(horizontal_line());
  layout->addWidget(new LabelControl("〓〓〓〓〓〓〓【 Panda Values 】〓〓〓〓〓〓〓", ""));
  layout->addWidget(new MaxSteer());
  layout->addWidget(new MaxRTDelta());
  layout->addWidget(new MaxRateUp());
  layout->addWidget(new MaxRateDown());
  const char* p_edit_go = "/data/openpilot/selfdrive/assets/addon/script/p_edit.sh ''";
  auto peditbtn = new ButtonControl("Change Panda Values", "RUN");
  QObject::connect(peditbtn, &ButtonControl::clicked, [=]() {
    if (ConfirmationDialog::confirm("Apply the changed panda value. Do you want to proceed? It automatically reboots.", this)){
      std::system(p_edit_go);
    }
  });
  layout->addWidget(peditbtn);
}

TuningPanel::TuningPanel(QWidget *parent) : QFrame(parent) {
  QVBoxLayout *layout = new QVBoxLayout(this);

  layout->setContentsMargins(50, 0, 50, 0);
  layout->setSpacing(30);

  // OPKR
  layout->addWidget(new LabelControl("〓〓〓〓〓〓〓〓【 TUNING 】〓〓〓〓〓〓〓〓", ""));
  layout->addWidget(new CameraOffset());
  layout->addWidget(new PathOffset());
  layout->addWidget(new LiveSteerRatioToggle());
  layout->addWidget(new LiveSRPercent());
  layout->addWidget(new SRBaseControl());
  layout->addWidget(new SRMaxControl());
  layout->addWidget(new SteerActuatorDelay());
  layout->addWidget(new SteerRateCost());
  layout->addWidget(new SteerLimitTimer());
  layout->addWidget(new TireStiffnessFactor());
  layout->addWidget(new SteerMaxBase());
  layout->addWidget(new SteerMaxMax());
  layout->addWidget(new SteerMaxv());
  layout->addWidget(new VariableSteerMaxToggle());
  layout->addWidget(new SteerDeltaUpBase());
  layout->addWidget(new SteerDeltaUpMax());
  layout->addWidget(new SteerDeltaDownBase());
  layout->addWidget(new SteerDeltaDownMax());
  layout->addWidget(new VariableSteerDeltaToggle());
  layout->addWidget(new SteerThreshold());

  layout->addWidget(horizontal_line());

  layout->addWidget(new LabelControl("〓〓〓〓〓〓〓〓【 CONTROL 】〓〓〓〓〓〓〓〓", ""));
  layout->addWidget(new LateralControl());
  layout->addWidget(new LiveTunePanelToggle());
  QString lat_control = QString::fromStdString(Params().get("LateralControlMethod", false));
  if (lat_control == "0") {
    layout->addWidget(new PidKp());
    layout->addWidget(new PidKi());
    layout->addWidget(new PidKd());
    layout->addWidget(new PidKf());
  } else if (lat_control == "1") {
    layout->addWidget(new InnerLoopGain());
    layout->addWidget(new OuterLoopGain());
    layout->addWidget(new TimeConstant());
    layout->addWidget(new ActuatorEffectiveness());
  } else if (lat_control == "2") {
    layout->addWidget(new Scale());
    layout->addWidget(new LqrKi());
    layout->addWidget(new DcGain());
  }

  layout->addWidget(horizontal_line());

  layout->addWidget(new LabelControl("〓〓〓〓〓〓〓【 LONGCONTROL 】〓〓〓〓〓〓〓", ""));
  layout->addWidget(new DynamicTR());
  layout->addWidget(new CruiseGapTR());
  layout->addWidget(new RadarLongHelperOption());
  layout->addWidget(new StoppingDistAdjToggle());
  layout->addWidget(new E2ELongToggle());
}

void SettingsWindow::showEvent(QShowEvent *event) {
  panel_widget->setCurrentIndex(0);
  nav_btns->buttons()[0]->setChecked(true);
}

SettingsWindow::SettingsWindow(QWidget *parent) : QFrame(parent) {

  // setup two main layouts
  sidebar_widget = new QWidget;
  QVBoxLayout *sidebar_layout = new QVBoxLayout(sidebar_widget);
  sidebar_layout->setMargin(0);
  panel_widget = new QStackedWidget();
  panel_widget->setStyleSheet(R"(
    border-radius: 30px;
    background-color: #292929;
  )");

  // close button
  QPushButton *close_btn = new QPushButton("×");
  close_btn->setStyleSheet(R"(
    QPushButton {
      font-size: 140px;
      padding-bottom: 20px;
      font-weight: bold;
      border 1px grey solid;
      border-radius: 50px;
      background-color: #292929;
      font-weight: 400;
    }
    QPushButton:pressed {
      background-color: #3B3B3B;
    }
  )");
  close_btn->setFixedSize(220, 130);
  sidebar_layout->addSpacing(5);
  sidebar_layout->addWidget(close_btn, 0, Qt::AlignCenter);
  QObject::connect(close_btn, &QPushButton::clicked, this, &SettingsWindow::closeSettings);

  // setup panels
  DevicePanel *device = new DevicePanel(this);
  SoftwarePanel *software = new SoftwarePanel(this);
  QObject::connect(device, &DevicePanel::reviewTrainingGuide, this, &SettingsWindow::reviewTrainingGuide);
  QObject::connect(device, &DevicePanel::showDriverView, this, &SettingsWindow::showDriverView);
  QObject::connect(software, &SoftwarePanel::closeSettings, this, &SettingsWindow::closeSettings);

  QList<QPair<QString, QWidget *>> panels = {
    {"Device", device},
    {"Network", network_panel(this)},
    {"Toggles", new TogglesPanel(this)},
    {"Software", software},
    {"UIMenu", new UIPanel(this)},
    {"Driving", new DrivingPanel(this)},
    {"Developer", new DeveloperPanel(this)},
    {"Tuning", new TuningPanel(this)},
  };

  sidebar_layout->addSpacing(45);

#ifdef ENABLE_MAPS
  auto map_panel = new MapPanel(this);
  panels.push_back({"Navigation", map_panel});
  QObject::connect(map_panel, &MapPanel::closeSettings, this, &SettingsWindow::closeSettings);
#endif

  const int padding = panels.size() > 3 ? 0 : 15;

  nav_btns = new QButtonGroup(this);
  for (auto &[name, panel] : panels) {
    QPushButton *btn = new QPushButton(name);
    btn->setCheckable(true);
    btn->setChecked(nav_btns->buttons().size() == 0);
    btn->setStyleSheet(QString(R"(
      QPushButton {
        color: grey;
        border: none;
        background: none;
        font-size: 54px;
        font-weight: 500;
        padding-top: %1px;
        padding-bottom: %1px;
      }
      QPushButton:checked {
        color: white;
      }
      QPushButton:pressed {
        color: #ADADAD;
      }
    )").arg(padding));

    nav_btns->addButton(btn);
    sidebar_layout->addWidget(btn, 0, Qt::AlignRight);

    const int lr_margin = name != "Network" ? 50 : 0;  // Network panel handles its own margins
    panel->setContentsMargins(lr_margin, 25, lr_margin, 25);

    ScrollView *panel_frame = new ScrollView(panel, this);
    panel_widget->addWidget(panel_frame);

    QObject::connect(btn, &QPushButton::clicked, [=, w = panel_frame]() {
      btn->setChecked(true);
      panel_widget->setCurrentWidget(w);
    });
  }
  sidebar_layout->setContentsMargins(50, 50, 100, 50);

  // main settings layout, sidebar + main panel
  QHBoxLayout *main_layout = new QHBoxLayout(this);

  sidebar_widget->setFixedWidth(500);
  main_layout->addWidget(sidebar_widget);
  main_layout->addWidget(panel_widget);

  setStyleSheet(R"(
    * {
      color: white;
      font-size: 50px;
    }
    SettingsWindow {
      background-color: black;
    }
  )");
}

void SettingsWindow::hideEvent(QHideEvent *event) {
#ifdef QCOM
  HardwareEon::close_activities();
#endif
}
