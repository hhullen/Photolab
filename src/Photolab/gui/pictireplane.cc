#include "gui/const.h"
#include "pictureplane.h"

PicturePlane::PicturePlane() {}

void PicturePlane::SetImages(QImage img) {
  if (img.isNull()) {
    return;
  }

  initImage_ = img.convertToFormat(QImage::Format_ARGB32);
  initPixmap_.convertFromImage(initImage_);

  history_.clear();
  history_.emplace_back(initImage_.copy());

  firstDraw_ = true;
}

void PicturePlane::AddCutterShift(QPoint v, QPoint) {
  if (initImage_.isNull()) {
    return;
  }

  qreal scaleX = transform_.m11();
  int newValue = cutterShift_ + v.x() / scaleX;

  cutterShift_ = std::clamp(newValue, 0, initImage_.width());
}

void PicturePlane::AddMoveShift(QPoint shift, QPoint cursor) {
  if (initImage_.isNull()) {
    return;
  }

  auto [isOnImg, _] = IsCursorOnImage(cursor);
  if (!isOnImg) {
    return;
  }

  qreal scaleX = transform_.m11();
  qreal scaleY = transform_.m22();

  transform_.translate(shift.x() / scaleX, shift.y() / scaleY);
}

void PicturePlane::AddScale(int v, QPointF cursor) {
  if (initImage_.isNull()) {
    return;
  }

  const qreal scaleX = transform_.m11();
  if ((scaleX > Const::Digit::kMaxScale && v > 0) ||
      (scaleX < Const::Digit::kMinScale && v < 0)) {
    return;
  }

  auto [isOnImg, onImgPos] = IsCursorOnImage(cursor);
  QPointF adjustShift = onImgPos;
  if (!isOnImg) {
    adjustShift.rx() = initImage_.width() / 2;
    adjustShift.ry() = initImage_.height() / 2;
  }

  const float dScale = 1 + 1 * static_cast<float>(v) / 1000;
  transform_.translate(adjustShift.x(), adjustShift.y());
  transform_.scale(dScale, dScale);
  transform_.translate(-adjustShift.x(), -adjustShift.y());
}

void PicturePlane::ResetChanges() {
  SetImages(initImage_);
  cutterShift_ = 0;
  transform_.reset();
}

const QImage &PicturePlane::GetInitialImage() { return initImage_; }

// Returns true is it is possible to undo after this call
bool PicturePlane::Undo() {
  if (history_.size() < 2) {
    return false;
  }

  history_.pop_back();

  if (history_.size() < 2) {
    return false;
  }

  return true;
}

void PicturePlane::DrawPicture(QPainter *p) {
  if (initImage_.isNull()) {
    return;
  }

  if (firstDraw_) {
    const QSize shift = (p->viewport().size() - initImage_.size()) / 2;
    transform_.translate(shift.width(), shift.height());
    firstDraw_ = false;
  }

  p->save();

  p->setTransform(transform_);

  const QRect rectInitial(0, 0, cutterShift_, initImage_.height());
  const QPoint zeroPoint{0, 0};
  p->drawPixmap(zeroPoint, initPixmap_, rectInitial);

  pixmapBuffer_.convertFromImage(history_.back(),
                                 Qt::ImageConversionFlag::NoFormatConversion);

  const QRect rect_changed(cutterShift_, 0, initImage_.width(),
                           initImage_.height());
  const QPoint cutterPos(cutterShift_, 0);
  p->drawPixmap(cutterPos, pixmapBuffer_, rect_changed);

  p->restore();
  p->save();

  const qreal translateX = transform_.m31();
  const qreal translateY = transform_.m32();

  p->translate(translateX, translateY);

  const QBrush brush(Const::Color::kCutterColor);
  const QPen pen(brush, Const::Digit::kCutterWidth);
  p->setPen(pen);
  p->setBrush(brush);

  const qreal scaleX = transform_.m22();
  const qreal scaleY = transform_.m22();
  const int adjustedCutter = cutterShift_ * scaleX;
  const int top = 0;

  p->drawLine(adjustedCutter, top, adjustedCutter,
              initImage_.height() * scaleY);

  const int diam = Const::Digit::kCutterHandlerRadius * 2;
  p->drawEllipse(adjustedCutter - Const::Digit::kCutterHandlerRadius,
                 top - diam, diam, diam);

  QFont font = p->font();
  font.setPointSize(Const::Digit::kSizeTitlePt);
  p->setFont(font);

  char sizeTitle[21];
  std::sprintf(sizeTitle, "%dx%d", initImage_.width(), initImage_.height());
  p->drawText(0, -diam * 2, sizeTitle);

  p->restore();
}

std::tuple<bool, QPointF> PicturePlane::IsCursorOnImage(QPointF cursor) {
  const QPointF inImageCursorPos = transform_.inverted().map(cursor);
  const QRectF imgRect(0, 0, initImage_.width(), initImage_.height());
  return {imgRect.contains(inImageCursorPos), std::move(inImageCursorPos)};
}

// Return mutable array of pixels presented by Matrix 1xN.
// whrere N is size in bytes
Matrix<uchar, MatrixType::NoCopy> PicturePlane::GetLastHistoryPixels() {
  if (history_.size() < 1) {
    throw std::invalid_argument("Fuck!");
  }

  return Matrix<uchar, MatrixType::NoCopy>(history_.back().bits(), 1,
                                           history_.back().sizeInBytes());
}

// Return immutable array of pixels presented by Matrix 1xN.
// whrere N is size in bytes
Matrix<const uchar, MatrixType::NoCopy> PicturePlane::GetPrevHistoryPixels() {
  if (history_.size() < 1) {
    throw std::invalid_argument("Shit, man!");
  }

  if (history_.size() == 1) {
    return Matrix<const uchar, MatrixType::NoCopy>(initImage_.bits(), 1,
                                                   initImage_.sizeInBytes());
    ;
  }

  const QImage &took = history_[history_.size() - 2];
  return Matrix<const uchar, MatrixType::NoCopy>(took.bits(), 1,
                                                 took.sizeInBytes());
}

void PicturePlane::MakeNewLayer() {
  history_.emplace_back(history_.back().copy());
}
