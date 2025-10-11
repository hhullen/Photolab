#ifndef PICTUREPLANE_H
#define PICTUREPLANE_H

#include <QDir>
#include <QObject>
#include <QPainter>
#include <QTransform>
#include <vector>

#include "libs/matrix/matrix.h"

class PicturePlane : public QObject {
  Q_OBJECT

 public:
  PicturePlane();

  void SetImages(QImage);
  void AddCutterShift(QPoint, QPoint);
  void AddMoveShift(QPoint, QPoint);
  void AddScale(int, QPointF);
  void ResetChanges();
  const QImage &GetInitialImage();
  Matrix<uchar, MatrixType::NoCopy> GetLastHistoryPixels();
  Matrix<const uchar, MatrixType::NoCopy> GetPrevHistoryPixels();
  void MakeNewLayer();
  bool Undo();

  void DrawPicture(QPainter *);

 private:
  bool firstDraw_{true};
  int cutterShift_{0};
  QTransform transform_;

  QImage initImage_{nullptr};
  QPixmap initPixmap_;

  QPixmap pixmapBuffer_;

  std::vector<QImage> history_;

  std::tuple<bool, QPointF> IsCursorOnImage(QPointF);
};

#endif  // PICTUREPLANE_H
