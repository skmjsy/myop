#pragma once

#include <QStackedLayout>
#include <QWidget>

#include "selfdrive/ui/qt/widgets/cameraview.h"
#include "selfdrive/ui/ui.h"

#include <QTimer>
#include "selfdrive/ui/qt/screenrecorder/screenrecorder.h"

// ***** onroad widgets *****

class OnroadAlerts : public QWidget {
  Q_OBJECT

public:
  OnroadAlerts(QWidget *parent = 0) : QWidget(parent) {};
  void updateAlert(const Alert &a, const QColor &color);

protected:
  void paintEvent(QPaintEvent*) override;

private:
  QColor bg;
  Alert alert = {};
};

// container window for the NVG UI
class NvgWindow : public CameraViewWidget {
  Q_OBJECT

  Q_PROPERTY(QString speed MEMBER speed NOTIFY valueChanged);
  Q_PROPERTY(QString speedUnit MEMBER speedUnit NOTIFY valueChanged);
  Q_PROPERTY(QString maxSpeed MEMBER maxSpeed NOTIFY valueChanged);
  Q_PROPERTY(bool is_cruise_set MEMBER is_cruise_set NOTIFY valueChanged);
  Q_PROPERTY(bool engageable MEMBER engageable NOTIFY valueChanged);
  Q_PROPERTY(bool dmActive MEMBER dmActive NOTIFY valueChanged);
  Q_PROPERTY(bool hideDM MEMBER hideDM NOTIFY valueChanged);
  Q_PROPERTY(int status MEMBER status NOTIFY valueChanged);

  Q_PROPERTY(bool showHowAlert MEMBER showHowAlert NOTIFY valueChanged);
  Q_PROPERTY(bool howWarning MEMBER howWarning NOTIFY valueChanged);

  Q_PROPERTY(bool showVTC MEMBER showVTC NOTIFY valueChanged);
  Q_PROPERTY(QString vtcSpeed MEMBER vtcSpeed NOTIFY valueChanged);
  Q_PROPERTY(QColor vtcColor MEMBER vtcColor NOTIFY valueChanged);
  Q_PROPERTY(bool showDebugUI MEMBER showDebugUI NOTIFY valueChanged);

  Q_PROPERTY(QString roadName MEMBER roadName NOTIFY valueChanged);

  Q_PROPERTY(bool showSpeedLimit MEMBER showSpeedLimit NOTIFY valueChanged);
  Q_PROPERTY(QString speedLimit MEMBER speedLimit NOTIFY valueChanged);
  Q_PROPERTY(QString slcSubText MEMBER slcSubText NOTIFY valueChanged);
  Q_PROPERTY(float slcSubTextSize MEMBER slcSubTextSize NOTIFY valueChanged);
  Q_PROPERTY(bool mapSourcedSpeedLimit MEMBER mapSourcedSpeedLimit NOTIFY valueChanged);
  Q_PROPERTY(bool slcActive MEMBER slcActive NOTIFY valueChanged);

  Q_PROPERTY(bool showTurnSpeedLimit MEMBER showTurnSpeedLimit NOTIFY valueChanged);
  Q_PROPERTY(QString turnSpeedLimit MEMBER turnSpeedLimit NOTIFY valueChanged);
  Q_PROPERTY(QString tscSubText MEMBER tscSubText NOTIFY valueChanged);
  Q_PROPERTY(bool tscActive MEMBER tscActive NOTIFY valueChanged);
  Q_PROPERTY(int curveSign MEMBER curveSign NOTIFY valueChanged);

public:
  explicit NvgWindow(VisionStreamType type, QWidget* parent = 0) : CameraViewWidget("camerad", type, true, parent) {}

protected:
  void paintGL() override;
  void initializeGL() override;
  void showEvent(QShowEvent *event) override;
  void updateFrameMat(int w, int h) override;
  void drawLaneLines(QPainter &painter, const UIScene &scene);
  void drawLead(QPainter &painter, const cereal::ModelDataV2::LeadDataV3::Reader &lead_data, const QPointF &vd, bool is_radar);
  inline QColor redColor(int alpha = 255) { return QColor(201, 34, 49, alpha); }
  double prev_draw_t = 0;

  // neokii
  void drawIcon(QPainter &p, int x, int y, QPixmap &img, QBrush bg, float opacity);
  void drawText(QPainter &p, int x, int y, const QString &text, int alpha = 255);
  void drawText2(QPainter &p, int x, int y, int flags, const QString &text, const QColor& color);
  void drawTextWithColor(QPainter &p, int x, int y, const QString &text, QColor& color);
  void paintEvent(QPaintEvent *event) override;

  //sunny
  void drawCenteredText(QPainter &p, int x, int y, const QString &text, QColor color);
  void drawVisionTurnControllerUI(QPainter &p, int x, int y, int size, const QColor &color, const QString &speed,
                                  int alpha);
  void drawCircle(QPainter &p, int x, int y, int r, QBrush bg);
  void drawSpeedSign(QPainter &p, QRect rc, const QString &speed, const QString &sub_text, int subtext_size,
                     bool is_map_sourced, bool is_active);
  void drawTrunSpeedSign(QPainter &p, QRect rc, const QString &speed, const QString &sub_text, int curv_sign,
                         bool is_active);

  const int radius = 192;
  const int img_size = (radius / 2) * 1.5;

  // neokii
  QPixmap ic_brake;
  QPixmap ic_autohold_warning;
  QPixmap ic_autohold_active;
  QPixmap ic_nda;
  QPixmap ic_hda;
  QPixmap ic_tire_pressure;
  QPixmap ic_turn_signal_l;
  QPixmap ic_turn_signal_r;
  QPixmap ic_satellite;
  QPixmap ic_up;
  //sunny
  QPixmap left_img;
  QPixmap right_img;
  QPixmap map_img;
  const int subsign_img_size = 35;
  QString speed;
  QString speedUnit;
  QString maxSpeed;
  bool is_cruise_set = false;
  bool engageable = false;
  bool dmActive = false;
  bool hideDM = false;
  int status = STATUS_DISENGAGED;

  bool showHowAlert = false;
  bool howWarning = false;

  bool showVTC = false;
  QString vtcSpeed;
  QColor vtcColor;
  bool showDebugUI = false;

  QString roadName;

  bool showSpeedLimit = false;
  QString speedLimit;
  QString slcSubText;
  float slcSubTextSize = 0.0;
  bool mapSourcedSpeedLimit = false;
  bool slcActive = false;

  bool showTurnSpeedLimit = false;
  QString turnSpeedLimit;
  QString tscSubText;
  bool tscActive = false;
  int curveSign = 0;

  void drawMaxSpeed(QPainter &p);
  void drawSpeed(QPainter &p);
  void drawBottomIcons(QPainter &p);
  void drawSpeedLimit(QPainter &p);
  void drawSpeedLimitMapBox(QPainter &p);
  void drawTurnSignals(QPainter &p);
  void drawGpsStatus(QPainter &p);
  void drawDebugText(QPainter &p);
  void drawHud(QPainter &p);
  //opkr
  void drawDeviceInfo(QPainter &p);
  void drawCompass(QPainter &p);

  signals:
  void valueChanged();
};

// container for all onroad widgets
class OnroadWindow : public QWidget {
  Q_OBJECT

public:
  OnroadWindow(QWidget* parent = 0);
  bool isMapVisible() const { return map && map->isVisible(); }

protected:
  void mousePressEvent(QMouseEvent* e) override;
  void mouseReleaseEvent(QMouseEvent* e) override;

  void paintEvent(QPaintEvent *event) override;

private:
  OnroadAlerts *alerts;
  NvgWindow *nvg;
  QColor bg = bg_colors[STATUS_DISENGAGED];
  QWidget *map = nullptr;
  QHBoxLayout* split;

  QVBoxLayout* vsplit;

private:
  ScreenRecoder* recorder;
  std::shared_ptr<QTimer> record_timer;
  QPoint startPos;

signals:
  void offroadTransitionSignal(bool offroad);

private slots:
  void offroadTransition(bool offroad);
  void updateState(const UIState &s);
};
