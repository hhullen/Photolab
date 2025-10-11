#ifndef VIEWSPACE_H
#define VIEWSPACE_H

#include <QBitmap>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPicture>
#include <QTimer>
#include <QWidget>

#include "gui/const.h"

const int kTileSize = 30;
const int kCornerRadius = 30;

class ViewSpace : public QWidget {
  Q_OBJECT

 public:
  explicit ViewSpace(QWidget *parent = nullptr);

  void SetInProcessing(bool);

  void paintEvent(QPaintEvent *) override;
  void resizeEvent(QResizeEvent *) override;

  void wheelEvent(QWheelEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void mousePressEvent(QMouseEvent *) override;

 signals:
  void DrawObject(QPainter *);
  void WheelDrugged(int, QPointF);
  void MouseDruggedLMB(QPoint, QPoint);
  void MouseDruggedRMB(QPoint, QPoint);

 private:
  QPixmap backgroundTile_;

  QPoint mousePressed_;

  bool processing_{true};
  int processIndicatorPos_{-Const::Digit::kPocessIndicatorWidth};
  QTimer *timer_;

  void RoundCorners(QPainter &painter);
  void DrawProcessIndicator(QPainter &painter);
};

#endif  // VIEWSPACE_H
