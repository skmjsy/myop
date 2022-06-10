#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QWidget>
#include <QProcess>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QAbstractSocket>

#include "selfdrive/hardware/hw.h"
#include "selfdrive/ui/qt/util.h"
#include "selfdrive/ui/qt/qt_window.h"
#include "selfdrive/ui/qt/widgets/scrollview.h"

int main(int argc, char *argv[]) {
  initApp();
  QApplication a(argc, argv);
  QWidget window;
  setMainWindow(&window);

  QGridLayout *main_layout = new QGridLayout(&window);
  main_layout->setMargin(50);

  QLabel *label = new QLabel(argv[1]);
  label->setWordWrap(true);
  label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
  ScrollView *scroll = new ScrollView(label);
  scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  main_layout->addWidget(scroll, 0, 0, Qt::AlignTop);

  // Scroll to the bottom
  QObject::connect(scroll->verticalScrollBar(), &QAbstractSlider::rangeChanged, [=]() {
    scroll->verticalScrollBar()->setValue(scroll->verticalScrollBar()->maximum());
  });

  QPushButton *btn = new QPushButton();
#ifdef __aarch64__
  QPushButton *btn2 = new QPushButton();
  QLabel *label2 = new QLabel();
  QString device_ip = "---";
  const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
  for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
    if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
      device_ip = address.toString();
  }
  label2->setText(device_ip);
  label2->setStyleSheet("color: #e0e879");
  main_layout->addWidget(label2, 0, 0, Qt::AlignRight | Qt::AlignTop);
  btn->setText("Git Pull");
  btn2->setText("MixPlorer");
  QObject::connect(btn, &QPushButton::clicked, [=]() {
    QProcess::execute("/data/openpilot/selfdrive/assets/addon/script/gitpull.sh");
    //Hardware::reboot();
  });
  QObject::connect(btn2, &QPushButton::clicked, [=]() {
    QProcess::execute("/data/openpilot/selfdrive/assets/addon/script/run_mixplorer.sh");
    btn2->setEnabled(false);
  });
  main_layout->addWidget(btn2, 0, 0, Qt::AlignLeft | Qt::AlignBottom);
#else
  btn->setText("Exit");
  QObject::connect(btn, &QPushButton::clicked, &a, &QApplication::quit);
#endif
  main_layout->addWidget(btn, 0, 0, Qt::AlignRight | Qt::AlignBottom);

  window.setStyleSheet(R"(
    * {
      outline: none;
      color: white;
      background-color: black;
      font-size: 60px;
    }
    QPushButton {
      padding: 50px;
      padding-right: 100px;
      padding-left: 100px;
      border: 2px solid white;
      border-radius: 20px;
      margin-right: 40px;
    }
  )");

  return a.exec();
}
