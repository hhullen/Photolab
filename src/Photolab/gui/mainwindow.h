#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QMenuBar>
#include <QSizePolicy>
#include <QStatusBar>
#include <QTextBrowser>
#include <QThread>

#include "controller_interface.h"
#include "gui/colorcorrectiondialog.h"
#include "kernelfilterdialog.h"
#include "pictureplane.h"
#include "simplefilterdialog.h"
#include "viewspace.h"

using BytesBGRA = unsigned char[4];
using PixBytes = Matrix<uchar, MatrixType::NoCopy>;
using ConstPixBytes = Matrix<const uchar, MatrixType::NoCopy>;

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(IController &ctrl);
  ~MainWindow();

 private:
  IController &controller_;
  QThread *thread_{nullptr};
  std::atomic<bool> kernelStop_{false};
  std::mutex kernelMutex_;

  ViewSpace *viewSpace_;
  PicturePlane *picturePlane_;
  SimpleFilterDialog *simpleFilterDialog_;
  KernelFilterDialog *kernelFilterDialog_;
  ColorCorrectionDialog *colorCorrectionDialog_;

  QMenuBar *menuBar_{nullptr};

  QMenu *menuFile_{nullptr};
  QMenu *menuEdit_{nullptr};
  QMenu *menuHelp_{nullptr};

  QAction *load_{nullptr};
  QAction *save_{nullptr};
  QAction *winx_{nullptr};
  QAction *kukoyaka_{nullptr};
  QAction *cyberpunkPoster_{nullptr};
  QAction *JohnySilverhand_{nullptr};
  QAction *nightCity_{nullptr};

  QAction *simpleFilter_{nullptr};
  QAction *convolutionFilter_{nullptr};
  QAction *colorCorrection_{nullptr};
  QAction *reset_{nullptr};
  QAction *undo_{nullptr};

  QAction *controls_{nullptr};

  QString currentFileName_;

  QStatusBar *statusBar_;

  void LoadFile();
  void SaveFile();
  void Reset();
  void DrawWinx();
  void DrawKukoyaka();
  void DrawCuberpunkPoster();
  void DrawJohnySilverhand();
  void DrawNightcity();
  void DrawImage(QString);
  void ShowControlsHelp();
  void StartOperatingSimpleFilter();
  void StartOperatingKernelFilter();
  void StartOperatingColorCorrection();
  std::tuple<const QImage &, ConstPixBytes, PixBytes, bool> GetPictirePicels();
  void MakeSimpleFilter(QString filter, QString channel, QColor color);
  void MakeKernelFilter(Matrix<int>, size_t);
  void MakeSimpleColorCorrection(int, int);
  void MakeHSVColorCorrection(int, int, int);
  void MakeHSLColorCorrection(int, int, int);
  void UpdateFilterKernel(QString);
  std::tuple<Matrix<int>, size_t> GetKernelOpts(QString);
  void CancelOperation();
  void AcceptOperation();

  void SetupMenuFile();
  void SetupMenuEdit();
  void SetupMenuHelp();
};
#endif  // MAINWINDOW_H
