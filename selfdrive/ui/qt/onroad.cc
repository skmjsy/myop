#include "selfdrive/ui/qt/onroad.h"

#include <cmath>

#include <QDebug>
#include <QSound>
#include <QMouseEvent>

#include "selfdrive/common/timing.h"
#include "selfdrive/ui/qt/util.h"

#include <iomanip>
#include <sstream>
#include "selfdrive/common/params.h"

#ifdef ENABLE_MAPS
#include "selfdrive/ui/qt/maps/map.h"
//#include "selfdrive/ui/qt/maps/map_helpers.h"
#endif

OnroadWindow::OnroadWindow(QWidget *parent) : QWidget(parent) {

  QVBoxLayout *main_layout  = new QVBoxLayout(this);
  main_layout->setMargin(bdr_s);

  QStackedLayout *stacked_layout = new QStackedLayout;
  stacked_layout->setStackingMode(QStackedLayout::StackAll);
  main_layout->addLayout(stacked_layout);

  nvg = new NvgWindow(VISION_STREAM_RGB_BACK, this);

  QWidget * split_wrapper = new QWidget;

  if (Params().getBool("EnableStealthEon")) {
    vsplit = new QVBoxLayout(split_wrapper);
    vsplit->setContentsMargins(0, 0, 0, 0);
    vsplit->setSpacing(0);
    vsplit->addWidget(nvg);
  }
  else {
    split = new QHBoxLayout(split_wrapper);
    split->setContentsMargins(0, 0, 0, 0);
    split->setSpacing(0);
    split->addWidget(nvg);
  }

  stacked_layout->addWidget(split_wrapper);

  alerts = new OnroadAlerts(this);
  alerts->setAttribute(Qt::WA_TransparentForMouseEvents, true);
  stacked_layout->addWidget(alerts);

  if (Params().getBool("EnableStealthEon")) {
    setFixedWidth(split_wrapper->width() * 2 - 200);
  }

  // setup stacking order
  alerts->raise();

  setAttribute(Qt::WA_OpaquePaintEvent);
  QObject::connect(uiState(), &UIState::uiUpdate, this, &OnroadWindow::updateState);
  QObject::connect(uiState(), &UIState::offroadTransition, this, &OnroadWindow::offroadTransition);
  QObject::connect(this, &OnroadWindow::offroadTransitionSignal, this, &OnroadWindow::offroadTransition);


  // screen recoder - neokii
  if (!Params().getBool("EnableStealthEon") && !Params().getBool("OpkrDrivingRecord")) {
    record_timer = std::make_shared<QTimer>();
    QObject::connect(record_timer.get(), &QTimer::timeout, [=]() {
      if(recorder) {
        recorder->update_screen();
      }
    });
    record_timer->start(1000/UI_FREQ);

    QWidget* recorder_widget = new QWidget(this);
    QVBoxLayout * recorder_layout = new QVBoxLayout (recorder_widget);
    recorder_layout->setMargin(35);
    recorder = new ScreenRecoder(this);
    recorder_layout->addWidget(recorder);
    recorder_layout->setAlignment(recorder, Qt::AlignRight | Qt::AlignBottom);

    stacked_layout->addWidget(recorder_widget);
    recorder_widget->raise();
    alerts->raise();
  }

}

//opkr
void OnroadWindow::updateState(const UIState &s) {
  QColor bgColor = bg_colors[s.status];
  Alert alert = Alert::get(*(s.sm), s.scene.started_frame);
  if (s.sm->updated("controlsState") || !alert.equal({})) {
    if (alert.type == "controlsUnresponsive") {
      bgColor = bg_colors[STATUS_ALERT];
    }
    if (!Params().getBool("IsOpenpilotViewEnabled")) {
        alerts->updateAlert(alert, bgColor);
    }
  }

  if (map != nullptr && !map->isVisible()) {
    bool sidebarVisible = geometry().x() > 0;
    map->setVisible(!sidebarVisible && !s.show_debug);
  }

  if (bg != bgColor) {
    // repaint border
    bg = bgColor;
    update();
  }
}

void OnroadWindow::mouseReleaseEvent(QMouseEvent* e) {
#ifdef QCOM2
  // neokii
  QPoint endPos = e->pos();
  int dx = endPos.x() - startPos.x();
  int dy = endPos.y() - startPos.y();
  if(std::abs(dx) > 250 || std::abs(dy) > 200) {
    if(std::abs(dx) < std::abs(dy)) {
      if(dy < 0) { // upward
        Params().remove("CalibrationParams");
        Params().remove("LiveParameters");
        QTimer::singleShot(1500, []() {
          Params().putBool("SoftRestartTriggered", true);
        });
        QSound::play("../assets/sounds/reset_calibration.wav");
      }
      else { // downward
        QTimer::singleShot(500, []() {
          Params().putBool("SoftRestartTriggered", true);
        });
      }
    }
    else if(std::abs(dx) > std::abs(dy)) {
      if(dx < 0) { // right to left
        if(recorder)
          recorder->toggle();
      }
      else { // left to right
        if(recorder)
          recorder->toggle();
      }
    }
    return;
  }
  if (map != nullptr) {
    bool sidebarVisible = geometry().x() > 0;
    map->setVisible(!sidebarVisible && !map->isVisible());
  }

  // propagation event to parent(HomeWindow)
  QWidget::mouseReleaseEvent(e);
#endif
}

void OnroadWindow::mousePressEvent(QMouseEvent* e) {
  startPos = e->pos();

  UIState *s = uiState();

  if (map != nullptr) {
    bool sidebarVisible = geometry().x() > 0;
    //map->setVisible(!sidebarVisible && !map->isVisible() && !s->show_debug);
    map->setVisible(!sidebarVisible && !map->isVisible());
  }
  //propagation event to parent(HomeWindow)
  QWidget::mouseReleaseEvent(e);
}


void OnroadWindow::offroadTransition(bool offroad) {
#ifdef ENABLE_MAPS
  if (!offroad) {
    QString token = QString::fromStdString(Params().get("dp_mapbox_token_sk"));
    if (map == nullptr && !token.isEmpty() && Params().getBool("MapboxEnabled") && !Params().getBool("EnableStealthEon")) {
      QMapboxGLSettings settings;

      if (!Hardware::PC()) {
        settings.setCacheDatabasePath("/data/mbgl-cache.db");
      }
      settings.setApiBaseUrl(MAPS_HOST);
      settings.setCacheDatabaseMaximumSize(20 * 1024 * 1024);
      settings.setAccessToken(token.trimmed());

      MapWindow * m = new MapWindow(settings);
      QObject::connect(this, &OnroadWindow::offroadTransitionSignal, m, &MapWindow::offroadTransition);

      if (Params().getBool("EnableStealthEon")) {
        vsplit->addWidget(m);
      }
      else {
        m->setFixedWidth(topWidget(this)->width() / 2);
        split->addWidget(m, 0, Qt::AlignRight);
      }

      map = m;
    }
  }
#endif

  alerts->updateAlert({}, bg);

  // update stream type
  bool wide_cam = Hardware::TICI() && Params().getBool("EnableWideCamera");
  nvg->setStreamType(wide_cam ? VISION_STREAM_RGB_WIDE : VISION_STREAM_RGB_BACK);

#ifdef QCOM2
  if(offroad && recorder) {
    recorder->stop(false);
  }
#endif
}

void OnroadWindow::paintEvent(QPaintEvent *event) {
  QPainter p(this);
  p.fillRect(rect(), QColor(bg.red(), bg.green(), bg.blue(), 255));
}

// ***** onroad widgets *****

// OnroadAlerts
void OnroadAlerts::updateAlert(const Alert &a, const QColor &color) {
  if (!alert.equal(a) || color != bg) {
    alert = a;
    bg = color;
    update();
  }
}

void OnroadAlerts::paintEvent(QPaintEvent *event) {
  if (alert.size == cereal::ControlsState::AlertSize::NONE) {
    return;
  }
  static std::map<cereal::ControlsState::AlertSize, const int> alert_sizes = {
    {cereal::ControlsState::AlertSize::SMALL, 271},
    {cereal::ControlsState::AlertSize::MID, 420},
    {cereal::ControlsState::AlertSize::FULL, height()},
  };
  int h = alert_sizes[alert.size];
  QRect r = QRect(0, height() - h, width(), h);

  QPainter p(this);

  // draw background + gradient
  p.setPen(Qt::NoPen);
  p.setCompositionMode(QPainter::CompositionMode_SourceOver);

  p.setBrush(QBrush(bg));
  p.drawRect(r);

  QLinearGradient g(0, r.y(), 0, r.bottom());
  g.setColorAt(0, QColor::fromRgbF(0, 0, 0, 0.05));
  g.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0.35));

  p.setCompositionMode(QPainter::CompositionMode_DestinationOver);
  p.setBrush(QBrush(g));
  p.fillRect(r, g);
  p.setCompositionMode(QPainter::CompositionMode_SourceOver);

  // text
  const QPoint c = r.center();
  p.setPen(QColor(0xff, 0xff, 0xff));
  p.setRenderHint(QPainter::TextAntialiasing);
  if (alert.size == cereal::ControlsState::AlertSize::SMALL) {
    configFont(p, "Open Sans", 74, "SemiBold");
    p.drawText(r, Qt::AlignCenter, alert.text1);
  } else if (alert.size == cereal::ControlsState::AlertSize::MID) {
    configFont(p, "Open Sans", 88, "Bold");
    p.drawText(QRect(0, c.y() - 125, width(), 150), Qt::AlignHCenter | Qt::AlignTop, alert.text1);
    configFont(p, "Open Sans", 66, "Regular");
    p.drawText(QRect(0, c.y() + 21, width(), 90), Qt::AlignHCenter, alert.text2);
  } else if (alert.size == cereal::ControlsState::AlertSize::FULL) {
    bool l = alert.text1.length() > 15;
    configFont(p, "Open Sans", l ? 132 : 177, "Bold");
    p.drawText(QRect(0, r.y() + (l ? 240 : 270), width(), 600), Qt::AlignHCenter | Qt::TextWordWrap, alert.text1);
    configFont(p, "Open Sans", 88, "Regular");
    p.drawText(QRect(0, r.height() - (l ? 361 : 420), width(), 300), Qt::AlignHCenter | Qt::TextWordWrap, alert.text2);
  }
}

// NvgWindow
void NvgWindow::initializeGL() {
  CameraViewWidget::initializeGL();
  qInfo() << "OpenGL version:" << QString((const char*)glGetString(GL_VERSION));
  qInfo() << "OpenGL vendor:" << QString((const char*)glGetString(GL_VENDOR));
  qInfo() << "OpenGL renderer:" << QString((const char*)glGetString(GL_RENDERER));
  qInfo() << "OpenGL language version:" << QString((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

  prev_draw_t = millis_since_boot();
  setBackgroundColor(bg_colors[STATUS_DISENGAGED]);

  // neokii
  ic_brake = QPixmap("../assets/images/img_brake_disc.png").scaled(img_size, img_size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  ic_autohold_warning = QPixmap("../assets/images/img_autohold_warning.png").scaled(img_size, img_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  ic_autohold_active = QPixmap("../assets/images/img_autohold_active.png").scaled(img_size, img_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  ic_nda = QPixmap("../assets/images/img_nda.png");
  ic_hda = QPixmap("../assets/images/hda.png");
  ic_tire_pressure = QPixmap("../assets/images/img_tire_pressure.png");
  ic_turn_signal_l = QPixmap("../assets/images/turn_signal_l.png");
  ic_turn_signal_r = QPixmap("../assets/images/turn_signal_r.png");
  ic_satellite = QPixmap("../assets/images/satellite.png");
  ic_up = QPixmap("../assets/images/up.png");

  //sunny
  map_img = QPixmap("../assets/img_world_icon.png").scaled(subsign_img_size, subsign_img_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  left_img = QPixmap("../assets/img_turn_left_icon.png").scaled(subsign_img_size, subsign_img_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  right_img = QPixmap("../assets/img_turn_right_icon.png").scaled(subsign_img_size, subsign_img_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

}

void NvgWindow::updateFrameMat(int w, int h) {
  CameraViewWidget::updateFrameMat(w, h);

  UIState *s = uiState();
  s->fb_w = w;
  s->fb_h = h;
  auto intrinsic_matrix = s->wide_camera ? ecam_intrinsic_matrix : fcam_intrinsic_matrix;
  float zoom = ZOOM / intrinsic_matrix.v[0];
  if (s->wide_camera) {
    zoom *= 0.5;
  }
  // Apply transformation such that video pixel coordinates match video
  // 1) Put (0, 0) in the middle of the video
  // 2) Apply same scaling as video
  // 3) Put (0, 0) in top left corner of video
  s->car_space_transform.reset();
  s->car_space_transform.translate(w / 2, h / 2 + y_offset)
      .scale(zoom, zoom)
      .translate(-intrinsic_matrix.v[2], -intrinsic_matrix.v[5]);
}

void NvgWindow::drawLaneLines(QPainter &painter, const UIScene &scene) {
  if (!scene.end_to_end) {
    // lanelines
    for (int i = 0; i < std::size(scene.lane_line_vertices); ++i) {
      painter.setBrush(QColor::fromRgbF(1.0, 1.0, 1.0, scene.lane_line_probs[i]));
      painter.drawPolygon(scene.lane_line_vertices[i].v, scene.lane_line_vertices[i].cnt);
    }
    // road edges
    for (int i = 0; i < std::size(scene.road_edge_vertices); ++i) {
      painter.setBrush(QColor::fromRgbF(1.0, 0, 0, std::clamp<float>(1.0 - scene.road_edge_stds[i], 0.0, 1.0)));
      painter.drawPolygon(scene.road_edge_vertices[i].v, scene.road_edge_vertices[i].cnt);
    }
  }
  // paint path
  QLinearGradient bg(0, height(), 0, height() / 4);
  bg.setColorAt(0, scene.end_to_end ? redColor(200) : QColor(255, 255, 255, 200));
  bg.setColorAt(1, scene.end_to_end ? redColor(0) : QColor(255, 255, 255, 0));
  painter.setBrush(bg);
  painter.drawPolygon(scene.track_vertices.v, scene.track_vertices.cnt);
}

void NvgWindow::drawLead(QPainter &painter, const cereal::ModelDataV2::LeadDataV3::Reader &lead_data, const QPointF &vd, bool is_radar) {
  const float speedBuff = 10.;
  const float leadBuff = 40.;
  const float d_rel = lead_data.getX()[0];
  const float v_rel = lead_data.getV()[0];

  float fillAlpha = 0;
  if (d_rel < leadBuff) {
    fillAlpha = 255 * (1.0 - (d_rel / leadBuff));
    if (v_rel < 0) {
      fillAlpha += 255 * (-1 * (v_rel / speedBuff));
    }
    fillAlpha = (int)(fmin(fillAlpha, 255));
  }

  float sz = std::clamp((25 * 30) / (d_rel / 3 + 30), 15.0f, 30.0f) * 2.35;
  float x = std::clamp((float)vd.x(), 0.f, width() - sz / 2);
  float y = std::fmin(height() - sz * .6, (float)vd.y());

  float g_xo = sz / 5;
  float g_yo = sz / 10;

  QPointF glow[] = {{x + (sz * 1.35) + g_xo, y + sz + g_yo}, {x, y - g_xo}, {x - (sz * 1.35) - g_xo, y + sz + g_yo}};
  painter.setBrush(is_radar ? QColor(86, 121, 216, 255) : QColor(218, 202, 37, 255));
  painter.drawPolygon(glow, std::size(glow));

  // chevron
  QPointF chevron[] = {{x + (sz * 1.25), y + sz}, {x, y}, {x - (sz * 1.25), y + sz}};
  painter.setBrush(redColor(fillAlpha));
  painter.drawPolygon(chevron, std::size(chevron));
}

void NvgWindow::paintGL() {
}

void NvgWindow::paintEvent(QPaintEvent *event) {
  QPainter p;
  p.begin(this);

  p.beginNativePainting();
  CameraViewWidget::paintGL();
  p.endNativePainting();

  UIState *s = uiState();
  if (s->worldObjectsVisible()) {
    drawHud(p);
  }

  p.end();

  double cur_draw_t = millis_since_boot();
  double dt = cur_draw_t - prev_draw_t;
  if (dt > 66) {
    // warn on sub 15fps
    LOGW("slow frame time: %.2f", dt);
  }
  prev_draw_t = cur_draw_t;
}

void NvgWindow::showEvent(QShowEvent *event) {
  CameraViewWidget::showEvent(event);

  ui_update_params(uiState());
  prev_draw_t = millis_since_boot();
}

void NvgWindow::drawText(QPainter &p, int x, int y, const QString &text, int alpha) {
  QFontMetrics fm(p.font());
  QRect init_rect = fm.boundingRect(text);
  QRect real_rect = fm.boundingRect(init_rect, 0, text);
  real_rect.moveCenter({x, y - real_rect.height() / 2});

  p.setPen(QColor(0xff, 0xff, 0xff, alpha));
  p.drawText(real_rect.x(), real_rect.bottom(), text);
}

void NvgWindow::drawTextWithColor(QPainter &p, int x, int y, const QString &text, QColor& color) {
  QFontMetrics fm(p.font());
  QRect init_rect = fm.boundingRect(text);
  QRect real_rect = fm.boundingRect(init_rect, 0, text);
  real_rect.moveCenter({x, y - real_rect.height() / 2});

  p.setPen(color);
  p.drawText(real_rect.x(), real_rect.bottom(), text);
}

void NvgWindow::drawIcon(QPainter &p, int x, int y, QPixmap &img, QBrush bg, float opacity) {
  p.setPen(Qt::NoPen);
  p.setBrush(bg);
  p.drawEllipse(x - radius / 2, y - radius / 2, radius, radius);
  p.setOpacity(opacity);
  p.drawPixmap(x - img_size / 2, y - img_size / 2, img_size, img_size, img);
}

void NvgWindow::drawText2(QPainter &p, int x, int y, int flags, const QString &text, const QColor& color) {
  QFontMetrics fm(p.font());
  QRect rect = fm.boundingRect(text);
  p.setPen(color);
  p.drawText(QRect(x, y, rect.width(), rect.height()), flags, text);
}

void NvgWindow::drawHud(QPainter &p) {

  p.setRenderHint(QPainter::Antialiasing);
  p.setPen(Qt::NoPen);
  p.setOpacity(1.);

  // Header gradient
  QLinearGradient bg(0, header_h - (header_h / 2.5), 0, header_h);
  bg.setColorAt(0, QColor::fromRgbF(0, 0, 0, 0.45));
  bg.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));
  p.fillRect(0, 0, width(), header_h, bg);

  UIState *s = uiState();

  const SubMaster &sm = *(s->sm);

  drawLaneLines(p, s->scene);

  auto leads = sm["modelV2"].getModelV2().getLeadsV3();
  if (leads[0].getProb() > .5) {
    drawLead(p, leads[0], s->scene.lead_vertices[0], s->scene.lead_radar[0]);
  }
  if (leads[1].getProb() > .5 && (std::abs(leads[1].getX()[0] - leads[0].getX()[0]) > 3.0)) {
    drawLead(p, leads[1], s->scene.lead_vertices[1], s->scene.lead_radar[1]);
  }

  drawMaxSpeed(p);
  drawSpeed(p);
  //drawSpeedLimit(p);
  drawSpeedLimitMapBox(p);
  drawTurnSignals(p);
  drawGpsStatus(p);
  //drawDeviceInfo(p);
  drawCompass(p);

  if(s->show_debug && width() > 1200)
    drawDebugText(p);

  const auto controls_state = sm["controlsState"].getControlsState();
  const auto car_params = sm["carParams"].getCarParams();
  const auto live_params = sm["liveParameters"].getLiveParameters();
  auto longitudinal_plan = sm["longitudinalPlan"].getLongitudinalPlan();
  auto live_map_data = sm["liveMapData"].getLiveMapData();

  const auto scc_smoother = sm["carControl"].getCarControl().getSccSmoother();
  bool is_metric = s->scene.is_metric;
  bool long_control = scc_smoother.getLongControl();

  // kph
  float applyMaxSpeed = scc_smoother.getApplyMaxSpeed();
  float cruiseMaxSpeed = scc_smoother.getCruiseMaxSpeed();

  bool is_cruise_set = (cruiseMaxSpeed > 0 && cruiseMaxSpeed < 255);

  int mdps_bus = car_params.getMdpsBus();
  int scc_bus = car_params.getSccBus();
  int longControlState = (int)controls_state.getLongControlState();
  auto lead_radar = sm["radarState"].getRadarState().getLeadOne();
  auto lead_one = sm["modelV2"].getModelV2().getLeadsV3()[0];
  float radar_dist = lead_radar.getStatus() && lead_radar.getRadar() ? lead_radar.getDRel() : 0;
  float vision_dist = lead_one.getProb() > .5 ? (lead_one.getX()[0] - 1.5) : 0;

  const float speed_limit = live_map_data.getSpeedLimit() * (s->scene.is_metric ? MS_TO_KPH : MS_TO_MPH);
  const float speed_limit_ahead = live_map_data.getSpeedLimitAhead() * (s->scene.is_metric ? MS_TO_KPH : MS_TO_MPH);
  const float turn_speed_limit = live_map_data.getTurnSpeedLimit() * (s->scene.is_metric ? MS_TO_KPH : MS_TO_MPH);
  const float vision_turn_speed = longitudinal_plan.getVisionTurnSpeed() * (s->scene.is_metric ? MS_TO_KPH : MS_TO_MPH);
  const int sl_distance = int(live_map_data.getSpeedLimitAheadDistance() * (s->scene.is_metric ? MS_TO_KPH : MS_TO_MPH) / 10.0) * 10;
  const QString sl_distance_str(QString::number(sl_distance) + (s->scene.is_metric ? "m" : "f"));

  QString infoText;
  infoText.sprintf("AO(%.2f/%.2f) SR(%.2f) SRC(%.2f) SAD(%.2f) MDPS(%d) SCC(%d) LAD(%.2f/%.2f) SCC(%.2f/%.2f/%.2f)",
                      live_params.getAngleOffsetDeg(),
                      live_params.getAngleOffsetAverageDeg(),
                      controls_state.getSteerRatio(),
                      controls_state.getSteerRateCost(),
                      controls_state.getSteerActuatorDelay(),
                      mdps_bus, scc_bus,
                      controls_state.getLongitudinalActuatorDelayLowerBound(),
                      controls_state.getLongitudinalActuatorDelayUpperBound(),
                      controls_state.getSccGasFactor(),
                      controls_state.getSccBrakeFactor(),
                      controls_state.getSccCurvatureFactor()
                      );

  QString infoText2;
  infoText2.sprintf("Lead(%.1f/%.1f/%.1f) SPD(%.1f/%.1f/%.1f) CPU(%d/%d/%d) LS(%d/%d)",
                      radar_dist, vision_dist, (radar_dist - vision_dist),
                      speed_limit, turn_speed_limit, vision_turn_speed,
                      (int)s->scene.cpuTemp, (int)s->scene.cpuPerc, (int)s->scene.ambientTemp,
                      (int)s->scene.getbrightness, (int)s->scene.setbrightness
                      );                    

  QString infoText3;
  infoText3.sprintf("Lead(%.1f/%.1f/%.1f) SPD(%.1f/%.1f/%.1f) CPU(%d/%d/%d)",
                      radar_dist, vision_dist, (radar_dist - vision_dist),
                      speed_limit, turn_speed_limit, vision_turn_speed,
                      (int)s->scene.cpuTemp, (int)s->scene.cpuPerc, (int)s->scene.ambientTemp
                      );   

  // info
  configFont(p, "Open Sans", 34, "Regular");
  p.setPen(QColor(0xff, 0xff, 0xff, 200));
  //p.drawText(rect().left() + 20, rect().height() - 15, infoText);
  p.drawText(rect().left() + 20, rect().height() - 15, infoText3);

  drawBottomIcons(p);
}

static const QColor get_tpms_color(float tpms) {
    if(tpms < 5 || tpms > 60) // N/A
        return QColor(255, 255, 255, 220);
    if(tpms < 31)
        return QColor(255, 90, 90, 220);
    return QColor(255, 255, 255, 220);
}

static const QString get_tpms_text(float tpms) {
    if(tpms < 5 || tpms > 60)
        return "";

    char str[32];
    snprintf(str, sizeof(str), "%.0f", round(tpms));
    return QString(str);
}

void NvgWindow::drawBottomIcons(QPainter &p) {
  const SubMaster &sm = *(uiState()->sm);
  auto car_state = sm["carState"].getCarState();
  auto scc_smoother = sm["carControl"].getCarControl().getSccSmoother();

  int x = radius / 2 + (bdr_s * 2) + (radius + 50);
  const int y = rect().bottom() - footer_h / 2 - 10;

  // tire pressure
  {
    const int w = 58;
    const int h = 126;
    const int x = 110;
    const int y = height() - h - 80;

    auto tpms = car_state.getTpms();
    const float fl = tpms.getFl();
    const float fr = tpms.getFr();
    const float rl = tpms.getRl();
    const float rr = tpms.getRr();

    p.setOpacity(0.8);
    p.drawPixmap(x, y, w, h, ic_tire_pressure);

    configFont(p, "Open Sans", 38, "Bold");

    QFontMetrics fm(p.font());
    QRect rcFont = fm.boundingRect("9");

    int center_x = x + 4;
    int center_y = y + h/2;
    const int marginX = (int)(rcFont.width() * 2.7f);
    const int marginY = (int)((h/2 - rcFont.height()) * 0.7f);

    drawText2(p, center_x-marginX, center_y-marginY-rcFont.height(), Qt::AlignRight, get_tpms_text(fl), get_tpms_color(fl));
    drawText2(p, center_x+marginX, center_y-marginY-rcFont.height(), Qt::AlignLeft, get_tpms_text(fr), get_tpms_color(fr));
    drawText2(p, center_x-marginX, center_y+marginY, Qt::AlignRight, get_tpms_text(rl), get_tpms_color(rl));
    drawText2(p, center_x+marginX, center_y+marginY, Qt::AlignLeft, get_tpms_text(rr), get_tpms_color(rr));
  }

  // cruise gap
  int gap = car_state.getCruiseGap();
  bool longControl = scc_smoother.getLongControl();
  int autoTrGap = scc_smoother.getAutoTrGap();

  p.setPen(Qt::NoPen);
  p.setBrush(QBrush(QColor(0, 0, 0, 255 * .1f)));
  p.drawEllipse(x - radius / 2, y - radius / 2, radius, radius);

  QString str;
  float textSize = 50.f;
  QColor textColor = QColor(255, 255, 255, 200);

  if(gap <= 0) {
    str = "N/A";
  }
  else if(longControl && gap == autoTrGap) {
    str = "AUTO";
    textColor = QColor(120, 255, 120, 200);
  }
  else {
    str.sprintf("%d", (int)gap);
    textColor = QColor(120, 255, 120, 200);
    textSize = 70.f;
  }

  configFont(p, "Open Sans", 35, "Bold");
  drawText(p, x, y-20, "GAP", 200);

  configFont(p, "Open Sans", textSize, "Bold");
  drawTextWithColor(p, x, y+50, str, textColor);

  // brake
  x = radius / 2 + (bdr_s * 2) + (radius + 50) * 2;
  bool brake_valid = car_state.getBrakeLights();
  float img_alpha = brake_valid ? 1.0f : 0.15f;
  float bg_alpha = brake_valid ? 0.3f : 0.1f;
  drawIcon(p, x, y, ic_brake, QColor(0, 0, 0, (255 * bg_alpha)), img_alpha);

  // auto hold
  int autohold = car_state.getAutoHold();
  if(autohold >= 0) {
    x = radius / 2 + (bdr_s * 2) + (radius + 50) * 3;
    x = x - 30;
    img_alpha = autohold > 0 ? 1.0f : 0.15f;
    bg_alpha = autohold > 0 ? 0.3f : 0.1f;
    drawIcon(p, x, y, autohold > 1 ? ic_autohold_warning : ic_autohold_active,
            QColor(0, 0, 0, (255 * bg_alpha)), img_alpha);
  }

  p.setOpacity(1.);
}

void NvgWindow::drawMaxSpeed(QPainter &p) {
  UIState *s = uiState();
  const SubMaster &sm = *(s->sm);
  const auto scc_smoother = sm["carControl"].getCarControl().getSccSmoother();
  bool is_metric = s->scene.is_metric;
  bool long_control = scc_smoother.getLongControl();
  
  // kph
  float applyMaxSpeed = scc_smoother.getApplyMaxSpeed();
  //float applyMaxSpeed = vtc_speed;
  float cruiseMaxSpeed = scc_smoother.getCruiseMaxSpeed();
  bool is_cruise_set = (cruiseMaxSpeed > 0 && cruiseMaxSpeed < 255);

  QRect rc(bdr_s * 2, bdr_s * 1.5, 184, 202);
  p.setPen(QPen(QColor(0xff, 0xff, 0xff, 100), 10));
  p.setBrush(QColor(0, 0, 0, 100));
  p.drawRoundedRect(rc, 20, 20);
  p.setPen(Qt::NoPen);

  if (is_cruise_set) {
    char str[256];
    if (is_metric)
        snprintf(str, sizeof(str), "%d", (int)(applyMaxSpeed + 0.5));
    else
        snprintf(str, sizeof(str), "%d", (int)(applyMaxSpeed*KM_TO_MILE + 0.5));

    configFont(p, "Open Sans", 45, "Bold");
    drawText(p, rc.center().x(), 100, str, 255);

    if (is_metric)
        snprintf(str, sizeof(str), "%d", (int)(cruiseMaxSpeed + 0.5));
    else
        snprintf(str, sizeof(str), "%d", (int)(cruiseMaxSpeed*KM_TO_MILE + 0.5));

    configFont(p, "Open Sans", 76, "Bold");
    drawText(p, rc.center().x(), 195, str, 255);
  } else {
    if(long_control) {
      configFont(p, "Open Sans", 48, "sans-semibold");
      drawText(p, rc.center().x(), 100, "OP", 100);
    }
    else {
      configFont(p, "Open Sans", 48, "sans-semibold");
      drawText(p, rc.center().x(), 100, "MAX", 100);
    }

    configFont(p, "Open Sans", 76, "sans-semibold");
    drawText(p, rc.center().x(), 195, "N/A", 100);
  }
}

void NvgWindow::drawSpeed(QPainter &p) {
  UIState *s = uiState();
  const SubMaster &sm = *(s->sm);
  float cur_speed = std::max(0.0, sm["carState"].getCarState().getCluSpeedMs() * (s->scene.is_metric ? MS_TO_KPH : MS_TO_MPH));

  QString speed;
  speed.sprintf("%.0f", cur_speed);
  configFont(p, "Open Sans", 176, "Bold");
  drawText(p, rect().center().x(), 230, speed);
  configFont(p, "Open Sans", 66, "Regular");
  drawText(p, rect().center().x(), 310, s->scene.is_metric ? "km/h" : "mph", 200);
}

void NvgWindow::drawCompass(QPainter &p) {
  UIState *s = uiState();

  int arrow_direction = (int)(s->scene.bearingUblox);

  // int x = bdr_s * 2;
  // int y = 610;

  // p.setPen(Qt::NoPen);
  // p.setBrush(QBrush(QColor(222, 222, 222, 222)));
  // QRect rect = QRect(x, y, radius, radius);
  // p.drawEllipse(rect);

  // p.setBrush(QBrush(QColor(255, 255, 255, 255)));

  // const int tickness = 14;
  // rect.adjust(tickness, tickness, -tickness, -tickness);
  // p.drawEllipse(rect);

  QString str;

  if(arrow_direction > 340 || arrow_direction <= 20)
    str.sprintf("N");
  else if(arrow_direction > 20 && arrow_direction <=70)
    str.sprintf("NE");
  else if(arrow_direction > 70 && arrow_direction <= 110)
    str.sprintf("E");
  else if(arrow_direction > 110 && arrow_direction <= 170)
    str.sprintf("SE");  
  else if(arrow_direction > 170 && arrow_direction <= 200)
    str.sprintf("S");      
  else if(arrow_direction > 200 && arrow_direction <= 250)
    str.sprintf("SW"); 
  else if(arrow_direction > 250 && arrow_direction <= 290)
    str.sprintf("W"); 
  else if(arrow_direction > 290 && arrow_direction <= 340)
    str.sprintf("NW");     

  // configFont(p, "Open Sans", 84, "Regular");
  // QColor qc = QColor(100, 100, 100, 254);
  // drawTextWithColor(p, rect.center().x(), y + 140, str, qc);  

  // p.setOpacity(1.f);
  // p.drawPixmap(rect.center().x()-20, y + 20, 40, 40, ic_up);

  // str.sprintf("%d", (int)(arrow_direction));
  // configFont(p, "Open Sans", 28, "Regular");
  // drawText(p, rc.center().x(), 770, str, 200);  

  //(p, rect().right() - 184 - bdr_s, int(bdr_s * 1.5), 184, vtcColor, vtcSpeed, 100

  int x = rect().right() - 184 - bdr_s;
  int y = int(bdr_s * 1.5);

  QRect rvtc(x, y, 184, 92);
  p.setPen(QPen(QColor(0xff, 0xff, 0xff, 100), 10));
  p.setBrush(QColor(0, 0, 0, 254));
  p.drawRoundedRect(rvtc, 10, 10);
  p.setPen(Qt::NoPen);

  // configFont(p, "Open Sans", 56, "SemiBold");
  // drawCenteredText(p, rvtc.center().x(), rvtc.center().y(), str, color);

  configFont(p, "Open Sans", 72, "SemiBold");
  QColor qc = QColor(200, 200, 200, 254);
  drawTextWithColor(p, rvtc.center().x(), rvtc.center().y() + 25, str, qc);  

}

void NvgWindow::drawDeviceInfo(QPainter &p) {
  UIState *s = uiState();

  QRect rc(30, 250, 184, 340);
  p.setPen(QPen(QColor(0xff, 0xff, 0xff, 100), 10));
  p.setBrush(QColor(0, 0, 0, 100));
  p.drawRoundedRect(rc, 10, 10);
  p.setPen(Qt::NoPen);

  //Temp
  QString str;
  str.sprintf("%d°C", (int)(s->scene.cpuTemp));
  configFont(p, "Open Sans", 64, "Regular");
  drawText(p, rc.center().x(), 330, str, 200);

  configFont(p, "Open Sans", 28, "Regular");
  drawText(p, rc.center().x(), 370, "CPU TEMP", 200);

  str.sprintf("%d %%", (int)(s->scene.cpuPerc));
  configFont(p, "Open Sans", 64, "Regular");
  drawText(p, rc.center().x(), 430, str, 200);

  configFont(p, "Open Sans", 28, "Regular");
  drawText(p, rc.center().x(), 470, "CPU", 200);  

  str.sprintf("%d°C", (int)(s->scene.ambientTemp));
  configFont(p, "Open Sans", 64, "Regular");
  drawText(p, rc.center().x(), 530, str, 200);

  configFont(p, "Open Sans", 28, "Regular");
  drawText(p, rc.center().x(), 570, "SYS TEMP", 200);

}

void NvgWindow::drawSpeedLimitMapBox(QPainter &p) {
  UIState *s = uiState();
  const SubMaster &sm = *(uiState()->sm);
  auto car_state = sm["carState"].getCarState();
  auto live_map_data = sm["liveMapData"].getLiveMapData();
  
  const auto lp = sm["longitudinalPlan"].getLongitudinalPlan();
  const float speed_limit = lp.getSpeedLimit() * (s->scene.is_metric ? MS_TO_KPH : MS_TO_MPH);
  //const float speed_limit = live_map_data.getSpeedLimit() * (s->scene.is_metric ? MS_TO_KPH : MS_TO_MPH);
  const QString str_limit_speed(QString::number(std::nearbyint(speed_limit)));

  int activeNDA = int(lp.getSpeedLimitControlState()); 

  if(activeNDA > 0)
  {
      int w = 120;
      int h = 54;
      int x = (width() + (bdr_s*2))/2 - w/2 - bdr_s;
      int y = 40 - bdr_s;

      p.setOpacity(1.f);
      p.drawPixmap(x, y, w, h, ic_hda);
  }

  // if(speed_limit > 10)
  // {
  //   int radius = 192;

  //   int x = 30;
  //   int y = 270;

  //   QRect rect(x, y, 184, 222);
  //   p.setPen(QPen(QColor(0xff, 0xff, 0xff, 100), 10));
  //   p.setBrush(QColor(0, 0, 0, 0));
  //   p.drawRoundedRect(rect, 20, 20);
  //   p.setPen(Qt::NoPen);

  //   p.setBrush(QBrush(QColor(255, 255, 255, 255)));

  //   const int tickness = 14;
  //   rect.adjust(tickness, tickness, -tickness, -tickness);
  //   p.drawRoundedRect(rect, 20, 20);

  //   QColor qc = QColor(100, 100, 100, 254);
    
  //   configFont(p, "Open Sans", 40, "Bold");
  //   p.setPen(QColor(0, 0, 0, 250));
  //   drawTextWithColor(p, rect.center().x(), y + 80, "LIMIT", qc);

  //   configFont(p, "Open Sans", 80, "Bold");
  //   drawTextWithColor(p, rect.center().x(), y + 170, str_limit_speed, qc);

  // }
  // else {
  //   int radius = 192;

  //   int x = 30;
  //   int y = 270;

  //   QRect rect(x, y, 184, 202);
  //   p.setPen(QPen(QColor(0xff, 0xff, 0xff, 100), 10));
  //   p.setBrush(QColor(0, 0, 0, 0));
  //   p.drawRoundedRect(rect, 20, 20);
  //   p.setPen(Qt::NoPen);

  //   p.setBrush(QBrush(QColor(255, 255, 255, 255)));

  //   const int tickness = 14;
  //   rect.adjust(tickness, tickness, -tickness, -tickness);
  //   p.drawRoundedRect(rect, 20, 20);

  //   configFont(p, "Open Sans", 80, "Bold");
  //   p.setPen(QColor(0, 0, 0, 230));
  //   p.drawText(rect, Qt::AlignCenter, "--");

  // }

  ////////////////////////////////////////////////////////////////////
    float cur_speed = std::max(0.0, sm["carState"].getCarState().getVEgo() * (s->scene.is_metric ? MS_TO_KPH : MS_TO_MPH));

    const auto vtcState = lp.getVisionTurnControllerState();
    const float vtc_speed = lp.getVisionTurnSpeed() * (s->scene.is_metric ? MS_TO_KPH : MS_TO_MPH);
    const auto lpSoruce = lp.getLongitudinalPlanSource();
    QColor vtc_color = tcs_colors[int(vtcState)];
    vtc_color.setAlpha(lpSoruce == cereal::LongitudinalPlan::LongitudinalPlanSource::TURN ? 255 : 100);

    setProperty("showVTC", vtcState > cereal::LongitudinalPlan::VisionTurnControllerState::DISABLED);
    setProperty("vtcSpeed", QString::number(std::nearbyint(vtc_speed)));
    setProperty("vtcColor", vtc_color);
    setProperty("showDebugUI", s->scene.show_debug_ui);

    const auto lmd = sm["liveMapData"].getLiveMapData();

    setProperty("roadName", QString::fromStdString(lmd.getCurrentRoadName()));

    const float speed_limit_offset = lp.getSpeedLimitOffset() * (s->scene.is_metric ? MS_TO_KPH : MS_TO_MPH);
    const auto slcState = lp.getSpeedLimitControlState();
    const bool sl_force_active = s->scene.speed_limit_control_enabled &&
                                 seconds_since_boot() < s->scene.last_speed_limit_sign_tap + 2.0;
    const bool sl_inactive = !sl_force_active && (!s->scene.speed_limit_control_enabled ||
                             slcState == cereal::LongitudinalPlan::SpeedLimitControlState::INACTIVE);
    const bool sl_temp_inactive = !sl_force_active && (s->scene.speed_limit_control_enabled &&
                                  slcState == cereal::LongitudinalPlan::SpeedLimitControlState::TEMP_INACTIVE);
    const int sl_distance = int(lp.getDistToSpeedLimit() * (s->scene.is_metric ? MS_TO_KPH : MS_TO_MPH) / 10.0) * 10;
    const QString sl_distance_str(QString::number(sl_distance) + (s->scene.is_metric ? "m" : "f"));
    const QString sl_offset_str(speed_limit_offset > 0.0 ?
                                "+" + QString::number(std::nearbyint(speed_limit_offset)) : "");
    const QString sl_inactive_str(sl_temp_inactive ? "TEMP" : "");
    const QString sl_substring(sl_inactive || sl_temp_inactive ? sl_inactive_str :
                               sl_distance > 0 ? sl_distance_str : sl_offset_str);

    setProperty("showSpeedLimit", speed_limit > 0.0);
    setProperty("speedLimit", activeNDA ? QString::number(std::nearbyint(speed_limit)) : "--");
    setProperty("slcSubText", sl_substring);
    setProperty("slcSubTextSize", sl_inactive || sl_temp_inactive || sl_distance > 0 ? 22.2 : 37.0);
    setProperty("mapSourcedSpeedLimit", lp.getIsMapSpeedLimit());
    //setProperty("slcActive", !sl_inactive && !sl_temp_inactive);
    setProperty("slcActive", !sl_inactive);

    const float tsc_speed = lp.getTurnSpeed() * (s->scene.is_metric ? MS_TO_KPH : MS_TO_MPH);
    const auto tscState = lp.getTurnSpeedControlState();
    const int t_distance = int(lp.getDistToTurn() * (s->scene.is_metric ? MS_TO_KPH : MS_TO_MPH) / 10.0) * 10;
    const QString t_distance_str(QString::number(t_distance) + (s->scene.is_metric ? "m" : "f"));

    setProperty("showTurnSpeedLimit", tsc_speed > 0.0 && (tsc_speed < cur_speed || s->scene.show_debug_ui));
    setProperty("turnSpeedLimit", QString::number(std::nearbyint(tsc_speed)));
    setProperty("tscSubText", t_distance > 0 ? t_distance_str : QString(""));
    setProperty("tscActive", tscState > cereal::LongitudinalPlan::SpeedLimitControlState::TEMP_INACTIVE);
    setProperty("curveSign", lp.getTurnSign());

      // max speed
  QRect rc(bdr_s * 2, bdr_s * 1.5, 184, 202);
  // p.setPen(QPen(QColor(0xff, 0xff, 0xff, 100), 10));
  // p.setBrush(QColor(0, 0, 0, 100));
  // p.drawRoundedRect(rc, 20, 20);
  // p.setPen(Qt::NoPen);

  // Bottom bar road name
  if (showDebugUI && !roadName.isEmpty()) {
    const int h = 60;
    QRect bar_rc(rect().left(), rect().bottom() - h, rect().width(), h);
    p.setBrush(QColor(0, 0, 0, 100));
    p.drawRect(bar_rc);
    configFont(p, "Open Sans", 38, "Bold");
    drawCenteredText(p, bar_rc.center().x(), bar_rc.center().y(),  QString::fromStdString(live_map_data.getCurrentRoadName()), QColor(255, 255, 255, 200));
  }

    // Speed Limit Sign
    if (showSpeedLimit) {
      drawSpeedSign(p, speed_sgn_rc, speedLimit, sl_substring, slcSubTextSize, mapSourcedSpeedLimit, slcActive);
    }

    if (showVTC && !tscActive) {
      setProperty("turnSpeedLimit", QString::number(std::nearbyint(vtc_speed)));
      setProperty("tscActive", vtcState > cereal::LongitudinalPlan::VisionTurnControllerState::DISABLED);
    }

    // Turn Speed Sign
    if (showVTC || showTurnSpeedLimit) {
      rc.moveTop(speed_sgn_rc.bottom() + bdr_s);
      drawTrunSpeedSign(p, rc, turnSpeedLimit, tscSubText, curveSign, tscActive);
    }

    //if (showVTC) {
    // drawVisionTurnControllerUI(p, rect().right() - 184 - bdr_s, int(bdr_s * 1.5) + 300, 184, vtcColor, vtcSpeed, 100);
    //}

}

void NvgWindow::drawCenteredText(QPainter &p, int x, int y, const QString &text, QColor color) {
  QFontMetrics fm(p.font());
  QRect init_rect = fm.boundingRect(text);
  QRect real_rect = fm.boundingRect(init_rect, 0, text);
  real_rect.moveCenter({x, y});

  p.setPen(color);
  p.drawText(real_rect, Qt::AlignCenter, text);
}

void NvgWindow::drawVisionTurnControllerUI(QPainter &p, int x, int y, int size, const QColor &color,
                                           const QString &vision_speed, int alpha) {
  QRect rvtc(x, y, size, size);
  p.setPen(QPen(color, 10));
  p.setBrush(QColor(0, 0, 0, alpha));
  p.drawRoundedRect(rvtc, 20, 20);
  p.setPen(Qt::NoPen);

  configFont(p, "Open Sans", 56, "SemiBold");
  drawCenteredText(p, rvtc.center().x(), rvtc.center().y(), vision_speed, color);
}

void NvgWindow::drawCircle(QPainter &p, int x, int y, int r, QBrush bg) {
  p.setPen(Qt::NoPen);
  p.setBrush(bg);
  p.drawEllipse(x - r, y - r, 2 * r, 2 * r);
}

void NvgWindow::drawSpeedSign(QPainter &p, QRect rc, const QString &speed_limit, const QString &sub_text,
                              int subtext_size, bool is_map_sourced, bool is_active) {
  const QColor ring_color = is_active ? QColor(255, 0, 0, 255) : QColor(0, 0, 0, 50);
  const QColor inner_color = QColor(255, 255, 255, is_active ? 255 : 85);
  const QColor text_color = QColor(0, 0, 0, is_active ? 255 : 85);

  const int x = rc.center().x();
  const int y = rc.center().y();
  const int r = rc.width() / 2.0f;

  drawCircle(p, x, y, r, ring_color);
  drawCircle(p, x, y, int(r * 0.8f), inner_color);

  configFont(p, "Open Sans", 89, "Bold");
  drawCenteredText(p, x, y, speed_limit, text_color);
  configFont(p, "Open Sans", subtext_size, "Bold");
  drawCenteredText(p, x, y + 55, sub_text, text_color);

  if (is_map_sourced) {
    p.setPen(Qt::NoPen);
    p.setOpacity(is_active ? 1.0 : 0.3);
    p.drawPixmap(x - subsign_img_size / 2, y - 55 - subsign_img_size / 2, map_img);
    p.setOpacity(1.0);
  }
}

void NvgWindow::drawTrunSpeedSign(QPainter &p, QRect rc, const QString &turn_speed, const QString &sub_text,
                                  int curv_sign, bool is_active) {
  const QColor border_color = is_active ? QColor(255, 0, 0, 255) : QColor(0, 0, 0, 50);
  const QColor inner_color = QColor(255, 255, 255, is_active ? 255 : 85);
  const QColor text_color = QColor(0, 0, 0, is_active ? 255 : 85);

  const int x = rc.center().x();
  const int y = rc.center().y();
  const int width = rc.width();

  const float stroke_w = 15.0;
  const float cS = stroke_w / 2.0 + 4.5;  // half width of the stroke on the corners of the triangle
  const float R = width / 2.0 - stroke_w / 2.0;
  const float A = 0.73205;
  const float h2 = 2.0 * R / (1.0 + A);
  const float h1 = A * h2;
  const float L = 4.0 * R / sqrt(3.0);

  // Draw the internal triangle, compensate for stroke width. Needed to improve rendering when in inactive
  // state due to stroke transparency being different from inner transparency.
  QPainterPath path;
  path.moveTo(x, y - R + cS);
  path.lineTo(x - L / 2.0 + cS, y + h1 + h2 - R - stroke_w / 2.0);
  path.lineTo(x + L / 2.0 - cS, y + h1 + h2 - R - stroke_w / 2.0);
  path.lineTo(x, y - R + cS);
  p.setPen(Qt::NoPen);
  p.setBrush(inner_color);
  p.drawPath(path);

  // Draw the stroke
  QPainterPath stroke_path;
  stroke_path.moveTo(x, y - R);
  stroke_path.lineTo(x - L / 2.0, y + h1 + h2 - R);
  stroke_path.lineTo(x + L / 2.0, y + h1 + h2 - R);
  stroke_path.lineTo(x, y - R);
  p.setPen(QPen(border_color, stroke_w, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  p.setBrush(Qt::NoBrush);
  p.drawPath(stroke_path);

  // Draw the turn sign
  if (curv_sign != 0) {
    p.setPen(Qt::NoPen);
    p.setOpacity(is_active ? 1.0 : 0.3);
    p.drawPixmap(int(x - (subsign_img_size / 2)), int(y - R + stroke_w + 30), curv_sign > 0 ? left_img : right_img);
    p.setOpacity(1.0);
  }

  // Draw the texts.
  configFont(p, "Open Sans", 67, "Bold");
  drawCenteredText(p, x, y + 25, turn_speed, text_color);
  configFont(p, "Open Sans", 22, "Bold");
  drawCenteredText(p, x, y + 65, sub_text, text_color);
}


void NvgWindow::drawSpeedLimit(QPainter &p) {
  const SubMaster &sm = *(uiState()->sm);
  auto car_state = sm["carState"].getCarState();
  auto scc_smoother = sm["carControl"].getCarControl().getSccSmoother();

  int activeNDA = scc_smoother.getRoadLimitSpeedActive();
  int limit_speed = scc_smoother.getRoadLimitSpeed();
  int left_dist = scc_smoother.getRoadLimitSpeedLeftDist();

  if(activeNDA > 0)
  {
      int w = 120;
      int h = 54;
      int x = (width() + (bdr_s*2))/2 - w/2 - bdr_s;
      int y = 40 - bdr_s;

      p.setOpacity(1.f);
      p.drawPixmap(x, y, w, h, activeNDA == 1 ? ic_nda : ic_hda);
  }

  if(limit_speed > 10 && left_dist > 0)
  {
    int radius = 192;

    int x = 30;
    int y = 270;

    p.setPen(Qt::NoPen);
    p.setBrush(QBrush(QColor(255, 0, 0, 255)));
    QRect rect = QRect(x, y, radius, radius);
    p.drawEllipse(rect);

    p.setBrush(QBrush(QColor(255, 255, 255, 255)));

    const int tickness = 14;
    rect.adjust(tickness, tickness, -tickness, -tickness);
    p.drawEllipse(rect);

    QString str_limit_speed, str_left_dist;
    str_limit_speed.sprintf("%d", limit_speed);

    if(left_dist >= 1000)
      str_left_dist.sprintf("%.1fkm", left_dist / 1000.f);
    else
      str_left_dist.sprintf("%dm", left_dist);

    configFont(p, "Open Sans", 80, "Bold");
    p.setPen(QColor(0, 0, 0, 230));
    p.drawText(rect, Qt::AlignCenter, str_limit_speed);

    configFont(p, "Open Sans", 60, "Bold");
    rect.translate(0, radius/2 + 45);
    rect.adjust(-30, 0, 30, 0);
    p.setPen(QColor(255, 255, 255, 230));
    p.drawText(rect, Qt::AlignCenter, str_left_dist);
  }
  else {
    auto controls_state = sm["controlsState"].getControlsState();
    int sccStockCamAct = (int)controls_state.getSccStockCamAct();
    int sccStockCamStatus = (int)controls_state.getSccStockCamStatus();

    if(sccStockCamAct == 2 && sccStockCamStatus == 2) {
      int radius = 192;

      int x = 30;
      int y = 270;

      p.setPen(Qt::NoPen);

      p.setBrush(QBrush(QColor(255, 0, 0, 255)));
      QRect rect = QRect(x, y, radius, radius);
      p.drawEllipse(rect);

      p.setBrush(QBrush(QColor(255, 255, 255, 255)));

      const int tickness = 14;
      rect.adjust(tickness, tickness, -tickness, -tickness);
      p.drawEllipse(rect);

      configFont(p, "Open Sans", 70, "Bold");
      p.setPen(QColor(0, 0, 0, 230));
      p.drawText(rect, Qt::AlignCenter, "CAM");
    }
  }
}

void NvgWindow::drawTurnSignals(QPainter &p) {
  static int blink_index = 0;
  static int blink_wait = 0;
  static double prev_ts = 0.0;

  if(blink_wait > 0) {
    blink_wait--;
    blink_index = 0;
  }
  else {
    const SubMaster &sm = *(uiState()->sm);
    auto car_state = sm["carState"].getCarState();
    bool left_on = car_state.getLeftBlinker();
    bool right_on = car_state.getRightBlinker();

    const float img_alpha = 0.8f;
    const int fb_w = width() / 2 - 200;
    const int center_x = width() / 2;
    const int w = fb_w / 25;
    const int h = 160;
    const int gap = fb_w / 25;
    const int margin = (int)(fb_w / 3.8f);
    const int base_y = (height() - h) / 2;
    const int draw_count = 8;

    int x = center_x;
    int y = base_y;

    if(left_on) {
      for(int i = 0; i < draw_count; i++) {
        float alpha = img_alpha;
        int d = std::abs(blink_index - i);
        if(d > 0)
          alpha /= d*2;

        p.setOpacity(alpha);
        float factor = (float)draw_count / (i + draw_count);
        p.drawPixmap(x - w - margin, y + (h-h*factor)/2, w*factor, h*factor, ic_turn_signal_l);
        x -= gap + w;
      }
    }

    x = center_x;
    if(right_on) {
      for(int i = 0; i < draw_count; i++) {
        float alpha = img_alpha;
        int d = std::abs(blink_index - i);
        if(d > 0)
          alpha /= d*2;

        float factor = (float)draw_count / (i + draw_count);
        p.setOpacity(alpha);
        p.drawPixmap(x + margin, y + (h-h*factor)/2, w*factor, h*factor, ic_turn_signal_r);
        x += gap + w;
      }
    }

    if(left_on || right_on) {

      double now = millis_since_boot();
      if(now - prev_ts > 900/UI_FREQ) {
        prev_ts = now;
        blink_index++;
      }

      if(blink_index >= draw_count) {
        blink_index = draw_count - 1;
        blink_wait = UI_FREQ/4;
      }
    }
    else {
      blink_index = 0;
    }
  }

  p.setOpacity(1.);
}

void NvgWindow::drawGpsStatus(QPainter &p) {
  const SubMaster &sm = *(uiState()->sm);
  auto gps = sm["gpsLocationExternal"].getGpsLocationExternal();
  float accuracy = gps.getAccuracy();
  if(accuracy < 0.01f || accuracy > 20.f)
    return;

  int w = 120;
  int h = 100;
  int x = width() - w - 30;
  int y = int(bdr_s * 1.5) + 122;

  p.setOpacity(0.8);
  p.drawPixmap(x, y, w, h, ic_satellite);

  configFont(p, "Open Sans", 40, "Bold");
  p.setPen(QColor(255, 255, 255, 200));
  p.setRenderHint(QPainter::TextAntialiasing);

  QRect rect = QRect(x, y + h + 10, w, 40);
  rect.adjust(-30, 0, 30, 0);

  QString str;
  str.sprintf("%.1fm", accuracy);
  p.drawText(rect, Qt::AlignHCenter, str);
  p.setOpacity(1.);
}

void NvgWindow::drawDebugText(QPainter &p) {
  const SubMaster &sm = *(uiState()->sm);
  QString str, temp;

  int y = 80;
  const int height = 60;

  const int text_x = width()/2 + 250;

  auto controls_state = sm["controlsState"].getControlsState();
  auto car_control = sm["carControl"].getCarControl();
  auto car_state = sm["carState"].getCarState();

  float applyAccel = controls_state.getApplyAccel();

  float aReqValue = controls_state.getAReqValue();
  float aReqValueMin = controls_state.getAReqValueMin();
  float aReqValueMax = controls_state.getAReqValueMax();

  int sccStockCamAct = (int)controls_state.getSccStockCamAct();
  int sccStockCamStatus = (int)controls_state.getSccStockCamStatus();

  int longControlState = (int)controls_state.getLongControlState();
  float vPid = controls_state.getVPid();
  float upAccelCmd = controls_state.getUpAccelCmd();
  float uiAccelCmd = controls_state.getUiAccelCmd();
  float ufAccelCmd = controls_state.getUfAccelCmd();
  float accel = car_control.getActuators().getAccel();

  const char* long_state[] = {"off", "pid", "stopping", "starting"};

  configFont(p, "Open Sans", 35, "Regular");
  p.setPen(QColor(255, 255, 255, 200));
  p.setRenderHint(QPainter::TextAntialiasing);

  str.sprintf("State: %s\n", long_state[longControlState]);
  p.drawText(text_x, y, str);

  y += height;
  str.sprintf("vPid: %.3f(%.1f)\n", vPid, vPid * 3.6f);
  p.drawText(text_x, y, str);

  y += height;
  str.sprintf("P: %.3f\n", upAccelCmd);
  p.drawText(text_x, y, str);

  y += height;
  str.sprintf("I: %.3f\n", uiAccelCmd);
  p.drawText(text_x, y, str);

  y += height;
  str.sprintf("F: %.3f\n", ufAccelCmd);
  p.drawText(text_x, y, str);

  y += height;
  str.sprintf("Accel: %.3f\n", accel);
  p.drawText(text_x, y, str);

  y += height;
  str.sprintf("Apply: %.3f, Stock: %.3f\n", applyAccel, aReqValue);
  p.drawText(text_x, y, str);

  y += height;
  str.sprintf("%.3f (%.3f/%.3f)\n", aReqValue, aReqValueMin, aReqValueMax);
  p.drawText(text_x, y, str);

  auto lead_radar = sm["radarState"].getRadarState().getLeadOne();
  auto lead_one = sm["modelV2"].getModelV2().getLeadsV3()[0];

  float radar_dist = lead_radar.getStatus() && lead_radar.getRadar() ? lead_radar.getDRel() : 0;
  float vision_dist = lead_one.getProb() > .5 ? (lead_one.getX()[0] - 1.5) : 0;

  y += height;
  str.sprintf("Lead: %.1f/%.1f/%.1f\n", radar_dist, vision_dist, (radar_dist - vision_dist));
  p.drawText(text_x, y, str);

  auto longitudinal_plan = sm["longitudinalPlan"].getLongitudinalPlan();
  auto live_map_data = sm["liveMapData"].getLiveMapData();

  y += height;
  str.sprintf("vEgo/GPSSpeed: %.1f/%.1f\n", car_state.getVEgo() * 3.6, live_map_data.getLastGpsSpeed() * 3.6);
  p.drawText(text_x, y, str);

  y += height;
  str.sprintf("Location: %.1f/%.1f\n", live_map_data.getLastGpsLatitude(), live_map_data.getLastGpsLongitude());
  p.drawText(text_x, y, str);

  y += height;
  str.sprintf("Speed Limit: %.1f\n", live_map_data.getSpeedLimit() * 3.6);
  p.drawText(text_x, y, str);

  y += height;
  str.sprintf("Speed Limit Ahead: %.1f\n", live_map_data.getSpeedLimitAhead() * 3.6);
  p.drawText(text_x, y, str);

  y += height;
  str.sprintf("Turn Speed Limit: %.1f\n", live_map_data.getTurnSpeedLimit() * 3.6);
  p.drawText(text_x, y, str);

  y += height;
  str.sprintf("End Distance: %.1f\n", live_map_data.getTurnSpeedLimitEndDistance());
  p.drawText(text_x, y, str);

  y += height;
  str.sprintf("VISION TURN CONTROLLER: %.1f\n", longitudinal_plan.getVisionTurnSpeed() * 3.6);
  p.drawText(text_x, y, str);

}