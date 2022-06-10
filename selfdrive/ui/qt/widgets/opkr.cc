#include "selfdrive/ui/qt/widgets/opkr.h"

#include <QHBoxLayout>
#include <QTextStream>
#include <QFile>
#include <QNetworkReply>

#include <QProcess>
#include <QAction>
#include <QMenu>
#include <QDateTime>

#include "selfdrive/common/params.h"
#include "selfdrive/ui/qt/api.h"
#include "selfdrive/ui/qt/widgets/input.h"

#include "selfdrive/ui/ui.h"

SwitchOpenpilot::SwitchOpenpilot() : ButtonControl("Change Repo/Branch", "", "Change to another open pilot code. You can change it by entering ID/repository/branch.") {

  QObject::connect(this, &ButtonControl::clicked, [=]() {
    if (text() == "CHANGE") {
      QString userid = InputDialog::getText("First: Input the Git ID.", this);
      if (userid.length() > 0) {
        getUserID(userid);
        QString repoid = InputDialog::getText("Second: Input the repository.", this);
        if (repoid.length() > 0) {
          getRepoID(repoid);
          QString branchid = InputDialog::getText("Last: Input the branch name.", this);
          if (branchid.length() > 0) {
            getBranchID(branchid);
            githubbranch = branchid;
            QString cmd0 = QString::fromStdString("Change repository/branch. It takes a little time to complete. Do you want to proceed?\n") + QString::fromStdString("https://github.com/") + githubid + QString::fromStdString("/") + githubrepo + QString::fromStdString(".git\n") + QString::fromStdString("Branch: ") + githubbranch;
            if (ConfirmationDialog::confirm(cmd0, this)) {
              setText("DONE");
              setEnabled(true);
              QString time_format = "yyyyMMddHHmmss";
              QDateTime a = QDateTime::currentDateTime();
              QString as = a.toString(time_format);
              QString cmd1 = "mv /data/openpilot /data/openpilot_" + as;
              QString cmd2 = "git clone -b " + githubbranch + " --single-branch https://github.com/" + githubid + "/" + githubrepo + ".git /data/openpilot";
              QProcess::execute("pkill -f thermald");
              QProcess::execute(cmd1);
              QProcess::execute(cmd2);
              QProcess::execute("chmod -R g-rwx /data/openpilot");
              QProcess::execute("chmod -R o-rwx /data/openpilot");
              QProcess::execute("chmod 755 /data/openpilot");
              QProcess::execute("chmod 755 /data/openpilot/cereal");
              QProcess::execute("reboot");
            }
          }
        }
      }
    } else {
      refresh();
    }
  });
  refresh();
}

void SwitchOpenpilot::refresh() {
  setText("CHANGE");
  setEnabled(true);
}

void SwitchOpenpilot::getUserID(const QString &userid) {
  HttpRequest *request = new HttpRequest(this, false);
  QObject::connect(request, &HttpRequest::requestDone, [=](const QString &resp, bool success) {
    if (success) {
      if (!resp.isEmpty()) {
        githubid = userid;
      } else {
        ConfirmationDialog::alert(userid + " The ID does not exist. Return to the input window, press the cancel button, and try again from the beginning.", this);
      }
    } else {
      if (request->timeout()) {
        ConfirmationDialog::alert("The requested time has exceeded.", this);
      } else {
        ConfirmationDialog::alert("The ID does not exist. Return to the input window, press the cancel button, and try again from the beginning.", this);
      }
    }

    refresh();
    request->deleteLater();
  });
  request->sendRequest("https://github.com/" + userid);
}

void SwitchOpenpilot::getRepoID(const QString &repoid) {
  HttpRequest *request = new HttpRequest(this, false);
  QObject::connect(request, &HttpRequest::requestDone, [=](const QString &resp, bool success) {
    if (success) {
      if (!resp.isEmpty()) {
        githubrepo = repoid;
      } else {
        ConfirmationDialog::alert(repoid + " The repository does not exist. Return to the input window, press the cancel button, and try again from the beginning.", this);
      }
    } else {
      if (request->timeout()) {
        ConfirmationDialog::alert("The requested time has exceeded.", this);
      } else {
        ConfirmationDialog::alert("The Repository does not exist. Return to the input window, press the cancel button, and try again from the beginning.", this);
      }
    }

    refresh();
    request->deleteLater();
  });
  request->sendRequest("https://github.com/" + githubid + "/" + repoid);
}

void SwitchOpenpilot::getBranchID(const QString &branchid) {
  HttpRequest *request = new HttpRequest(this, false);
  QObject::connect(request, &HttpRequest::requestDone, [=](const QString &resp, bool success) {
    if (success) {
      if (!resp.isEmpty()) {
        githubbranch = branchid;
      } else {
        ConfirmationDialog::alert(branchid + " The branch does not exist. Press the cancel button and try again from the beginning.", this);
      }
    } else {
      if (request->timeout()) {
        ConfirmationDialog::alert("The requested time has exceeded.", this);
      } else {
        ConfirmationDialog::alert("The Branch does not exist. Return to the input window, press the cancel button, and try again from the beginning.", this);
      }
    }

    refresh();
    request->deleteLater();
  });
  request->sendRequest("https://github.com/" + githubid + "/" + githubrepo + "/tree/" + branchid);
}

GitHash::GitHash() : AbstractControl("Commit (Local/Remote)", "", "") {

  QString lhash = QString::fromStdString(params.get("GitCommit").substr(0, 10));
  QString rhash = QString::fromStdString(params.get("GitCommitRemote").substr(0, 10));
  hlayout->addStretch(1);
  
  local_hash.setText(QString::fromStdString(params.get("GitCommit").substr(0, 10)));
  remote_hash.setText(QString::fromStdString(params.get("GitCommitRemote").substr(0, 10)));
  //local_hash.setAlignment(Qt::AlignVCenter);
  remote_hash.setAlignment(Qt::AlignVCenter);
  local_hash.setStyleSheet("color: #aaaaaa");
  if (lhash == rhash) {
    remote_hash.setStyleSheet("color: #aaaaaa");
  } else {
    remote_hash.setStyleSheet("color: #0099ff");
  }
  hlayout->addWidget(&local_hash);
  hlayout->addWidget(&remote_hash);
}

OpenpilotView::OpenpilotView() : AbstractControl("Driving Camera", "Preview the open pilot driving screen.", "") {

  // setup widget
  hlayout->addStretch(1);

  btn.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");

  btn.setFixedSize(250, 100);
  hlayout->addWidget(&btn);

  QObject::connect(&btn, &QPushButton::clicked, [=]() {
    bool stat = params.getBool("IsOpenpilotViewEnabled");
    if (stat) {
      params.putBool("IsOpenpilotViewEnabled", false);
    } else {
      params.putBool("IsOpenpilotViewEnabled", true);
    }
    refresh();
  });
  refresh();
}

void OpenpilotView::refresh() {
  bool param = params.getBool("IsOpenpilotViewEnabled");
  QString car_param = QString::fromStdString(params.get("CarParams"));
  if (param) {
    btn.setText("UNVIEW");
  } else {
    btn.setText("PREVIEW");
  }
  if (car_param.length()) {
    btn.setEnabled(false);
  } else {
    btn.setEnabled(true);
  }
}

CarSelectCombo::CarSelectCombo() : AbstractControl("", "", "") 
{
  combobox.setStyleSheet(R"(
    subcontrol-origin: padding;
    subcontrol-position: top left;
    selection-background-color: #111;
    selection-color: yellow;
    color: white;
    background-color: #393939;
    border-style: solid;
    border: 0px solid #1e1e1e;
    border-radius: 0;
    width: 100px;
  )");

  combobox.addItem("Select Your Car");
  QFile carlistfile("/data/params/d/CarList");
  if (carlistfile.open(QIODevice::ReadOnly)) {
    QTextStream carname(&carlistfile);
    while (!carname.atEnd()) {
      QString line = carname.readLine();
      combobox.addItem(line);
    }
    carlistfile.close();
  }

  combobox.setFixedWidth(1055);

  btn.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");

  btn.setFixedSize(150, 100);

  QObject::connect(&btn, &QPushButton::clicked, [=]() {
    if (btn.text() == "UNSET") {
      if (ConfirmationDialog::confirm("Do you want to unset?", this)) {
        params.remove("CarModel");
        combobox.setCurrentIndex(0);
        refresh();
      }
    }
  });

  //combobox.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

  hlayout->addWidget(&combobox);
  hlayout->addWidget(&btn, Qt::AlignRight);

  QObject::connect(&combobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [=](int index)
  {
    combobox.itemData(combobox.currentIndex());
    QString str = combobox.currentText();
    if (combobox.currentIndex() != 0) {
      if (ConfirmationDialog::confirm("Press OK to set your car as\n" + str, this)) {
        params.put("CarModel", str.toStdString());
      }
    }
    refresh();
  });
  refresh();
}

void CarSelectCombo::refresh() {
  QString selected_carname = QString::fromStdString(params.get("CarModel"));
  int index = combobox.findText(selected_carname);
  if (index >= 0) combobox.setCurrentIndex(index);
  if (selected_carname.length()) {
    btn.setEnabled(true);
    btn.setText("UNSET");
  } else {
    btn.setEnabled(false);
    btn.setText("SET");
  }
}

TimeZoneSelectCombo::TimeZoneSelectCombo() : AbstractControl("", "", "") 
{
  combobox.setStyleSheet(R"(
    subcontrol-origin: padding;
    subcontrol-position: top left;
    selection-background-color: #111;
    selection-color: yellow;
    color: white;
    background-color: #393939;
    border-style: solid;
    border: 0px solid #1e1e1e;
    border-radius: 0;
    width: 100px;
  )");

  combobox.addItem("Select Your TimeZone");
  QFile timezonelistfile("/data/openpilot/selfdrive/assets/addon/param/TimeZone");
  if (timezonelistfile.open(QIODevice::ReadOnly)) {
    QTextStream timezonename(&timezonelistfile);
    while (!timezonename.atEnd()) {
      QString line = timezonename.readLine();
      combobox.addItem(line);
    }
    timezonelistfile.close();
  }

  combobox.setFixedWidth(1055);

  btn.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");

  btn.setFixedSize(150, 100);

  QObject::connect(&btn, &QPushButton::clicked, [=]() {
    if (btn.text() == "UNSET") {
      if (ConfirmationDialog::confirm("Do you want to set default?", this)) {
        params.put("OPKRTimeZone", "UTC");
        combobox.setCurrentIndex(0);
        refresh();
      }
    }
  });

  //combobox.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

  hlayout->addWidget(&combobox);
  hlayout->addWidget(&btn, Qt::AlignRight);

  QObject::connect(&combobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [=](int index)
  {
    combobox.itemData(combobox.currentIndex());
    QString str = combobox.currentText();
    if (combobox.currentIndex() != 0) {
      if (ConfirmationDialog::confirm("Press OK to set your timezone as\n" + str, this)) {
        params.put("OPKRTimeZone", str.toStdString());
      }
    }
    refresh();
  });
  refresh();
}

void TimeZoneSelectCombo::refresh() {
  QString selected_timezonename = QString::fromStdString(params.get("OPKRTimeZone"));
  int index = combobox.findText(selected_timezonename);
  if (index >= 0) combobox.setCurrentIndex(index);
  if (selected_timezonename.length()) {
    btn.setEnabled(true);
    btn.setText("UNSET");
  } else {
    btn.setEnabled(false);
    btn.setText("SET");
  }
}

//UI
AutoShutdown::AutoShutdown() : AbstractControl("EON AutoShutdown", "EON is automatically turned off after the set time while the engine is turned off (offload) after driving (onload).", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrAutoShutdown"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 0 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("OpkrAutoShutdown", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrAutoShutdown"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 10 ) {
      value = 10;
    }
    QString values = QString::number(value);
    params.put("OpkrAutoShutdown", values.toStdString());
    refresh();
  });
  refresh();
}

void AutoShutdown::refresh() {
  QString option = QString::fromStdString(params.get("OpkrAutoShutdown"));
  if (option == "0") {
    label.setText(QString::fromStdString("AlwaysOn"));
  } else if (option == "1") {
    label.setText(QString::fromStdString("RightOff"));
  } else if (option == "2") {
    label.setText(QString::fromStdString("30sec"));
  } else if (option == "3") {
    label.setText(QString::fromStdString("1min"));
  } else if (option == "4") {
    label.setText(QString::fromStdString("3mins"));
  } else if (option == "5") {
    label.setText(QString::fromStdString("5mins"));
  } else if (option == "6") {
    label.setText(QString::fromStdString("10mins"));
  } else if (option == "7") {
    label.setText(QString::fromStdString("30mins"));
  } else if (option == "8") {
    label.setText(QString::fromStdString("1hour"));
  } else if (option == "9") {
    label.setText(QString::fromStdString("3hours"));
  } else if (option == "10") {
    label.setText(QString::fromStdString("5hours"));
  }
  btnminus.setText("－");
  btnplus.setText("＋");
}

ForceShutdown::ForceShutdown() : AbstractControl("EON ForceShutdown", "If the screen is turned off while off-road without driving (on-road X), force it to turn off after a certain period of time. When a touch event occurs, the off time is reset.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrForceShutdown"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 0 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("OpkrForceShutdown", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrForceShutdown"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 5 ) {
      value = 5;
    }
    QString values = QString::number(value);
    params.put("OpkrForceShutdown", values.toStdString());
    refresh();
  });
  refresh();
}

void ForceShutdown::refresh() {
  QString option = QString::fromStdString(params.get("OpkrForceShutdown"));
  if (option == "0") {
    label.setText(QString::fromStdString("AlwaysOn"));
  } else if (option == "1") {
    label.setText(QString::fromStdString("1min"));
  } else if (option == "2") {
    label.setText(QString::fromStdString("3mins"));
  } else if (option == "3") {
    label.setText(QString::fromStdString("5mins"));
  } else if (option == "4") {
    label.setText(QString::fromStdString("10mins"));
  } else if (option == "5") {
    label.setText(QString::fromStdString("30mins"));
  }
  btnminus.setText("－");
  btnplus.setText("＋");
}


VolumeControl::VolumeControl() : AbstractControl("EON Volume Control(%)", "Adjust the volume of EON. Android Default/Manual Settings", "../assets/offroad/icon_shell.png") {

  effect.setSource(QUrl::fromLocalFile("/data/openpilot/selfdrive/assets/addon/sound/ding.wav"));
  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrUIVolumeBoost"));
    int value = str.toInt();
    value = value - 5;
    if (value <= -5 ) {
      value = -5;
    }
    QString values = QString::number(value);
    QUIState::ui_state.scene.nVolumeBoost = value;
    params.put("OpkrUIVolumeBoost", values.toStdString());
    playsound();
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrUIVolumeBoost"));
    int value = str.toInt();
    value = value + 5;
    if (value >= 100 ) {
      value = 100;
    }
    QString values = QString::number(value);
    QUIState::ui_state.scene.nVolumeBoost = value;
    params.put("OpkrUIVolumeBoost", values.toStdString());
    playsound();
    refresh();
  });
  refresh();
}

void VolumeControl::refresh() {
  QString option = QString::fromStdString(params.get("OpkrUIVolumeBoost"));
  if (option == "0") {
    label.setText(QString::fromStdString("Default"));
  } else if (option == "-5") {
    label.setText(QString::fromStdString("Mute"));
  } else {
    label.setText(QString::fromStdString(params.get("OpkrUIVolumeBoost")));
  }
  btnminus.setText("－");
  btnplus.setText("＋");
}

void VolumeControl::playsound() {
  float value = QUIState::ui_state.scene.nVolumeBoost;
  if (value > 1 ) {
    effect.setVolume(value * 0.01);
    effect.play();
  } else if (value >= 0) {
    effect.setVolume(0.5);
    effect.play();
  }
}

BrightnessControl::BrightnessControl() : AbstractControl("EON Brightness Control(%)", "Manually adjust the brightness of the EON screen.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrUIBrightness"));
    int value = str.toInt();
    value = value - 5;
    if (value <= 0 ) {
      value = 0;
    }
    QUIState::ui_state.scene.brightness = value;
    QString values = QString::number(value);
    params.put("OpkrUIBrightness", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrUIBrightness"));
    int value = str.toInt();
    value = value + 5;
    if (value >= 100 ) {
      value = 100;
    }
    QUIState::ui_state.scene.brightness = value;
    QString values = QString::number(value);
    params.put("OpkrUIBrightness", values.toStdString());
    refresh();
  });
  refresh();
}

void BrightnessControl::refresh() {
  QString option = QString::fromStdString(params.get("OpkrUIBrightness"));
  if (option == "0") {
    label.setText(QString::fromStdString("Auto"));
  } else {
    label.setText(QString::fromStdString(params.get("OpkrUIBrightness")));
  }
  btnminus.setText("－");
  btnplus.setText("＋");
}

BrightnessOffControl::BrightnessOffControl() : AbstractControl("Brightness at SCR Off(%)", "When using the EON screen off function, the brightness is reduced according to the automatic brightness ratio.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrUIBrightnessOff"));
    int value = str.toInt();
    value = value - 10;
    if (value <= 0 ) {
      value = 0;
    }
    QUIState::ui_state.scene.brightness_off = value;
    QString values = QString::number(value);
    params.put("OpkrUIBrightnessOff", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrUIBrightnessOff"));
    int value = str.toInt();
    value = value + 10;
    if (value >= 100 ) {
      value = 100;
    }
    QUIState::ui_state.scene.brightness_off = value;
    QString values = QString::number(value);
    params.put("OpkrUIBrightnessOff", values.toStdString());
    refresh();
  });
  refresh();
}

void BrightnessOffControl::refresh() {
  QString option = QString::fromStdString(params.get("OpkrUIBrightnessOff"));
  if (option == "0") {
    label.setText(QString::fromStdString("Dark"));
  } else {
    label.setText(QString::fromStdString(params.get("OpkrUIBrightnessOff")));
  }
  btnminus.setText("－");
  btnplus.setText("＋");
}

AutoScreenOff::AutoScreenOff() : AbstractControl("EON SCR Off Timer", "Turn off the EON screen or reduce brightness to protect the screen after driving starts. It automatically brightens or turns on when a touch or event occurs.", "../assets/offroad/icon_shell.png") 
{

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrAutoScreenOff"));
    int value = str.toInt();
    value = value - 1;
    if (value <= -2 ) {
      value = -2;
    }
    QUIState::ui_state.scene.autoScreenOff = value;
    QString values = QString::number(value);
    params.put("OpkrAutoScreenOff", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrAutoScreenOff"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 10 ) {
      value = 10;
    }
    QUIState::ui_state.scene.autoScreenOff = value;
    QString values = QString::number(value);
    params.put("OpkrAutoScreenOff", values.toStdString());
    refresh();
  });
  refresh();
}

void AutoScreenOff::refresh() 
{
  QString option = QString::fromStdString(params.get("OpkrAutoScreenOff"));
  if (option == "-2") {
    label.setText(QString::fromStdString("AlwaysOn"));
  } else if (option == "-1") {
    label.setText(QString::fromStdString("15secs"));
  } else if (option == "0") {
    label.setText(QString::fromStdString("30secs"));
  } else {
    label.setText(QString::fromStdString(params.get("OpkrAutoScreenOff")) + "min(s)");
  }
  btnminus.setText("－");
  btnplus.setText("＋");
}

ChargingMin::ChargingMin() : AbstractControl("BAT MinCharging Value", "Sets the minimum battery charge value.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrBatteryChargingMin"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 10 ) {
      value = 10;
    }
    QString values = QString::number(value);
    params.put("OpkrBatteryChargingMin", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrBatteryChargingMin"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 90 ) {
      value = 90;
    }
    QString values = QString::number(value);
    params.put("OpkrBatteryChargingMin", values.toStdString());
    refresh();
  });
  refresh();
}

void ChargingMin::refresh() {
  label.setText(QString::fromStdString(params.get("OpkrBatteryChargingMin")));
  btnminus.setText("－");
  btnplus.setText("＋");
}

ChargingMax::ChargingMax() : AbstractControl("BAT MaxCharging Value", "Sets the maximum battery charge value.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrBatteryChargingMax"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 10 ) {
      value = 10;
    }
    QString values = QString::number(value);
    params.put("OpkrBatteryChargingMax", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrBatteryChargingMax"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 90 ) {
      value = 90;
    }
    QString values = QString::number(value);
    params.put("OpkrBatteryChargingMax", values.toStdString());
    refresh();
  });
  refresh();
}

void ChargingMax::refresh() {
  label.setText(QString::fromStdString(params.get("OpkrBatteryChargingMax")));
  btnminus.setText("－");
  btnplus.setText("＋");
}

RecordCount::RecordCount() : AbstractControl("Number of Recorded Files", "Sets the maximum number of recording files.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("RecordingCount"));
    int value = str.toInt();
    value = value - 5;
    if (value <= 5 ) {
      value = 5;
    }
    QString values = QString::number(value);
    params.put("RecordingCount", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("RecordingCount"));
    int value = str.toInt();
    value = value + 5;
    if (value >= 300 ) {
      value = 300;
    }
    QString values = QString::number(value);
    params.put("RecordingCount", values.toStdString());
    refresh();
  });
  refresh();
}

void RecordCount::refresh() {
  label.setText(QString::fromStdString(params.get("RecordingCount")));
  btnminus.setText("－");
  btnplus.setText("＋");
}

RecordQuality::RecordQuality() : AbstractControl("Recording Quality", "Set the recording quality. Low/Mid/high definition/high definition/ultra-high definition.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("RecordingQuality"));
    int value = str.toInt();
    value = value - 1;
    if (value <= -1 ) {
      value = 3;
    }
    QString values = QString::number(value);
    params.put("RecordingQuality", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("RecordingQuality"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 4 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("RecordingQuality", values.toStdString());
    refresh();
  });
  refresh();
}

void RecordQuality::refresh() {
  QString option = QString::fromStdString(params.get("RecordingQuality"));
  if (option == "0") {
    label.setText(QString::fromStdString("Low"));
  } else if (option == "1") {
    label.setText(QString::fromStdString("Mid"));
  } else if (option == "2") {
    label.setText(QString::fromStdString("High"));
  } else {
    label.setText(QString::fromStdString("U-High"));
  }
  btnminus.setText("◀");
  btnplus.setText("▶");
}

MonitoringMode::MonitoringMode() : AbstractControl("Driver Monitoring Mode", "Set the monitoring mode. In the case of preference/prevention of sleepiness and sleepiness prevention, you can send a warning message faster by adjusting (lowering) the threshold value below.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrMonitoringMode"));
    int value = str.toInt();
    value = value - 1;
    if (value <= -1 ) {
      value = 1;
    }
    QString values = QString::number(value);
    params.put("OpkrMonitoringMode", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrMonitoringMode"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 2 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("OpkrMonitoringMode", values.toStdString());
    refresh();
  });
  refresh();
}

void MonitoringMode::refresh() {
  QString option = QString::fromStdString(params.get("OpkrMonitoringMode"));
  if (option == "0") {
    label.setText(QString::fromStdString("Default"));
  } else if (option == "1") {
    label.setText(QString::fromStdString("UnSleep"));
  }
  btnminus.setText("◀");
  btnplus.setText("▶");
}

MonitorEyesThreshold::MonitorEyesThreshold() : AbstractControl("E2E EYE Threshold", "Adjust the reference value for the eye detection range. Set the reference value for the value that suits you. When you close your eyes, you should set it lower than the distracted Eyes value. Default: 0.75", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrMonitorEyesThreshold"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 1 ) {
      value = 1;
    }
    QString values = QString::number(value);
    params.put("OpkrMonitorEyesThreshold", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrMonitorEyesThreshold"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 100 ) {
      value = 100;
    }
    QString values = QString::number(value);
    params.put("OpkrMonitorEyesThreshold", values.toStdString());
    refresh();
  });
  refresh();
}

void MonitorEyesThreshold::refresh() {
  auto strs = QString::fromStdString(params.get("OpkrMonitorEyesThreshold"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.01;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
  btnminus.setText("－");
  btnplus.setText("＋");
}

NormalEyesThreshold::NormalEyesThreshold() : AbstractControl("Normal EYE Threshold", "Adjust the eye recognition reference value. Lower the value when the recognition rate is low. Default: 0.5", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrMonitorNormalEyesThreshold"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 1 ) {
      value = 1;
    }
    QString values = QString::number(value);
    params.put("OpkrMonitorNormalEyesThreshold", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrMonitorNormalEyesThreshold"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 100 ) {
      value = 100;
    }
    QString values = QString::number(value);
    params.put("OpkrMonitorNormalEyesThreshold", values.toStdString());
    refresh();
  });
  refresh();
}

void NormalEyesThreshold::refresh() {
  auto strs = QString::fromStdString(params.get("OpkrMonitorNormalEyesThreshold"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.01;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
  btnminus.setText("－");
  btnplus.setText("＋");
}

BlinkThreshold::BlinkThreshold() : AbstractControl("Blink Threshold", "Adjust the recognition value for the degree of blinking. When you close your eyes, check BlinkProb and lower the value. Default: 0.5", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrMonitorBlinkThreshold"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 1 ) {
      value = 1;
    }
    QString values = QString::number(value);
    params.put("OpkrMonitorBlinkThreshold", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrMonitorBlinkThreshold"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 100 ) {
      value = 100;
    }
    QString values = QString::number(value);
    params.put("OpkrMonitorBlinkThreshold", values.toStdString());
    refresh();
  });
  refresh();
}

void BlinkThreshold::refresh() {
  auto strs = QString::fromStdString(params.get("OpkrMonitorBlinkThreshold"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.01;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
  btnminus.setText("－");
  btnplus.setText("＋");
}

//Driving
CruisemodeSelInit::CruisemodeSelInit() : AbstractControl("Cruise Start Mode", "Set the cruise start mode. OP mode/dist+curve/dist only/curve only/one-way 1 lane/safetycam deceleration Only. op mode:no button speed control, dist+curve:use button speed control in the inter-vehicle distance and curve section, dist only:curve only:curve one-way speed only, one-way speed control", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("CruiseStatemodeSelInit"));
    int value = str.toInt();
    value = value - 1;
    if (value <= -1 ) {
      value = 5;
    }
    QString values = QString::number(value);
    params.put("CruiseStatemodeSelInit", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("CruiseStatemodeSelInit"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 6 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("CruiseStatemodeSelInit", values.toStdString());
    refresh();
  });
  refresh();
}

void CruisemodeSelInit::refresh() {
  QString option = QString::fromStdString(params.get("CruiseStatemodeSelInit"));
  if (option == "0") {
    label.setText(QString::fromStdString("OP Stock"));
  } else if (option == "1") {
    label.setText(QString::fromStdString("Dist+Curv"));
  } else if (option == "2") {
    label.setText(QString::fromStdString("DistOnly"));
  } else if (option == "3") {
    label.setText(QString::fromStdString("CurvOnly"));
  } else if (option == "4") {
    label.setText(QString::fromStdString("OneWay"));
  } else {
    label.setText(QString::fromStdString("CamOnly"));
  }
  btnminus.setText("◀");
  btnplus.setText("▶");
}

LaneChangeSpeed::LaneChangeSpeed() : AbstractControl("LaneChange Speed", "Set the lane changeable speed.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrLaneChangeSpeed"));
    int value = str.toInt();
    value = value - 5;
    if (value <= 20 ) {
      value = 20;
    }
    QString values = QString::number(value);
    params.put("OpkrLaneChangeSpeed", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrLaneChangeSpeed"));
    int value = str.toInt();
    value = value + 5;
    if (value >= 160 ) {
      value = 160;
    }
    QString values = QString::number(value);
    params.put("OpkrLaneChangeSpeed", values.toStdString());
    refresh();
  });
  refresh();
}

void LaneChangeSpeed::refresh() {
  label.setText(QString::fromStdString(params.get("OpkrLaneChangeSpeed")));
  btnminus.setText("－");
  btnplus.setText("＋");
}

LaneChangeDelay::LaneChangeDelay() : AbstractControl("LaneChange Delay", "Set the delay time after turn signal operation before lane change.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrAutoLaneChangeDelay"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 0 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("OpkrAutoLaneChangeDelay", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrAutoLaneChangeDelay"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 5 ) {
      value = 5;
    }
    QString values = QString::number(value);
    params.put("OpkrAutoLaneChangeDelay", values.toStdString());
    refresh();
  });
  refresh();
}

void LaneChangeDelay::refresh() {
  QString option = QString::fromStdString(params.get("OpkrAutoLaneChangeDelay"));
  if (option == "0") {
    label.setText(QString::fromStdString("Nudge"));
  } else if (option == "1") {
    label.setText(QString::fromStdString("RightNow"));
  } else if (option == "2") {
    label.setText(QString::fromStdString("0.5sec"));
  } else if (option == "3") {
    label.setText(QString::fromStdString("1sec"));
  } else if (option == "4") {
    label.setText(QString::fromStdString("1.5sec"));
  } else {
    label.setText(QString::fromStdString("2secs"));
  }
  btnminus.setText("－");
  btnplus.setText("＋");
}

LeftCurvOffset::LeftCurvOffset() : AbstractControl("LeftCurv Offset", "Adjust the position of the vehicle in the curve section. (-value: move the car to the left, +value: move the car to the right)", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("LeftCurvOffsetAdj"));
    int value = str.toInt();
    value = value - 1;
    if (value <= -50 ) {
      value = -50;
    }
    QString values = QString::number(value);
    params.put("LeftCurvOffsetAdj", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("LeftCurvOffsetAdj"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 50 ) {
      value = 50;
    }
    QString values = QString::number(value);
    params.put("LeftCurvOffsetAdj", values.toStdString());
    refresh();
  });
  refresh();
}

void LeftCurvOffset::refresh() {
  label.setText(QString::fromStdString(params.get("LeftCurvOffsetAdj")));
  btnminus.setText("－");
  btnplus.setText("＋");
}

RightCurvOffset::RightCurvOffset() : AbstractControl("RightCurv Offset", "Adjust the position of the vehicle in the curve section. (-value: move the car to the left, +value: move the car to the right)", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("RightCurvOffsetAdj"));
    int value = str.toInt();
    value = value - 1;
    if (value <= -50 ) {
      value = -50;
    }
    QString values = QString::number(value);
    params.put("RightCurvOffsetAdj", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("RightCurvOffsetAdj"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 50 ) {
      value = 50;
    }
    QString values = QString::number(value);
    params.put("RightCurvOffsetAdj", values.toStdString());
    refresh();
  });
  refresh();
}

void RightCurvOffset::refresh() {
  label.setText(QString::fromStdString(params.get("RightCurvOffsetAdj")));
  btnminus.setText("－");
  btnplus.setText("＋");
}

MaxAngleLimit::MaxAngleLimit() : AbstractControl("Max Steering Angle", "Set the maximum steering angle of the handle where the openpilot is possible. Please note that some vehicles may experience errors if the angle is set above 90 degrees.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrMaxAngleLimit"));
    int value = str.toInt();
    value = value - 10;
    if (value <= 80 ) {
      value = 80;
    }
    QString values = QString::number(value);
    params.put("OpkrMaxAngleLimit", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrMaxAngleLimit"));
    int value = str.toInt();
    value = value + 10;
    if (value >= 360 ) {
      value = 360;
    }
    QString values = QString::number(value);
    params.put("OpkrMaxAngleLimit", values.toStdString());
    refresh();
  });
  refresh();
}

void MaxAngleLimit::refresh() {
  QString option = QString::fromStdString(params.get("OpkrMaxAngleLimit"));
  if (option == "80") {
    label.setText(QString::fromStdString("NoLimit"));
  } else {
    label.setText(QString::fromStdString(params.get("OpkrMaxAngleLimit")));
  }
  btnminus.setText("－");
  btnplus.setText("＋");
}

SteerAngleCorrection::SteerAngleCorrection() : AbstractControl("Str Angle Adjust", "On the straight path, adjust the SteerAngle zero to zero the current steering angle. ex) Set it to 0.5 degrees Celsius for a straight line, and -0.5 degrees Celsius for -0.5 degrees Celsius.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrSteerAngleCorrection"));
    int value = str.toInt();
    value = value - 1;
    if (value <= -50 ) {
      value = -50;
    }
    QString values = QString::number(value);
    params.put("OpkrSteerAngleCorrection", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrSteerAngleCorrection"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 50 ) {
      value = 50;
    }
    QString values = QString::number(value);
    params.put("OpkrSteerAngleCorrection", values.toStdString());
    refresh();
  });
  refresh();
}

void SteerAngleCorrection::refresh() {
  auto strs = QString::fromStdString(params.get("OpkrSteerAngleCorrection"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.1;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
  btnminus.setText("－");
  btnplus.setText("＋");
}

SpeedLimitOffset::SpeedLimitOffset() : AbstractControl("SpeedLimit Offset", "During safetycam deceleration, it decelerates by compensating for the difference between GPS speed and real speed.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btn.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btn.setFixedSize(110, 100);
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btn);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btn, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrSpeedLimitOffsetOption"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 2 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("OpkrSpeedLimitOffsetOption", values.toStdString());
    refresh();
  });

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrSpeedLimitOffset"));
    int value = str.toInt();
    value = value - 1;
    if (value <= -30 ) {
      value = -30;
    }
    QString values = QString::number(value);
    //QUIState::ui_state.speed_lim_off = value;
    params.put("OpkrSpeedLimitOffset", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrSpeedLimitOffset"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 30 ) {
      value = 30;
    }
    QString values = QString::number(value);
    //QUIState::ui_state.speed_lim_off = value;
    params.put("OpkrSpeedLimitOffset", values.toStdString());
    refresh();
  });
  refresh();
}

void SpeedLimitOffset::refresh() {
  auto strs = QString::fromStdString(params.get("OpkrSpeedLimitOffsetOption"));
  if (strs == "0") {
    btn.setText("%");
  } else {
    btn.setText("±");
  }
  label.setText(QString::fromStdString(params.get("OpkrSpeedLimitOffset")));
  btnminus.setText("－");
  btnplus.setText("＋");
}

RESChoice::RESChoice() : AbstractControl("AutoRES Option", "Sets the auto RES option. 1. Adjust the temporary cruise speed, 2. Adjust the set speed itself according to the presence or absence of a preceding car. 3. Adjust the cruise speed if there is a preceding car, and adjust the set speed if there is no preceding car. Please note that the automatic RES may not work well depending on the conditions.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("AutoResOption"));
    int value = str.toInt();
    value = value - 1;
    if (value <= -1 ) {
      value = 2;
    }
    QString values = QString::number(value);
    params.put("AutoResOption", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("AutoResOption"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 3 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("AutoResOption", values.toStdString());
    refresh();
  });
  refresh();
}

void RESChoice::refresh() {
  QString option = QString::fromStdString(params.get("AutoResOption"));
  if (option == "0") {
    label.setText(QString::fromStdString("CruiseSet"));
  } else if (option == "1") {
    label.setText(QString::fromStdString("MaxSpeedSet"));
  } else {
    label.setText(QString::fromStdString("AUTO(LeadCar)"));
  }
  btnminus.setText("◀");
  btnplus.setText("▶");
}

AutoResCondition::AutoResCondition() : AbstractControl("AutoRES Condition", "Sets the automatic RES condition. When the brake is released/operated when the accelerator pedal is operated.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("AutoResCondition"));
    int value = str.toInt();
    value = value - 1;
    if (value <= -1 ) {
      value = 1;
    }
    QString values = QString::number(value);
    params.put("AutoResCondition", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("AutoResCondition"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 2 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("AutoResCondition", values.toStdString());
    refresh();
  });
  refresh();
}

void AutoResCondition::refresh() {
  QString option = QString::fromStdString(params.get("AutoResCondition"));
  if (option == "0") {
    label.setText(QString::fromStdString("RelBrake"));
  } else {
    label.setText(QString::fromStdString("OnGas"));
  }
  btnminus.setText("◀");
  btnplus.setText("▶");
}

AutoResLimitTime::AutoResLimitTime() : AbstractControl("AutoRES Allow(sec)", "Adjust the automatic RES allowance time. Automatic RES operates only within the set time after the cruise is released.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("AutoResLimitTime"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 0 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("AutoResLimitTime", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("AutoResLimitTime"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 60 ) {
      value = 60;
    }
    QString values = QString::number(value);
    params.put("AutoResLimitTime", values.toStdString());
    refresh();
  });
  refresh();
}

void AutoResLimitTime::refresh() {
  QString option = QString::fromStdString(params.get("AutoResLimitTime"));
  if (option == "0") {
    label.setText(QString::fromStdString("NoLimit"));
  } else {
    label.setText(QString::fromStdString(params.get("AutoResLimitTime")));
  }
  btnminus.setText("-");
  btnplus.setText("+");
}

AutoEnableSpeed::AutoEnableSpeed() : AbstractControl("Auto Engage Speed(km/h)", "Set the automatic engage speed.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("AutoEnableSpeed"));
    int value = str.toInt();
    value = value - 3;
    if (value <= 0 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("AutoEnableSpeed", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("AutoEnableSpeed"));
    int value = str.toInt();
    value = value + 3;
    if (value >= 30 ) {
      value = 30;
    }
    QString values = QString::number(value);
    params.put("AutoEnableSpeed", values.toStdString());
    refresh();
  });
  refresh();
}

void AutoEnableSpeed::refresh() {
  QString option = QString::fromStdString(params.get("AutoEnableSpeed"));
  if (option == "0") {
    label.setText(QString::fromStdString("atDepart"));
  } else {
    label.setText(QString::fromStdString(params.get("AutoEnableSpeed")));
  }
  btnminus.setText("-");
  btnplus.setText("+");
}

CamDecelDistAdd::CamDecelDistAdd() : AbstractControl("SafetyCamDist Adj(%)", "Reduce or increase the deceleration start distance during deceleration of the safety section (+ value: deceleration start from a long distance, -value: deceleration start at a short distance) = interpolation value X interpolation value X reduction/increase ratio according to difference between current speed and cam speed.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SafetyCamDecelDistGain"));
    int value = str.toInt();
    value = value - 5;
    if (value <= -100 ) {
      value = -100;
    }
    QString values = QString::number(value);
    params.put("SafetyCamDecelDistGain", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SafetyCamDecelDistGain"));
    int value = str.toInt();
    value = value + 5;
    if (value >= 100 ) {
      value = 100;
    }
    QString values = QString::number(value);
    params.put("SafetyCamDecelDistGain", values.toStdString());
    refresh();
  });
  refresh();
}

void CamDecelDistAdd::refresh() {
  QString option = QString::fromStdString(params.get("SafetyCamDecelDistGain"));
  if (option == "0") {
    label.setText(QString::fromStdString("Default"));
  } else {
    label.setText(QString::fromStdString(params.get("SafetyCamDecelDistGain")));
  }
  btnminus.setText("-");
  btnplus.setText("+");
}

//Panda
MaxSteer::MaxSteer() : AbstractControl("MAX_STEER", "Modify the Panda MAX_STEEER value. Press the Run button below to apply.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("MaxSteer"));
    int value = str.toInt();
    value = value - 2;
    if (value <= 384 ) {
      value = 384;
    }
    QString values = QString::number(value);
    params.put("MaxSteer", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("MaxSteer"));
    int value = str.toInt();
    value = value + 2;
    if (value >= 1024 ) {
      value = 1024;
    }
    QString values = QString::number(value);
    params.put("MaxSteer", values.toStdString());
    refresh();
  });
  refresh();
}

void MaxSteer::refresh() {
  label.setText(QString::fromStdString(params.get("MaxSteer")));
  btnminus.setText("－");
  btnplus.setText("＋");
}

MaxRTDelta::MaxRTDelta() : AbstractControl("RT_DELTA", "Modify the panda RT_DELTA value. Press the Run button below to apply.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("MaxRTDelta"));
    int value = str.toInt();
    value = value - 2;
    if (value <= 50 ) {
      value = 50;
    }
    QString values = QString::number(value);
    params.put("MaxRTDelta", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("MaxRTDelta"));
    int value = str.toInt();
    value = value + 2;
    if (value >= 500 ) {
      value = 500;
    }
    QString values = QString::number(value);
    params.put("MaxRTDelta", values.toStdString());
    refresh();
  });
  refresh();
}

void MaxRTDelta::refresh() {
  label.setText(QString::fromStdString(params.get("MaxRTDelta")));
  btnminus.setText("－");
  btnplus.setText("＋");
}

MaxRateUp::MaxRateUp() : AbstractControl("MAX_RATE_UP", "Modify the Panda MAX_RATE_UP value. Press the Run button below to apply.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("MaxRateUp"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 3 ) {
      value = 3;
    }
    QString values = QString::number(value);
    params.put("MaxRateUp", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("MaxRateUp"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 7 ) {
      value = 7;
    }
    QString values = QString::number(value);
    params.put("MaxRateUp", values.toStdString());
    refresh();
  });
  refresh();
}

void MaxRateUp::refresh() {
  label.setText(QString::fromStdString(params.get("MaxRateUp")));
  btnminus.setText("－");
  btnplus.setText("＋");
}

MaxRateDown::MaxRateDown() : AbstractControl("MAX_RATE_DOWN", "Modify the Panda MAX_RATE_DOWN value. Press the Run button below to apply.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("MaxRateDown"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 7 ) {
      value = 7;
    }
    QString values = QString::number(value);
    params.put("MaxRateDown", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("MaxRateDown"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 15 ) {
      value = 15;
    }
    QString values = QString::number(value);
    params.put("MaxRateDown", values.toStdString());
    refresh();
  });
  refresh();
}

void MaxRateDown::refresh() {
  label.setText(QString::fromStdString(params.get("MaxRateDown")));
  btnminus.setText("－");
  btnplus.setText("＋");
}

//튜닝
CameraOffset::CameraOffset() : AbstractControl("CameraOffset", "Sets the CameraOffset value. (+value:Move Left, -value:Move Right)", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("CameraOffsetAdj"));
    int value = str.toInt();
    value = value - 5;
    if (value <= -1000 ) {
      value = -1000;
    }
    QString values = QString::number(value);
    params.put("CameraOffsetAdj", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("CameraOffsetAdj"));
    int value = str.toInt();
    value = value + 5;
    if (value >= 1000 ) {
      value = 1000;
    }
    QString values = QString::number(value);
    params.put("CameraOffsetAdj", values.toStdString());
    refresh();
  });
  refresh();
}

void CameraOffset::refresh() {
  auto strs = QString::fromStdString(params.get("CameraOffsetAdj"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.001;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
  btnminus.setText("－");
  btnplus.setText("＋");
}

PathOffset::PathOffset() : AbstractControl("PathOffset", "Sets the PathOffset value. (+value:Move left, -value:Move right)", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("PathOffsetAdj"));
    int value = str.toInt();
    value = value - 5;
    if (value <= -1000 ) {
      value = -1000;
    }
    QString values = QString::number(value);
    params.put("PathOffsetAdj", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("PathOffsetAdj"));
    int value = str.toInt();
    value = value + 5;
    if (value >= 1000 ) {
      value = 1000;
    }
    QString values = QString::number(value);
    params.put("PathOffsetAdj", values.toStdString());
    refresh();
  });
  refresh();
}

void PathOffset::refresh() {
  auto strs = QString::fromStdString(params.get("PathOffsetAdj"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.001;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
  btnminus.setText("－");
  btnplus.setText("＋");
}

SRBaseControl::SRBaseControl() : AbstractControl("SteerRatio", "Sets the SteerRatio default value.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btndigit.setStyleSheet(R"(
    QPushButton {
      padding: 0;
      border-radius: 50px;
      font-size: 35px;
      font-weight: 500;
      color: #E4E4E4;
      background-color: #393939;
    }
    QPushButton:pressed {
      background-color: #ababab;
    }
  )");
  btnminus.setStyleSheet(R"(
    QPushButton {
      padding: 0;
      border-radius: 50px;
      font-size: 35px;
      font-weight: 500;
      color: #E4E4E4;
      background-color: #393939;
    }
    QPushButton:pressed {
      background-color: #ababab;
    }
  )");
  btnplus.setStyleSheet(R"(
    QPushButton {
      padding: 0;
      border-radius: 50px;
      font-size: 35px;
      font-weight: 500;
      color: #E4E4E4;
      background-color: #393939;
    }
    QPushButton:pressed {
      background-color: #ababab;
    }
  )");
  btndigit.setFixedSize(100, 100);
  btnminus.setFixedSize(100, 100);
  btnplus.setFixedSize(100, 100);
  hlayout->addWidget(&btndigit);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);
  btndigit.setText("0.01");
  btnminus.setText("-");
  btnplus.setText("+");

  QObject::connect(&btndigit, &QPushButton::clicked, [=]() {
    digit = digit * 10;
    if (digit >= 11 ) {
      digit = 0.01;
    }
    QString level = QString::number(digit);
    btndigit.setText(level);
  });

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerRatioAdj"));
    int value = str.toInt();
    value = value - (digit*100);
    if (value <= 800) {
      value = 800;
    }
    QString values = QString::number(value);
    params.put("SteerRatioAdj", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerRatioAdj"));
    int value = str.toInt();
    value = value + (digit*100);
    if (value >= 2000) {
      value = 2000;
    }
    QString values = QString::number(value);
    params.put("SteerRatioAdj", values.toStdString());
    refresh();
  });
  refresh();
}

void SRBaseControl::refresh() {
  auto strs = QString::fromStdString(params.get("SteerRatioAdj"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.01;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
}

SRMaxControl::SRMaxControl() : AbstractControl("SteerRatioMax", "Sets the SteerRatio maximum value.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btndigit.setStyleSheet(R"(
    QPushButton {
      padding: 0;
      border-radius: 50px;
      font-size: 35px;
      font-weight: 500;
      color: #E4E4E4;
      background-color: #393939;
    }
    QPushButton:pressed {
      background-color: #ababab;
    }
  )");
  btnminus.setStyleSheet(R"(
    QPushButton {
      padding: 0;
      border-radius: 50px;
      font-size: 35px;
      font-weight: 500;
      color: #E4E4E4;
      background-color: #393939;
    }
    QPushButton:pressed {
      background-color: #ababab;
    }
  )");
  btnplus.setStyleSheet(R"(
    QPushButton {
      padding: 0;
      border-radius: 50px;
      font-size: 35px;
      font-weight: 500;
      color: #E4E4E4;
      background-color: #393939;
    }
    QPushButton:pressed {
      background-color: #ababab;
    }
  )");
  btndigit.setFixedSize(100, 100);
  btnminus.setFixedSize(100, 100);
  btnplus.setFixedSize(100, 100);
  hlayout->addWidget(&btndigit);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);
  btndigit.setText("0.01");
  btnminus.setText("-");
  btnplus.setText("+");

  QObject::connect(&btndigit, &QPushButton::clicked, [=]() {
    digit = digit * 10;
    if (digit >= 11 ) {
      digit = 0.01;
    }
    QString level = QString::number(digit);
    btndigit.setText(level);
  });

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerRatioMaxAdj"));
    int value = str.toInt();
    value = value - (digit*100);
    if (value <= 800) {
      value = 800;
    }
    QString values = QString::number(value);
    params.put("SteerRatioMaxAdj", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerRatioMaxAdj"));
    int value = str.toInt();
    value = value + (digit*100);
    if (value >= 2000) {
      value = 2000;
    }
    QString values = QString::number(value);
    params.put("SteerRatioMaxAdj", values.toStdString());
    refresh();
  });
  refresh();
}

void SRMaxControl::refresh() {
  auto strs = QString::fromStdString(params.get("SteerRatioMaxAdj"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.01;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
}

SteerActuatorDelay::SteerActuatorDelay() : AbstractControl("SteerActuatorDelay", "Adjust the SteerActuatorDelay value.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerActuatorDelayAdj"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 1 ) {
      value = 1;
    }
    QString values = QString::number(value);
    params.put("SteerActuatorDelayAdj", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerActuatorDelayAdj"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 100 ) {
      value = 100;
    }
    QString values = QString::number(value);
    params.put("SteerActuatorDelayAdj", values.toStdString());
    refresh();
  });
  refresh();
}

void SteerActuatorDelay::refresh() {
  auto strs = QString::fromStdString(params.get("SteerActuatorDelayAdj"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.01;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
  btnminus.setText("－");
  btnplus.setText("＋");
}

SteerRateCost::SteerRateCost() : AbstractControl("SteerRateCost", "Adjust the SteerRateCost value.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerRateCostAdj"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 1 ) {
      value = 1;
    }
    QString values = QString::number(value);
    params.put("SteerRateCostAdj", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerRateCostAdj"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 200 ) {
      value = 200;
    }
    QString values = QString::number(value);
    params.put("SteerRateCostAdj", values.toStdString());
    refresh();
  });
  refresh();
}

void SteerRateCost::refresh() {
  auto strs = QString::fromStdString(params.get("SteerRateCostAdj"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.01;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
  btnminus.setText("－");
  btnplus.setText("＋");
}

SteerLimitTimer::SteerLimitTimer() : AbstractControl("SteerLimitTimer", "Adjust the SteerLimitTimer value.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerLimitTimerAdj"));
    int value = str.toInt();
    value = value - 5;
    if (value <= 0 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("SteerLimitTimerAdj", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerLimitTimerAdj"));
    int value = str.toInt();
    value = value + 5;
    if (value >= 300 ) {
      value = 300;
    }
    QString values = QString::number(value);
    params.put("SteerLimitTimerAdj", values.toStdString());
    refresh();
  });
  refresh();
}

void SteerLimitTimer::refresh() {
  auto strs = QString::fromStdString(params.get("SteerLimitTimerAdj"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.01;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
  btnminus.setText("－");
  btnplus.setText("＋");
}

TireStiffnessFactor::TireStiffnessFactor() : AbstractControl("TireStiffnessFactor", "Adjust the TireStiffnessFactor value.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("TireStiffnessFactorAdj"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 1 ) {
      value = 1;
    }
    QString values = QString::number(value);
    params.put("TireStiffnessFactorAdj", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("TireStiffnessFactorAdj"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 200 ) {
      value = 200;
    }
    QString values = QString::number(value);
    params.put("TireStiffnessFactorAdj", values.toStdString());
    refresh();
  });
  refresh();
}

void TireStiffnessFactor::refresh() {
  auto strs = QString::fromStdString(params.get("TireStiffnessFactorAdj"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.01;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
  btnminus.setText("－");
  btnplus.setText("＋");
}

SteerMaxBase::SteerMaxBase() : AbstractControl("SteerMaxDefault", "Adjust the SteerMax default.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerMaxBaseAdj"));
    int value = str.toInt();
    value = value - 2;
    if (value <= 200 ) {
      value = 200;
    }
    QString values = QString::number(value);
    params.put("SteerMaxBaseAdj", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerMaxBaseAdj"));
    int value = str.toInt();
    value = value + 2;
    if (value >= 512 ) {
      value = 512;
    }
    QString values = QString::number(value);
    params.put("SteerMaxBaseAdj", values.toStdString());
    refresh();
  });
  refresh();
}

void SteerMaxBase::refresh() {
  label.setText(QString::fromStdString(params.get("SteerMaxBaseAdj")));
  btnminus.setText("－");
  btnplus.setText("＋");
}

SteerMaxMax::SteerMaxMax() : AbstractControl("SteerMaxMax", "Adjust the SteerMax maximum.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerMaxAdj"));
    int value = str.toInt();
    value = value - 2;
    if (value <= 254 ) {
      value = 254;
    }
    QString values = QString::number(value);
    params.put("SteerMaxAdj", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerMaxAdj"));
    int value = str.toInt();
    value = value + 2;
    if (value >= 1024 ) {
      value = 1024;
    }
    QString values = QString::number(value);
    params.put("SteerMaxAdj", values.toStdString());
    refresh();
  });
  refresh();
}

void SteerMaxMax::refresh() {
  label.setText(QString::fromStdString(params.get("SteerMaxAdj")));
  btnminus.setText("－");
  btnplus.setText("＋");
}

SteerMaxv::SteerMaxv() : AbstractControl("SteerMaxV", "Adjust the SteerMaxV value.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerMaxvAdj"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 10 ) {
      value = 10;
    }
    QString values = QString::number(value);
    params.put("SteerMaxvAdj", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerMaxvAdj"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 30 ) {
      value = 30;
    }
    QString values = QString::number(value);
    params.put("SteerMaxvAdj", values.toStdString());
    refresh();
  });
  refresh();
}

void SteerMaxv::refresh() {
  auto strs = QString::fromStdString(params.get("SteerMaxvAdj"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.1;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
  btnminus.setText("－");
  btnplus.setText("＋");
}

SteerDeltaUpBase::SteerDeltaUpBase() : AbstractControl("SteerDeltaUpDefault", "Adjust the Steer DeltaUp default value.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerDeltaUpBaseAdj"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 2 ) {
      value = 2;
    }
    QString values = QString::number(value);
    params.put("SteerDeltaUpBaseAdj", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerDeltaUpBaseAdj"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 7 ) {
      value = 7;
    }
    QString values = QString::number(value);
    params.put("SteerDeltaUpBaseAdj", values.toStdString());
    refresh();
  });
  refresh();
}

void SteerDeltaUpBase::refresh() {
  label.setText(QString::fromStdString(params.get("SteerDeltaUpBaseAdj")));
  btnminus.setText("－");
  btnplus.setText("＋");
}

SteerDeltaUpMax::SteerDeltaUpMax() : AbstractControl("SteerDeltaUpMax", "Adjust the Steer DeltaUp maximum value.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerDeltaUpAdj"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 3 ) {
      value = 3;
    }
    QString values = QString::number(value);
    params.put("SteerDeltaUpAdj", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerDeltaUpAdj"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 7 ) {
      value = 7;
    }
    QString values = QString::number(value);
    params.put("SteerDeltaUpAdj", values.toStdString());
    refresh();
  });
  refresh();
}

void SteerDeltaUpMax::refresh() {
  label.setText(QString::fromStdString(params.get("SteerDeltaUpAdj")));
  btnminus.setText("－");
  btnplus.setText("＋");
}

SteerDeltaDownBase::SteerDeltaDownBase() : AbstractControl("SteerDeltaDownDefault", "Adjust the SteerDeltaDown default.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerDeltaDownBaseAdj"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 3 ) {
      value = 3;
    }
    QString values = QString::number(value);
    params.put("SteerDeltaDownBaseAdj", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerDeltaDownBaseAdj"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 15 ) {
      value = 15;
    }
    QString values = QString::number(value);
    params.put("SteerDeltaDownBaseAdj", values.toStdString());
    refresh();
  });
  refresh();
}

void SteerDeltaDownBase::refresh() {
  label.setText(QString::fromStdString(params.get("SteerDeltaDownBaseAdj")));
  btnminus.setText("－");
  btnplus.setText("＋");
}

SteerDeltaDownMax::SteerDeltaDownMax() : AbstractControl("SteerDeltaDownMax", "Adjust the SteerDeltaDown maximum.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerDeltaDownAdj"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 7 ) {
      value = 7;
    }
    QString values = QString::number(value);
    params.put("SteerDeltaDownAdj", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerDeltaDownAdj"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 15 ) {
      value = 15;
    }
    QString values = QString::number(value);
    params.put("SteerDeltaDownAdj", values.toStdString());
    refresh();
  });
  refresh();
}

void SteerDeltaDownMax::refresh() {
  label.setText(QString::fromStdString(params.get("SteerDeltaDownAdj")));
  btnminus.setText("－");
  btnplus.setText("＋");
}

SteerThreshold::SteerThreshold() : AbstractControl("SteerThreshold", "Adjust the SteerThreshold value.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerThreshold"));
    int value = str.toInt();
    value = value - 10;
    if (value <= 50 ) {
      value = 50;
    }
    QString values = QString::number(value);
    params.put("SteerThreshold", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("SteerThreshold"));
    int value = str.toInt();
    value = value + 10;
    if (value >= 300 ) {
      value = 300;
    }
    QString values = QString::number(value);
    params.put("SteerThreshold", values.toStdString());
    refresh();
  });
  refresh();
}

void SteerThreshold::refresh() {
  label.setText(QString::fromStdString(params.get("SteerThreshold")));
  btnminus.setText("－");
  btnplus.setText("＋");
}

//제어
LateralControl::LateralControl() : AbstractControl("LatControl(Reboot)", "Set the steering control method(PID/INDI/LQR). Reboot Required.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("LateralControlMethod"));
    int latcontrol = str.toInt();
    latcontrol = latcontrol - 1;
    if (latcontrol <= -1 ) {
      latcontrol = 2;
    }
    QString latcontrols = QString::number(latcontrol);
    params.put("LateralControlMethod", latcontrols.toStdString());
    refresh();
  });

  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("LateralControlMethod"));
    int latcontrol = str.toInt();
    latcontrol = latcontrol + 1;
    if (latcontrol >= 3 ) {
      latcontrol = 0;
    }
    QString latcontrols = QString::number(latcontrol);
    params.put("LateralControlMethod", latcontrols.toStdString());
    refresh();
  });
  refresh();
}

void LateralControl::refresh() {
  QString latcontrol = QString::fromStdString(params.get("LateralControlMethod"));
  if (latcontrol == "0") {
    label.setText(QString::fromStdString("PID"));
  } else if (latcontrol == "1") {
    label.setText(QString::fromStdString("INDI"));
  } else if (latcontrol == "2") {
    label.setText(QString::fromStdString("LQR"));
  }
  btnminus.setText("◀");
  btnplus.setText("▶");
}

PidKp::PidKp() : AbstractControl("Kp", "Adjust Kp", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("PidKp"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 1 ) {
      value = 1;
    }
    QString values = QString::number(value);
    params.put("PidKp", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("PidKp"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 50 ) {
      value = 50;
    }
    QString values = QString::number(value);
    params.put("PidKp", values.toStdString());
    refresh();
  });
  refresh();
}

void PidKp::refresh() {
  auto strs = QString::fromStdString(params.get("PidKp"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.01;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
  btnminus.setText("－");
  btnplus.setText("＋");
}

PidKi::PidKi() : AbstractControl("Ki", "Adjust Ki", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("PidKi"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 1 ) {
      value = 1;
    }
    QString values = QString::number(value);
    params.put("PidKi", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("PidKi"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 100 ) {
      value = 100;
    }
    QString values = QString::number(value);
    params.put("PidKi", values.toStdString());
    refresh();
  });
  refresh();
}

void PidKi::refresh() {
  auto strs = QString::fromStdString(params.get("PidKi"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.001;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
  btnminus.setText("－");
  btnplus.setText("＋");
}

PidKd::PidKd() : AbstractControl("Kd", "Adjust Kd", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("PidKd"));
    int value = str.toInt();
    value = value - 5;
    if (value <= 0 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("PidKd", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("PidKd"));
    int value = str.toInt();
    value = value + 5;
    if (value >= 300 ) {
      value = 300;
    }
    QString values = QString::number(value);
    params.put("PidKd", values.toStdString());
    refresh();
  });
  refresh();
}

void PidKd::refresh() {
  auto strs = QString::fromStdString(params.get("PidKd"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.01;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
  btnminus.setText("－");
  btnplus.setText("＋");
}

PidKf::PidKf() : AbstractControl("Kf", "Adjust Kf", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("PidKf"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 1 ) {
      value = 1;
    }
    QString values = QString::number(value);
    params.put("PidKf", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("PidKf"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 50 ) {
      value = 50;
    }
    QString values = QString::number(value);
    params.put("PidKf", values.toStdString());
    refresh();
  });
  refresh();
}

void PidKf::refresh() {
  auto strs = QString::fromStdString(params.get("PidKf"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.00001;
  QString valuefs = QString::number(valuef, 'f', 5);
  label.setText(QString::fromStdString(valuefs.toStdString()));
  btnminus.setText("－");
  btnplus.setText("＋");
}

OuterLoopGain::OuterLoopGain() : AbstractControl("OuterLoopGain", "Adjust OuterLoopGain", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OuterLoopGain"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 1 ) {
      value = 1;
    }
    QString values = QString::number(value);
    params.put("OuterLoopGain", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OuterLoopGain"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 200 ) {
      value = 200;
    }
    QString values = QString::number(value);
    params.put("OuterLoopGain", values.toStdString());
    refresh();
  });
  refresh();
}

void OuterLoopGain::refresh() {
  auto strs = QString::fromStdString(params.get("OuterLoopGain"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.1;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
  btnminus.setText("－");
  btnplus.setText("＋");
}

InnerLoopGain::InnerLoopGain() : AbstractControl("InnerLoopGain", "Adjust InnerLoopGain", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("InnerLoopGain"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 1 ) {
      value = 1;
    }
    QString values = QString::number(value);
    params.put("InnerLoopGain", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("InnerLoopGain"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 200 ) {
      value = 200;
    }
    QString values = QString::number(value);
    params.put("InnerLoopGain", values.toStdString());
    refresh();
  });
  refresh();
}

void InnerLoopGain::refresh() {
  auto strs = QString::fromStdString(params.get("InnerLoopGain"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.1;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
  btnminus.setText("－");
  btnplus.setText("＋");
}

TimeConstant::TimeConstant() : AbstractControl("TimeConstant", "Adjust TimeConstant", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("TimeConstant"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 1 ) {
      value = 1;
    }
    QString values = QString::number(value);
    params.put("TimeConstant", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("TimeConstant"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 200 ) {
      value = 200;
    }
    QString values = QString::number(value);
    params.put("TimeConstant", values.toStdString());
    refresh();
  });
  refresh();
}

void TimeConstant::refresh() {
  auto strs = QString::fromStdString(params.get("TimeConstant"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.1;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
  btnminus.setText("－");
  btnplus.setText("＋");
}

ActuatorEffectiveness::ActuatorEffectiveness() : AbstractControl("ActuatorEffectiveness", "Adjust ActuatorEffectiveness", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("ActuatorEffectiveness"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 1 ) {
      value = 1;
    }
    QString values = QString::number(value);
    params.put("ActuatorEffectiveness", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("ActuatorEffectiveness"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 200 ) {
      value = 200;
    }
    QString values = QString::number(value);
    params.put("ActuatorEffectiveness", values.toStdString());
    refresh();
  });
  refresh();
}

void ActuatorEffectiveness::refresh() {
  auto strs = QString::fromStdString(params.get("ActuatorEffectiveness"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.1;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
  btnminus.setText("－");
  btnplus.setText("＋");
}

Scale::Scale() : AbstractControl("Scale", "Adjust Scale", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("Scale"));
    int value = str.toInt();
    value = value - 50;
    if (value <= 50 ) {
      value = 50;
    }
    QString values = QString::number(value);
    params.put("Scale", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("Scale"));
    int value = str.toInt();
    value = value + 50;
    if (value >= 5000 ) {
      value = 5000;
    }
    QString values = QString::number(value);
    params.put("Scale", values.toStdString());
    refresh();
  });
  refresh();
}

void Scale::refresh() {
  label.setText(QString::fromStdString(params.get("Scale")));
  btnminus.setText("－");
  btnplus.setText("＋");
}

LqrKi::LqrKi() : AbstractControl("LqrKi", "Adjust ki", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("LqrKi"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 1 ) {
      value = 1;
    }
    QString values = QString::number(value);
    params.put("LqrKi", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("LqrKi"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 100 ) {
      value = 100;
    }
    QString values = QString::number(value);
    params.put("LqrKi", values.toStdString());
    refresh();
  });
  refresh();
}

void LqrKi::refresh() {
  auto strs = QString::fromStdString(params.get("LqrKi"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.001;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
  btnminus.setText("－");
  btnplus.setText("＋");
}

DcGain::DcGain() : AbstractControl("DcGain", "Adjust DcGain", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("DcGain"));
    int value = str.toInt();
    value = value - 5;
    if (value <= 5 ) {
      value = 5;
    }
    QString values = QString::number(value);
    params.put("DcGain", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("DcGain"));
    int value = str.toInt();
    value = value + 5;
    if (value >= 500 ) {
      value = 500;
    }
    QString values = QString::number(value);
    params.put("DcGain", values.toStdString());
    refresh();
  });
  refresh();
}

void DcGain::refresh() {
  auto strs = QString::fromStdString(params.get("DcGain"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.00001;
  QString valuefs = QString::number(valuef);
  label.setText(QString::fromStdString(valuefs.toStdString()));
  btnminus.setText("－");
  btnplus.setText("＋");
}

CruiseGapTR::CruiseGapTR() : AbstractControl("CruiseGap", "Adjust the inter-vehicle distance (TR) according to the cruise gap. TR refers to the time in seconds of collision with the car in front, and the larger it becomes, the farther it is from the car in front.", "") {
  QString dtr = QString::fromStdString(params.get("DynamicTR"));
  if (dtr == "0") {
    btn1.setStyleSheet(R"(
      padding: -10;
      border-radius: 35px;
      font-size: 30px;
      font-weight: 500;
      color: #E4E4E4;
      background-color: #393939;
    )");
    btn2.setStyleSheet(R"(
      padding: -10;
      border-radius: 35px;
      font-size: 30px;
      font-weight: 500;
      color: #E4E4E4;
      background-color: #393939;
    )");
    btn3.setStyleSheet(R"(
      padding: -10;
      border-radius: 35px;
      font-size: 30px;
      font-weight: 500;
      color: #E4E4E4;
      background-color: #393939;
    )");
    btn4.setStyleSheet(R"(
      padding: -10;
      border-radius: 35px;
      font-size: 30px;
      font-weight: 500;
      color: #E4E4E4;
      background-color: #393939;
    )");
  } else {
    btn1.setStyleSheet(R"(
      padding: 0;
      border-radius: 50px;
      font-size: 35px;
      font-weight: 500;
      color: #E4E4E4;
      background-color: #393939;
    )");
    btn2.setStyleSheet(R"(
      padding: 0;
      border-radius: 50px;
      font-size: 35px;
      font-weight: 500;
      color: #E4E4E4;
      background-color: #393939;
    )");
    btn3.setStyleSheet(R"(
      padding: 0;
      border-radius: 50px;
      font-size: 35px;
      font-weight: 500;
      color: #E4E4E4;
      background-color: #393939;
    )");
    btn4.setStyleSheet(R"(
      padding: 0;
      border-radius: 50px;
      font-size: 35px;
      font-weight: 500;
      color: #E4E4E4;
      background-color: #393939;
    )");
  }
  label1.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label1.setStyleSheet("color: #e0e879");
  label2.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label2.setStyleSheet("color: #e0e879");
  label3.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label3.setStyleSheet("color: #e0e879");
  label4.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label4.setStyleSheet("color: #e0e879");
  label1a.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label2a.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label3a.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label4a.setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  if (dtr != "1") {
    hlayout->addWidget(&label1a);
    hlayout->addWidget(&label1);
    if (dtr == "0") {
      btn1.setFixedSize(60, 100);
      label1a.setText("1:");
    } else {
      btn1.setFixedSize(100, 100);
      label1a.setText("1S:");
    }
    hlayout->addWidget(&btn1);
  }
  if (dtr != "2") {
    hlayout->addWidget(&label2a);
    hlayout->addWidget(&label2);
    if (dtr == "0") {
      btn2.setFixedSize(60, 100);
      label2a.setText("2:");
    } else {
      btn2.setFixedSize(100, 100);
      label2a.setText("2S:");
    }
    hlayout->addWidget(&btn2);
  }
  if (dtr != "3") {
    hlayout->addWidget(&label3a);
    hlayout->addWidget(&label3);
    if (dtr == "0") {
      btn3.setFixedSize(60, 100);
      label3a.setText("3:");
    } else {
      btn3.setFixedSize(100, 100);
      label3a.setText("3S:");
    }
    hlayout->addWidget(&btn3);
  }
  if (dtr != "4") {
    hlayout->addWidget(&label4a);
    hlayout->addWidget(&label4);
    if (dtr == "0") {
      btn4.setFixedSize(60, 100);
      label4a.setText("4:");
    } else {
      btn4.setFixedSize(100, 100);
      label4a.setText("4S:");
    }
    hlayout->addWidget(&btn4);
  }

  QObject::connect(&btn1, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("CruiseGap1"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 13 ) {
      value = 7;
    }
    QString values = QString::number(value);
    params.put("CruiseGap1", values.toStdString());
    refresh1();
  });

  QObject::connect(&btn2, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("CruiseGap2"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 16 ) {
      value = 8;
    }
    QString values = QString::number(value);
    params.put("CruiseGap2", values.toStdString());
    refresh2();
  });
  
  QObject::connect(&btn3, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("CruiseGap3"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 20 ) {
      value = 10;
    }
    QString values = QString::number(value);
    params.put("CruiseGap3", values.toStdString());
    refresh3();
  });

  QObject::connect(&btn4, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("CruiseGap4"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 30 ) {
      value = 12;
    }
    QString values = QString::number(value);
    params.put("CruiseGap4", values.toStdString());
    refresh4();
  });

  refresh1();
  refresh2();
  refresh3();
  refresh4();
}

void CruiseGapTR::refresh1() {
  auto strs = QString::fromStdString(params.get("CruiseGap1"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.1;
  QString valuefs = QString::number(valuef);
  label1.setText(QString::fromStdString(valuefs.toStdString()));
  btn1.setText("▲");
}
void CruiseGapTR::refresh2() {
  auto strs = QString::fromStdString(params.get("CruiseGap2"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.1;
  QString valuefs = QString::number(valuef);
  label2.setText(QString::fromStdString(valuefs.toStdString()));
  btn2.setText("▲");
}
void CruiseGapTR::refresh3() {
  auto strs = QString::fromStdString(params.get("CruiseGap3"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.1;
  QString valuefs = QString::number(valuef);
  label3.setText(QString::fromStdString(valuefs.toStdString()));
  btn3.setText("▲");
}
void CruiseGapTR::refresh4() {
  auto strs = QString::fromStdString(params.get("CruiseGap4"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.1;
  QString valuefs = QString::number(valuef);
  label4.setText(QString::fromStdString(valuefs.toStdString()));
  btn4.setText("▲");
}

DynamicTR::DynamicTR() : AbstractControl("Use DynamicTR", "Use DynamicTR and assign it to the corresponding gap.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("DynamicTR"));
    int value = str.toInt();
    value = value - 1;
    if (value <= -1 ) {
      value = 4;
    }
    QString values = QString::number(value);
    params.put("DynamicTR", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("DynamicTR"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 5 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("DynamicTR", values.toStdString());
    refresh();
  });
  refresh();
}

void DynamicTR::refresh() {
  QString option = QString::fromStdString(params.get("DynamicTR"));
  if (option == "0") {
    label.setText(QString::fromStdString("UnUse"));
  } else if (option == "1") {
    label.setText(QString::fromStdString("■"));
  } else if (option == "2") {
    label.setText(QString::fromStdString("■■"));
  } else if (option == "3") {
    label.setText(QString::fromStdString("■■■"));
  } else {
    label.setText(QString::fromStdString("■■■■"));
  }
  btnminus.setText("◀");
  btnplus.setText("▶");
}

LCTimingFactor::LCTimingFactor() : AbstractControl("", "", "") {

  btn1.setStyleSheet(R"(
    padding: -10;
    border-radius: 35px;
    font-size: 30px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btn2.setStyleSheet(R"(
    padding: -10;
    border-radius: 35px;
    font-size: 30px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btn3.setStyleSheet(R"(
    padding: -10;
    border-radius: 35px;
    font-size: 30px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btn4.setStyleSheet(R"(
    padding: -10;
    border-radius: 35px;
    font-size: 30px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  label1.setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  label1.setStyleSheet("color: #e0e879");
  label2.setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  label2.setStyleSheet("color: #e0e879");
  label3.setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  label3.setStyleSheet("color: #e0e879");
  label4.setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  label4.setStyleSheet("color: #e0e879");
  label1a.setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  label2a.setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  label3a.setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  label4a.setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

  hlayout->addWidget(&label1a);
  hlayout->addWidget(&label1);
  btn1.setFixedSize(50, 100);
  label1a.setText("30:");
  hlayout->addWidget(&btn1);
  hlayout->addWidget(&label2a);
  hlayout->addWidget(&label2);
  btn2.setFixedSize(50, 100);
  label2a.setText("60:");
  hlayout->addWidget(&btn2);
  hlayout->addWidget(&label3a);
  hlayout->addWidget(&label3);
  btn3.setFixedSize(50, 100);
  label3a.setText("80:");
  hlayout->addWidget(&btn3);
  hlayout->addWidget(&label4a);
  hlayout->addWidget(&label4);
  btn4.setFixedSize(50, 100);
  label4a.setText("110:");
  hlayout->addWidget(&btn4);

  QObject::connect(&btn1, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("LCTimingFactor30"));
    int value = str.toInt();
    auto str2 = QString::fromStdString(params.get("LCTimingFactor60"));
    int value2 = str2.toInt();
    auto str_ud = QString::fromStdString(params.get("LCTimingFactorUD"));
    if (str_ud == "1") {
      value = value + 5;
    } else {
      value = value - 5;
    }
    if (value >= value2 ) {
      value = value2;
    } else if (value <= 5 ) {
      value = 5;
    }
    QString values = QString::number(value);
    params.put("LCTimingFactor30", values.toStdString());
    refresh1();
  });

  QObject::connect(&btn2, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("LCTimingFactor60"));
    int value = str.toInt();
    auto str0 = QString::fromStdString(params.get("LCTimingFactor30"));
    int value0 = str0.toInt();
    auto str2 = QString::fromStdString(params.get("LCTimingFactor80"));
    int value2 = str2.toInt();
    auto str_ud = QString::fromStdString(params.get("LCTimingFactorUD"));
    if (str_ud == "1") {
      value = value + 5;
    } else {
      value = value - 5;
    }
    if (value >= value2 ) {
      value = value2;
    } else if (value <= value0 ) {
      value = value0;
    }
    QString values = QString::number(value);
    params.put("LCTimingFactor60", values.toStdString());
    refresh2();
  });
  
  QObject::connect(&btn3, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("LCTimingFactor80"));
    int value = str.toInt();
    auto str0 = QString::fromStdString(params.get("LCTimingFactor60"));
    int value0 = str0.toInt();
    auto str2 = QString::fromStdString(params.get("LCTimingFactor110"));
    int value2 = str2.toInt();
    auto str_ud = QString::fromStdString(params.get("LCTimingFactorUD"));
    if (str_ud == "1") {
      value = value + 5;
    } else {
      value = value - 5;
    }
    if (value >= value2 ) {
      value = value2;
    } else if (value <= value0 ) {
      value = value0;
    }
    QString values = QString::number(value);
    params.put("LCTimingFactor80", values.toStdString());
    refresh3();
  });

  QObject::connect(&btn4, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("LCTimingFactor110"));
    int value = str.toInt();
    auto str0 = QString::fromStdString(params.get("LCTimingFactor80"));
    int value0 = str0.toInt();
    auto str_ud = QString::fromStdString(params.get("LCTimingFactorUD"));
    if (str_ud == "1") {
      value = value + 5;
    } else {
      value = value - 5;
    }
    if (value <= value0 ) {
      value = value0;
    } else if (value >= 300 ) {
      value = 300;
    }
    QString values = QString::number(value);
    params.put("LCTimingFactor110", values.toStdString());
    refresh4();
  });

  refresh1();
  refresh2();
  refresh3();
  refresh4();
}

void LCTimingFactor::refresh1() {
  auto strs = QString::fromStdString(params.get("LCTimingFactor30"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.01;
  QString valuefs = QString::number(valuef);
  label1.setText(QString::fromStdString(valuefs.toStdString()));
  btn1.setText("↕");
}
void LCTimingFactor::refresh2() {
  auto strs = QString::fromStdString(params.get("LCTimingFactor60"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.01;
  QString valuefs = QString::number(valuef);
  label2.setText(QString::fromStdString(valuefs.toStdString()));
  btn2.setText("↕");
}
void LCTimingFactor::refresh3() {
  auto strs = QString::fromStdString(params.get("LCTimingFactor80"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.01;
  QString valuefs = QString::number(valuef);
  label3.setText(QString::fromStdString(valuefs.toStdString()));
  btn3.setText("↕");
}
void LCTimingFactor::refresh4() {
  auto strs = QString::fromStdString(params.get("LCTimingFactor110"));
  int valuei = strs.toInt();
  float valuef = valuei * 0.01;
  QString valuefs = QString::number(valuef);
  label4.setText(QString::fromStdString(valuefs.toStdString()));
  btn4.setText("↕");
}

LCTimingFactorUD::LCTimingFactorUD() : AbstractControl("LaneChange Time (km/h: value)", "When changing lanes, adjust the timing of lane change for each speed. If you want a quick lane change, increase the value and lower the value if you want a slow lane change.", "../assets/offroad/icon_shell.png") {

  btn.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btn.setFixedSize(125, 100);
  btn2.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btn2.setFixedSize(150, 100);
  hlayout->addWidget(&btn2);
  hlayout->addWidget(&btn);

  QObject::connect(&btn2, &QPushButton::clicked, [=]() {
    bool stat = params.getBool("LCTimingFactorEnable");
    if (stat) {
      params.putBool("LCTimingFactorEnable", false);
    } else {
      params.putBool("LCTimingFactorEnable", true);
    }
    refresh2();
  });

  QObject::connect(&btn, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("LCTimingFactorUD"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 2 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("LCTimingFactorUD", values.toStdString());
    refresh();
  });
  refresh();
  refresh2();
}

void LCTimingFactorUD::refresh() {
  auto strs = QString::fromStdString(params.get("LCTimingFactorUD"));
  if (strs == "1") {
    btn.setText("↑");
  } else {
    btn.setText("↓");
  }
}

void LCTimingFactorUD::refresh2() {
  bool param = params.getBool("LCTimingFactorEnable");
  if (param) {
    btn2.setText("ON");
    btn2.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #00A12E;
    )");
  } else {
    btn2.setText("OFF");
    btn2.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  }
}

LiveSRPercent::LiveSRPercent() : AbstractControl("LiveSR Adjust(%)", "When using LiveSR, the learned value is arbitrarily adjusted (%) and used. -Value:Lower from learned value, +Value:Lower from learned value", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("LiveSteerRatioPercent"));
    int value = str.toInt();
    value = value - 1;
    if (value <= -50 ) {
      value = -50;
    }
    QString values = QString::number(value);
    params.put("LiveSteerRatioPercent", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("LiveSteerRatioPercent"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 50 ) {
      value = 50;
    }
    QString values = QString::number(value);
    params.put("LiveSteerRatioPercent", values.toStdString());
    refresh();
  });
  refresh();
}

void LiveSRPercent::refresh() {
  QString option = QString::fromStdString(params.get("LiveSteerRatioPercent"));
  if (option == "0") {
    label.setText(QString::fromStdString("Default"));
  } else {
    label.setText(QString::fromStdString(params.get("LiveSteerRatioPercent")));
  }
  btnminus.setText("-");
  btnplus.setText("+");
}

VCurvSpeed::VCurvSpeed() : AbstractControl("", "", "") {

  btn1.setStyleSheet(R"(
    padding: -10;
    border-radius: 35px;
    font-size: 30px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btn2.setStyleSheet(R"(
    padding: -10;
    border-radius: 35px;
    font-size: 30px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btn3.setStyleSheet(R"(
    padding: -10;
    border-radius: 35px;
    font-size: 30px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btn4.setStyleSheet(R"(
    padding: -10;
    border-radius: 35px;
    font-size: 30px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  label1.setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  label1.setStyleSheet("color: #e0e879");
  label2.setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  label2.setStyleSheet("color: #e0e879");
  label3.setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  label3.setStyleSheet("color: #e0e879");
  label4.setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  label4.setStyleSheet("color: #e0e879");
  label1a.setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  label2a.setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  label3a.setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  label4a.setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

  hlayout->addWidget(&label1a);
  hlayout->addWidget(&label1);
  btn1.setFixedSize(50, 100);
  label1a.setText("30:");
  hlayout->addWidget(&btn1);
  hlayout->addWidget(&label2a);
  hlayout->addWidget(&label2);
  btn2.setFixedSize(50, 100);
  label2a.setText("50:");
  hlayout->addWidget(&btn2);
  hlayout->addWidget(&label3a);
  hlayout->addWidget(&label3);
  btn3.setFixedSize(50, 100);
  label3a.setText("70:");
  hlayout->addWidget(&btn3);
  hlayout->addWidget(&label4a);
  hlayout->addWidget(&label4);
  btn4.setFixedSize(50, 100);
  label4a.setText("90:");
  hlayout->addWidget(&btn4);

  QObject::connect(&btn1, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("VCurvSpeed30"));
    int value = str.toInt();
    auto str2 = QString::fromStdString(params.get("VCurvSpeed50"));
    int value2 = str2.toInt();
    auto str_ud = QString::fromStdString(params.get("VCurvSpeedUD"));
    if (str_ud == "1") {
      value = value + 1;
    } else {
      value = value - 1;
    }
    if (value >= value2 ) {
      value = value2;
    } else if (value <= 20 ) {
      value = 20;
    }
    QString values = QString::number(value);
    params.put("VCurvSpeed30", values.toStdString());
    refresh1();
  });

  QObject::connect(&btn2, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("VCurvSpeed50"));
    int value = str.toInt();
    auto str0 = QString::fromStdString(params.get("VCurvSpeed30"));
    int value0 = str0.toInt();
    auto str2 = QString::fromStdString(params.get("VCurvSpeed70"));
    int value2 = str2.toInt();
    auto str_ud = QString::fromStdString(params.get("VCurvSpeedUD"));
    if (str_ud == "1") {
      value = value + 1;
    } else {
      value = value - 1;
    }
    if (value >= value2 ) {
      value = value2;
    } else if (value <= value0 ) {
      value = value0;
    }
    QString values = QString::number(value);
    params.put("VCurvSpeed50", values.toStdString());
    refresh2();
  });
  
  QObject::connect(&btn3, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("VCurvSpeed70"));
    int value = str.toInt();
    auto str0 = QString::fromStdString(params.get("VCurvSpeed50"));
    int value0 = str0.toInt();
    auto str2 = QString::fromStdString(params.get("VCurvSpeed90"));
    int value2 = str2.toInt();
    auto str_ud = QString::fromStdString(params.get("VCurvSpeedUD"));
    if (str_ud == "1") {
      value = value + 1;
    } else {
      value = value - 1;
    }
    if (value >= value2 ) {
      value = value2;
    } else if (value <= value0 ) {
      value = value0;
    }
    QString values = QString::number(value);
    params.put("VCurvSpeed70", values.toStdString());
    refresh3();
  });

  QObject::connect(&btn4, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("VCurvSpeed90"));
    int value = str.toInt();
    auto str0 = QString::fromStdString(params.get("VCurvSpeed70"));
    int value0 = str0.toInt();
    auto str_ud = QString::fromStdString(params.get("VCurvSpeedUD"));
    if (str_ud == "1") {
      value = value + 1;
    } else {
      value = value - 1;
    }
    if (value <= value0 ) {
      value = value0;
    } else if (value >= 90 ) {
      value = 90;
    }
    QString values = QString::number(value);
    params.put("VCurvSpeed90", values.toStdString());
    refresh4();
  });

  refresh1();
  refresh2();
  refresh3();
  refresh4();
}

void VCurvSpeed::refresh1() {
  auto strs = QString::fromStdString(params.get("VCurvSpeed30"));
  int valuei = strs.toInt();
  QString valuefs = QString::number(valuei);
  label1.setText(QString::fromStdString(valuefs.toStdString()));
  btn1.setText("↕");
}
void VCurvSpeed::refresh2() {
  auto strs = QString::fromStdString(params.get("VCurvSpeed50"));
  int valuei = strs.toInt();
  QString valuefs = QString::number(valuei);
  label2.setText(QString::fromStdString(valuefs.toStdString()));
  btn2.setText("↕");
}
void VCurvSpeed::refresh3() {
  auto strs = QString::fromStdString(params.get("VCurvSpeed70"));
  int valuei = strs.toInt();
  QString valuefs = QString::number(valuei);
  label3.setText(QString::fromStdString(valuefs.toStdString()));
  btn3.setText("↕");
}
void VCurvSpeed::refresh4() {
  auto strs = QString::fromStdString(params.get("VCurvSpeed90"));
  int valuei = strs.toInt();
  QString valuefs = QString::number(valuei);
  label4.setText(QString::fromStdString(valuefs.toStdString()));
  btn4.setText("↕");
}

VCurvSpeedUD::VCurvSpeedUD() : AbstractControl("VisionCurvDecel(ModelSpeed: CarSpeed)", "Adjust the curve deceleration speed according to the model speed. (interpolation value)", "../assets/offroad/icon_shell.png") {

  btn.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btn.setFixedSize(125, 100);
  hlayout->addWidget(&btn);

  QObject::connect(&btn, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("VCurvSpeedUD"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 2 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("VCurvSpeedUD", values.toStdString());
    refresh();
  });
  refresh();
}

void VCurvSpeedUD::refresh() {
  auto strs = QString::fromStdString(params.get("VCurvSpeedUD"));
  if (strs == "1") {
    btn.setText("↑");
  } else {
    btn.setText("↓");
  }
}

OCurvOffset::OCurvOffset() : AbstractControl("OSM CurvDecel(%)", "Adjust the speed of the OSM curve (+value: speed up, -value: speed down)", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OCurvOffset"));
    int value = str.toInt();
    value = value - 1;
    if (value <= -50 ) {
      value = -50;
    }
    QString values = QString::number(value);
    //QUIState::ui_state.speed_lim_off = value;
    params.put("OCurvOffset", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OCurvOffset"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 50 ) {
      value = 50;
    }
    QString values = QString::number(value);
    //QUIState::ui_state.speed_lim_off = value;
    params.put("OCurvOffset", values.toStdString());
    refresh();
  });
  refresh();
}

void OCurvOffset::refresh() {
  label.setText(QString::fromStdString(params.get("OCurvOffset")));
  btnminus.setText("－");
  btnplus.setText("＋");
}

GetOffAlert::GetOffAlert() : AbstractControl("EON Detach Alert Sound", "Device alert you a alarm to detach the EON when ignition off.(NO Alert/KOR/ENG)", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrEnableGetoffAlert"));
    int value = str.toInt();
    value = value - 1;
    if (value <= -1 ) {
      value = 2;
    }
    QString values = QString::number(value);
    params.put("OpkrEnableGetoffAlert", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrEnableGetoffAlert"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 3 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("OpkrEnableGetoffAlert", values.toStdString());
    refresh();
  });
  refresh();
}

void GetOffAlert::refresh() {
  QString option = QString::fromStdString(params.get("OpkrEnableGetoffAlert"));
  if (option == "0") {
    label.setText(QString::fromStdString("None"));
  } else if (option == "1") {
    label.setText(QString::fromStdString("KOR"));
  } else {
    label.setText(QString::fromStdString("ENG"));
  }
  btnminus.setText("◀");
  btnplus.setText("▶");
}

OPKRNaviSelect::OPKRNaviSelect() : AbstractControl("Navigation Select", "Select the navigation you want to use.(Mappy/Waze)", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OPKRNaviSelect"));
    int value = str.toInt();
    value = value - 1;
    if (value <= -1 ) {
      value = 1;
    }
    QString values = QString::number(value);
    params.put("OPKRNaviSelect", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OPKRNaviSelect"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 2 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("OPKRNaviSelect", values.toStdString());
    refresh();
  });
  refresh();
}

void OPKRNaviSelect::refresh() {
  QString option = QString::fromStdString(params.get("OPKRNaviSelect"));
  if (option == "0") {
    label.setText(QString::fromStdString("Mappy"));
  } else {
    label.setText(QString::fromStdString("Waze"));
  }
  btnminus.setText("◀");
  btnplus.setText("▶");
}

OPKRServerSelect::OPKRServerSelect() : AbstractControl("API Server", "Set API server to Retropilot/Comma/User's", "../assets/offroad/icon_shell.png") {
  btn1.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btn1.setFixedSize(250, 100);
  btn2.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btn2.setFixedSize(250, 100);
  btn3.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btn3.setFixedSize(250, 100);
  hlayout->addWidget(&btn1);
  hlayout->addWidget(&btn2);
  hlayout->addWidget(&btn3);
  btn1.setText("Retropilot");
  btn2.setText("Comma");
  btn3.setText("User's");

  QObject::connect(&btn1, &QPushButton::clicked, [=]() {
    params.put("OPKRServer", "0");
    refresh();
  });
  QObject::connect(&btn2, &QPushButton::clicked, [=]() {
    params.put("OPKRServer", "1");
    if (ConfirmationDialog::alert("You've chosen comma server. Your uploads might be ignored if you upload your data. I highly recommend you should reset the device to avoid be banned.", this)) {}
    refresh();
  });
  QObject::connect(&btn3, &QPushButton::clicked, [=]() {
    params.put("OPKRServer", "2");
    if (ConfirmationDialog::alert("You've chosen own server. Please set your api host at the menu below.", this)) {}
    refresh();
  });
  refresh();
}

void OPKRServerSelect::refresh() {
  QString option = QString::fromStdString(params.get("OPKRServer"));
  if (option == "0") {
    btn1.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #00A12E;
    )");
    btn2.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
    )");
    btn3.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
    )");
  } else if (option == "1") {
    btn1.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
    )");
    btn2.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #00A12E;
    )");
    btn3.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
    )");
  } else {
    btn1.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
    )");
    btn2.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
    )");
    btn3.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #00A12E;
    )");
  }
}

OPKRServerAPI::OPKRServerAPI() : AbstractControl("User's API", "Set Your API server URL or IP", "") {
  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);
  btn.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btn.setFixedSize(150, 100);
  hlayout->addWidget(&btn);

  QObject::connect(&btn, &QPushButton::clicked, [=]() {
    if (btn.text() == "SET") {
      QString users_api_host = InputDialog::getText("Input Your API(url or ip):", this);
      if (users_api_host.length() > 0) {
        QString cmd0 = QString::fromStdString("Your Input is\n") + users_api_host + QString::fromStdString("\nPress OK to apply&reboot");
        if (ConfirmationDialog::confirm(cmd0, this)) {
          params.put("OPKRServerAPI", users_api_host.toStdString());
          params.put("OPKRServer", "2");
          QProcess::execute("rm -f /data/params/d/DongleId");
          QProcess::execute("rm -f /data/params/d/IMEI");
          QProcess::execute("rm -f /data/params/d/HardwareSerial");
          QProcess::execute("reboot");
        }
      }
    } else if (btn.text() == "UNSET") {
      if (ConfirmationDialog::confirm("Do you want to unset? the API server gets back to Retropilot server and Device will be rebooted now.", this)) {
        params.remove("OPKRServerAPI");
        params.put("OPKRServer", "0");
        QProcess::execute("rm -f /data/params/d/DongleId");
        QProcess::execute("rm -f /data/params/d/IMEI");
        QProcess::execute("rm -f /data/params/d/HardwareSerial");
        QProcess::execute("reboot");
      }
    }
  });
  refresh();
}

void OPKRServerAPI::refresh() {
  auto str = QString::fromStdString(params.get("OPKRServerAPI"));
  if (str.length() > 0) {
    label.setText(QString::fromStdString(params.get("OPKRServerAPI")));
    btn.setText("UNSET");
  } else {
    btn.setText("SET");
  }
}

OPKRMapboxStyle::OPKRMapboxStyle() : AbstractControl("Mapbox Style", "Set the Mapbox sytle to Comma/OPKR/User's", "../assets/offroad/icon_shell.png") {
  btn1.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btn1.setFixedSize(250, 100);
  btn2.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btn2.setFixedSize(250, 100);
  btn3.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btn3.setFixedSize(250, 100);
  hlayout->addWidget(&btn1);
  hlayout->addWidget(&btn2);
  hlayout->addWidget(&btn3);
  btn1.setText("Comma");
  btn2.setText("OPKR");
  btn3.setText("User's");

  QObject::connect(&btn1, &QPushButton::clicked, [=]() {
    params.put("OPKRMapboxStyleSelect", "0");
    refresh();
  });
  QObject::connect(&btn2, &QPushButton::clicked, [=]() {
    params.put("OPKRMapboxStyleSelect", "1");
    refresh();
  });
  QObject::connect(&btn3, &QPushButton::clicked, [=]() {
    params.put("OPKRMapboxStyleSelect", "2");
    if (ConfirmationDialog::alert("You've chosen own style. Please set your mapbox style to the param <MapboxStyleCustom>. File location: /data/params/d/MapboxStyleCustom", this)) {}
    refresh();
  });
  refresh();
}

void OPKRMapboxStyle::refresh() {
  QString option = QString::fromStdString(params.get("OPKRMapboxStyleSelect"));
  if (option == "0") {
    btn1.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #00A12E;
    )");
    btn2.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
    )");
    btn3.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
    )");
  } else if (option == "1") {
    btn1.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
    )");
    btn2.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #00A12E;
    )");
    btn3.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
    )");
  } else {
    btn1.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
    )");
    btn2.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
    )");
    btn3.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #00A12E;
    )");
  }
}

RESCountatStandstill::RESCountatStandstill() : AbstractControl("RES Count at Standstill", "Comma Default: 25, this value cannot be acceptable at some cars. So adjust the number if you want to. It generates RES CAN messages when leadcar is moving. If departure is failed, increase the number. In opposite, if CAN error occurs, decrease the number.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("RESCountatStandstill"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 1 ) {
      value = 1;
    }
    QString values = QString::number(value);
    params.put("RESCountatStandstill", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("RESCountatStandstill"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 50 ) {
      value = 50;
    }
    QString values = QString::number(value);
    params.put("RESCountatStandstill", values.toStdString());
    refresh();
  });
  refresh();
}

void RESCountatStandstill::refresh() {
  label.setText(QString::fromStdString(params.get("RESCountatStandstill")));
  btnminus.setText("-");
  btnplus.setText("+");
}

SpeedLimitSignType::SpeedLimitSignType() : AbstractControl("SafetyCam SignType", "Select SafetyCam SignType (Circle/Rectangle)", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrSpeedLimitSignType"));
    int value = str.toInt();
    value = value - 1;
    if (value <= -1 ) {
      value = 1;
    }
    QString values = QString::number(value);
    params.put("OpkrSpeedLimitSignType", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("OpkrSpeedLimitSignType"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 2 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("OpkrSpeedLimitSignType", values.toStdString());
    refresh();
  });
  refresh();
}

void SpeedLimitSignType::refresh() {
  QString option = QString::fromStdString(params.get("OpkrSpeedLimitSignType"));
  if (option == "0") {
    label.setText(QString::fromStdString("Circle"));
  } else {
    label.setText(QString::fromStdString("Rectangle"));
  }
  btnminus.setText("◀");
  btnplus.setText("▶");
}

RadarLongHelperOption::RadarLongHelperOption() : AbstractControl("Radar Long Assist", "Vision Only, Vision+Radar, Radar Only", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("RadarLongHelper"));
    int value = str.toInt();
    value = value - 1;
    if (value <= -1 ) {
      value = 2;
    }
    QString values = QString::number(value);
    params.put("RadarLongHelper", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("RadarLongHelper"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 3 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("RadarLongHelper", values.toStdString());
    refresh();
  });
  refresh();
}

void RadarLongHelperOption::refresh() {
  QString option = QString::fromStdString(params.get("RadarLongHelper"));
  if (option == "0") {
    label.setText(QString::fromStdString("Vision Only"));
  } else if (option == "1") {
    label.setText(QString::fromStdString("Vision+Radar"));
  } else {
    label.setText(QString::fromStdString("Radar Only"));
  }
  btnminus.setText("◀");
  btnplus.setText("▶");
}

CurvDecelSelect::CurvDecelSelect() : AbstractControl("Curv Decel Option", "None, Vision+OSM, Vision Only, OSM Only", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("CurvDecelOption"));
    int value = str.toInt();
    value = value - 1;
    if (value <= -1 ) {
      value = 3;
    }
    QString values = QString::number(value);
    params.put("CurvDecelOption", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("CurvDecelOption"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 4 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("CurvDecelOption", values.toStdString());
    refresh();
  });
  refresh();
}

void CurvDecelSelect::refresh() {
  QString option = QString::fromStdString(params.get("CurvDecelOption"));
  if (option == "0") {
    label.setText(QString::fromStdString("None"));
  } else if (option == "1") {
    label.setText(QString::fromStdString("Vision+OSM"));
  } else if (option == "2") {
    label.setText(QString::fromStdString("Vision Only"));
  } else {
    label.setText(QString::fromStdString("OSM Only"));
  }
  btnminus.setText("◀");
  btnplus.setText("▶");
}

AutoRESDelay::AutoRESDelay() : AbstractControl("AutoRES Delay(sec)", "Give delay time to trigger for AutoRES while driving.", "../assets/offroad/icon_shell.png") {

  label.setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label.setStyleSheet("color: #e0e879");
  hlayout->addWidget(&label);

  btnminus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnplus.setStyleSheet(R"(
    padding: 0;
    border-radius: 50px;
    font-size: 35px;
    font-weight: 500;
    color: #E4E4E4;
    background-color: #393939;
  )");
  btnminus.setFixedSize(150, 100);
  btnplus.setFixedSize(150, 100);
  hlayout->addWidget(&btnminus);
  hlayout->addWidget(&btnplus);

  QObject::connect(&btnminus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("AutoRESDelay"));
    int value = str.toInt();
    value = value - 1;
    if (value <= 0 ) {
      value = 0;
    }
    QString values = QString::number(value);
    params.put("AutoRESDelay", values.toStdString());
    refresh();
  });
  
  QObject::connect(&btnplus, &QPushButton::clicked, [=]() {
    auto str = QString::fromStdString(params.get("AutoRESDelay"));
    int value = str.toInt();
    value = value + 1;
    if (value >= 10 ) {
      value = 10;
    }
    QString values = QString::number(value);
    params.put("AutoRESDelay", values.toStdString());
    refresh();
  });
  refresh();
}

void AutoRESDelay::refresh() {
  QString option = QString::fromStdString(params.get("AutoRESDelay"));
  if (option == "0") {
    label.setText(QString::fromStdString("No Delay"));
  } else {
    label.setText(QString::fromStdString(params.get("AutoRESDelay")));
  }
  btnminus.setText("-");
  btnplus.setText("+");
}