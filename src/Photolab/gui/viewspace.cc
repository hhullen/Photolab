#include "viewspace.h"

#include "gui/const.h"

ViewSpace::ViewSpace(QWidget *parent)
    : QWidget{parent}, backgroundTile_{kTileSize * 2, kTileSize * 2} {
  setAttribute(Qt::WA_MouseTracking);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  backgroundTile_.fill(Const::Color::kTileLight);
  QPainter painter(&backgroundTile_);
  painter.fillRect(0, 0, kTileSize, kTileSize, Const::Color::kTileDark);
  painter.fillRect(kTileSize, kTileSize, kTileSize, kTileSize,
                   Const::Color::kTileDark);
  painter.end();

  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, [this] {
    processIndicatorPos_ += Const::Digit::kPocessIndicatorShift;
    if (processIndicatorPos_ > width()) {
      processIndicatorPos_ = -Const::Digit::kPocessIndicatorWidth;
    }
    update();
  });
}

void ViewSpace::SetInProcessing(bool v) {
  processing_ = v;
  processIndicatorPos_ = -Const::Digit::kPocessIndicatorWidth;

  if (processing_) {
    timer_->start(Const::Digit::kPocessIndicatorShiftDelayMs);
  }
}

void ViewSpace::paintEvent(QPaintEvent *e) {
  QPainter painter;
  painter.begin(this);

  painter.setClipRect(e->rect());
  painter.setRenderHint(QPainter::Antialiasing);

  RoundCorners(painter);
  painter.drawTiledPixmap(e->rect(), backgroundTile_);

  emit DrawObject(&painter);

  if (processing_) {
    DrawProcessIndicator(painter);
  } else {
    timer_->stop();
  }

  painter.end();
}

void ViewSpace::RoundCorners(QPainter &painter) {
  QPainterPath clipPath;

  clipPath.addRoundedRect(rect(), kCornerRadius, kCornerRadius);

  painter.setClipPath(clipPath, Qt::ClipOperation::ReplaceClip);
}

void ViewSpace::DrawProcessIndicator(QPainter &painter) {
  int barWidth = Const::Digit::kPocessIndicatorWidth;
  int x = processIndicatorPos_;
  int y = 0;
  int h = painter.viewport().height();

  QRect rect(x, y, barWidth, h);

  QLinearGradient gradient(rect.left(), 0, rect.right(), 0);

  gradient.setColorAt(0.0, QColor(255, 255, 255, 0));
  gradient.setColorAt(0.5, QColor(255, 250, 255, 150));
  gradient.setColorAt(1.0, QColor(255, 255, 255, 0));

  painter.fillRect(rect, gradient);
}

void ViewSpace::resizeEvent(QResizeEvent *e) { QWidget::resizeEvent(e); }

void ViewSpace::wheelEvent(QWheelEvent *e) {
  emit WheelDrugged(e->angleDelta().y(), e->position());

  e->accept();
  update();
}

void ViewSpace::mouseMoveEvent(QMouseEvent *e) {
  if (e->buttons() == Qt::MouseButton::LeftButton) {
    emit MouseDruggedLMB(e->pos() - mousePressed_, mousePressed_);
  } else if (e->buttons() == Qt::MouseButton::RightButton) {
    emit MouseDruggedRMB(e->pos() - mousePressed_, mousePressed_);
  }
  mousePressed_ = e->pos();

  e->accept();
  update();
}

void ViewSpace::mousePressEvent(QMouseEvent *e) {
  mousePressed_ = e->pos();

  e->accept();
}
