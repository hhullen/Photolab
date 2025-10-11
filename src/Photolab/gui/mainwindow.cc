#include "mainwindow.h"

#include "const.h"

MainWindow::MainWindow(IController &ctrl) : controller_{ctrl} {
  QWidget *central = new QWidget(this);
  setCentralWidget(central);

  viewSpace_ = new ViewSpace(central);
  viewSpace_->setMinimumSize(Const::Size::kMainWindowMininum);

  picturePlane_ = new PicturePlane;

  QHBoxLayout *viewLayout = new QHBoxLayout(central);
  viewLayout->addWidget(viewSpace_);
  central->setLayout(viewLayout);

  menuBar_ = new QMenuBar(this);
  setMenuBar(menuBar_);

  // add icons?
  SetupMenuFile();
  SetupMenuEdit();
  SetupMenuHelp();

  statusBar_ = new QStatusBar(this);
  setStatusBar(statusBar_);

  connect(viewSpace_, &ViewSpace::DrawObject, picturePlane_,
          &PicturePlane::DrawPicture);
  connect(viewSpace_, &ViewSpace::WheelDrugged, picturePlane_,
          &PicturePlane::AddScale);
  connect(viewSpace_, &ViewSpace::MouseDruggedLMB, picturePlane_,
          &PicturePlane::AddCutterShift);
  connect(viewSpace_, &ViewSpace::MouseDruggedRMB, picturePlane_,
          &PicturePlane::AddMoveShift);
}

MainWindow::~MainWindow() {}

void MainWindow::LoadFile() {
  auto filePath = QFileDialog::getOpenFileName(
      this, Const::Label::kOpenFileDialog, QDir::homePath(),
      Const::Constrain::kImageFormats);

  if (filePath == "") {
    statusBar_->showMessage(Const::Msg::kFailedLoadFile,
                            Const::Digit::kMessageDelayMs);
    return;
  }

  currentFileName_ = QFileInfo(filePath).fileName();

  DrawImage(filePath);

  statusBar_->showMessage(Const::Msg::kSuccessfulyLoadedFile + QString(" ") +
                              filePath,
                          Const::Digit::kMessageDelayMs);
}

void MainWindow::SaveFile() {
  const QImage &img = picturePlane_->GetInitialImage();
  PixBytes pixels = picturePlane_->GetLastHistoryPixels();

  const QImage toSave(pixels.getData(), img.width(), img.height(),
                      img.format());

  QString filePath = QFileDialog::getExistingDirectory(
      this, Const::Label::kSelectFolderFileDialog, QDir::homePath());

  QString dt =
      QDateTime::currentDateTime().toString().replace(' ', '_').replace(':',
                                                                        '_');
  filePath = (filePath + "/" + dt + "_" + currentFileName_);

  if (!toSave.save(filePath)) {
    statusBar_->showMessage(Const::Msg::kFailedSaveFile + QString(" ") +
                                filePath,
                            Const::Digit::kMessageDelayMs);
  }
  statusBar_->showMessage(Const::Msg::kSuccessfulySavedFile + QString(" ") +
                              filePath,
                          Const::Digit::kMessageDelayMs);
}

void MainWindow::Reset() {
  undo_->setEnabled(false);
  picturePlane_->ResetChanges();
  viewSpace_->update();
}

void MainWindow::DrawWinx() { DrawImage(Const::Resource::kFilePathWinx); }

void MainWindow::DrawKukoyaka() {
  DrawImage(Const::Resource::kFilePathKukoyaka);
}

void MainWindow::DrawCuberpunkPoster() {
  DrawImage(Const::Resource::kFilePathCuberpunkPoster);
}

void MainWindow::DrawJohnySilverhand() {
  DrawImage(Const::Resource::kFilePathJohnySilverhand);
}

void MainWindow::DrawNightcity() {
  DrawImage(Const::Resource::kFilePathNightCity);
}

void MainWindow::DrawImage(QString path) {
  Reset();

  const QImage img = QImage(path);
  if (img.isNull()) {
    statusBar_->showMessage(Const::Msg::kFailedLoadFile,
                            Const::Digit::kMessageDelayMs);
    return;
  }

  picturePlane_->SetImages(img);
  currentFileName_ = QFileInfo(path).fileName();
  viewSpace_->update();

  save_->setEnabled(true);
  menuEdit_->setEnabled(true);
}

void MainWindow::ShowControlsHelp() {
  QFile textFile(Const::Resource::kFilePathControlsHelp);
  if (!textFile.open(QFile::ReadOnly)) {
    statusBar_->showMessage(Const::Msg::kFailedLoadInfo);
    return;
  }

  QString text = textFile.readAll();

  QTextBrowser *sourceViewer = new QTextBrowser;
  sourceViewer->setWindowTitle(Const::Label::kConstolsWindow);
  sourceViewer->setParent(this, Qt::Dialog);
  sourceViewer->setAttribute(Qt::WA_DeleteOnClose);
  sourceViewer->setLineWrapMode(QTextEdit::LineWrapMode::WidgetWidth);
  sourceViewer->setHtml(text);
  sourceViewer->resize(Const::Size::kControlsHelp);
  sourceViewer->show();
}

void MainWindow::StartOperatingSimpleFilter() {
  picturePlane_->MakeNewLayer();

  simpleFilterDialog_ = new SimpleFilterDialog(this, menuBar_);

  connect(simpleFilterDialog_, &SimpleFilterDialog::MakeFilter, this,
          &MainWindow::MakeSimpleFilter);
  connect(simpleFilterDialog_, &SimpleFilterDialog::Canceled, this,
          &MainWindow::CancelOperation);
  connect(simpleFilterDialog_, &SimpleFilterDialog::Accepted, this,
          &MainWindow::AcceptOperation);
  simpleFilterDialog_->setAttribute(Qt::WA_DeleteOnClose);

  simpleFilterDialog_->Show();
}

void MainWindow::StartOperatingKernelFilter() {
  picturePlane_->MakeNewLayer();

  kernelFilterDialog_ = new KernelFilterDialog(this, menuBar_);
  kernelFilterDialog_->setAttribute(Qt::WA_DeleteOnClose);

  connect(kernelFilterDialog_, &KernelFilterDialog::FilterChanged, this,
          &MainWindow::UpdateFilterKernel);
  connect(kernelFilterDialog_, &KernelFilterDialog::Accepted, this,
          &MainWindow::AcceptOperation);
  connect(kernelFilterDialog_, &KernelFilterDialog::Canceled, this,
          &MainWindow::CancelOperation);

  kernelFilterDialog_->Show();
}

void MainWindow::StartOperatingColorCorrection() {
  picturePlane_->MakeNewLayer();

  colorCorrectionDialog_ = new ColorCorrectionDialog(this, menuBar_);
  colorCorrectionDialog_->setAttribute(Qt::WA_DeleteOnClose);

  connect(colorCorrectionDialog_, &ColorCorrectionDialog::MakeSimple, this,
          &::MainWindow::MakeSimpleColorCorrection);
  connect(colorCorrectionDialog_, &ColorCorrectionDialog::MakeHSV, this,
          &::MainWindow::MakeHSVColorCorrection);
  connect(colorCorrectionDialog_, &ColorCorrectionDialog::MakeHSL, this,
          &::MainWindow::MakeHSLColorCorrection);
  connect(colorCorrectionDialog_, &ColorCorrectionDialog::Accepted, this,
          &::MainWindow::AcceptOperation);
  connect(colorCorrectionDialog_, &ColorCorrectionDialog::Canceled, this,
          &::MainWindow::CancelOperation);

  colorCorrectionDialog_->show();
}

std::tuple<const QImage &, ConstPixBytes, PixBytes, bool>
MainWindow::GetPictirePicels() {
  const QImage &img = picturePlane_->GetInitialImage();
  ConstPixBytes prevPixels = picturePlane_->GetPrevHistoryPixels();
  PixBytes pixels = picturePlane_->GetLastHistoryPixels();

  bool valid = prevPixels.cols() == static_cast<size_t>(img.sizeInBytes()) &&
               pixels.cols() == static_cast<size_t>(img.sizeInBytes());
  if (!valid) {
    statusBar_->showMessage(Const::Msg::kFailedOperatingHistory,
                            Const::Digit::kMessageDelayMs);
  }

  return {img, std::move(prevPixels), std::move(pixels), valid};
}

void MainWindow::MakeSimpleFilter(QString filter, QString channel,
                                  QColor color) {
  auto &&[img, prevPixels, pixels, valid] = GetPictirePicels();

  if (!valid) {
    return;
  }

  const BytesBGRA col{static_cast<unsigned char>(color.blue()),
                      static_cast<unsigned char>(color.green()),
                      static_cast<unsigned char>(color.red()),
                      static_cast<unsigned char>(color.alpha())};

  controller_.MakeSimpleFilter(std::move(prevPixels), std::move(pixels),
                               img.sizeInBytes(), filter.toStdString(),
                               channel.toStdString(), col);

  viewSpace_->update();
}

void MainWindow::UpdateFilterKernel(QString kernelName) {
  auto [kernel, normalizer] = GetKernelOpts(kernelName);

  if (kernel.rows() < Const::Digit::kMinKernelSize) {
    statusBar_->showMessage(Const::Msg::kFailedFailedGettingKernel,
                            Const::Digit::kMessageDelayMs);
    kernelFilterDialog_->close();
    return;
  }

  kernelFilterDialog_->SetFilterOpts(kernelName, kernel, normalizer);
  MakeKernelFilter(std::move(kernel), normalizer);
}

std::tuple<Matrix<int>, size_t> MainWindow::GetKernelOpts(QString kernelName) {
  if (kernelName == Const::Name::Filter::Kernel::kOptCustom) {
    return kernelFilterDialog_->GetCustomKernelOpts();
  }
  return controller_.GetFilterKernel(
      kernelName.toStdString(), kernelFilterDialog_->GetCurrentArealSize());
}

void MainWindow::MakeKernelFilter(Matrix<int> kernel, size_t normalizer) {
  if (thread_) {
    kernelStop_ = true;
    thread_->wait();
    kernelStop_ = false;
  }

  auto [img, prevPixels, pixels, valid] = GetPictirePicels();

  if (!valid) {
    return;
  }

  viewSpace_->SetInProcessing(true);
  kernelFilterDialog_->SetButtonsEnebled(false);
  std::shared_ptr<bool> success(new bool(false));
  thread_ = QThread::create(
      [this, img, normalizer,
       success](Matrix<int> kernel, ConstPixBytes prevPixels, PixBytes pixels) {
        *success = controller_.MakeKernelFilter(
            std::move(prevPixels), std::move(pixels), img.width(), img.height(),
            std::move(kernel), normalizer, kernelStop_);
        viewSpace_->SetInProcessing(false);
        kernelFilterDialog_->SetButtonsEnebled(true);
        viewSpace_->update();
      },
      std::move(kernel), std::move(prevPixels), std::move(pixels));

  thread_->start();
}

void MainWindow::MakeSimpleColorCorrection(int brightness, int contrast) {
  auto [img, prevPixels, pixels, valid] = GetPictirePicels();

  if (!valid) {
    return;
  }

  controller_.MakeSimpleColorCorrection(std::move(prevPixels),
                                        std::move(pixels), img.sizeInBytes(),
                                        brightness, contrast);

  viewSpace_->update();
}

void MainWindow::MakeHSVColorCorrection(int h, int s, int v) {
  auto [img, prevPixels, pixels, valid] = GetPictirePicels();
  controller_.MakeHSVColorCorrection(std::move(prevPixels), std::move(pixels),
                                     img.sizeInBytes(), h, s, v);
  viewSpace_->update();
}

void MainWindow::MakeHSLColorCorrection(int h, int s, int l) {
  auto [img, prevPixels, pixels, valid] = GetPictirePicels();
  controller_.MakeHSVColorCorrection(std::move(prevPixels), std::move(pixels),
                                     img.sizeInBytes(), h, s, l);
  viewSpace_->update();
}

void MainWindow::CancelOperation() {
  if (thread_) {
    kernelStop_ = true;
    thread_->wait();
    kernelStop_ = false;
  }

  if (!picturePlane_->Undo()) {
    undo_->setEnabled(false);
  }

  viewSpace_->update();
}

void MainWindow::AcceptOperation() { undo_->setEnabled(true); }

void MainWindow::SetupMenuFile() {
  menuFile_ = new QMenu(Const::Name::kMenuFile, menuBar_);
  menuBar_->addMenu(menuFile_);

  load_ = new QAction(QIcon(":icons/resources/load.png"),
                      Const::Name::MenuAct::kLoad, menuFile_);
  load_->setShortcut(QKeySequence(Const::Shortcut::kActionLoad));
  save_ = new QAction(QIcon(":icons/resources/save.png"),
                      Const::Name::MenuAct::kSave, menuFile_);
  save_->setShortcut(QKeySequence(Const::Shortcut::kActionSave));
  save_->setEnabled(false);
  winx_ = new QAction(QIcon(":icons/resources/picture.png"),
                      Const::Name::MenuAct::kWinx, menuFile_);
  kukoyaka_ = new QAction(QIcon(":icons/resources/picture.png"),
                          Const::Name::MenuAct::kKukoyaka, menuFile_);
  cyberpunkPoster_ =
      new QAction(QIcon(":icons/resources/picture.png"),
                  Const::Name::MenuAct::kCyberPunkPoster, menuFile_);
  JohnySilverhand_ =
      new QAction(QIcon(":icons/resources/picture.png"),
                  Const::Name::MenuAct::kJohnySilverhand, menuFile_);
  nightCity_ = new QAction(QIcon(":icons/resources/picture.png"),
                           Const::Name::MenuAct::kNightCity, menuFile_);

  menuFile_->addAction(load_);
  menuFile_->addAction(save_);
  menuFile_->addSeparator();
  menuFile_->addAction(winx_);
  menuFile_->addAction(kukoyaka_);
  menuFile_->addAction(cyberpunkPoster_);
  menuFile_->addAction(JohnySilverhand_);
  menuFile_->addAction(nightCity_);

  connect(load_, &QAction::triggered, this, &MainWindow::LoadFile);
  connect(save_, &QAction::triggered, this, &MainWindow::SaveFile);
  connect(winx_, &QAction::triggered, this, &MainWindow::DrawWinx);
  connect(kukoyaka_, &QAction::triggered, this, &MainWindow::DrawKukoyaka);
  connect(cyberpunkPoster_, &QAction::triggered, this,
          &MainWindow::DrawCuberpunkPoster);
  connect(JohnySilverhand_, &QAction::triggered, this,
          &MainWindow::DrawJohnySilverhand);
  connect(nightCity_, &QAction::triggered, this, &MainWindow::DrawNightcity);
}

void MainWindow::SetupMenuEdit() {
  menuEdit_ = new QMenu(Const::Name::kMenuEdit, menuBar_);
  menuBar_->addMenu(menuEdit_);

  reset_ = new QAction(QIcon(":icons/resources/reset.png"),
                       Const::Name::MenuAct::kReset, menuEdit_);
  reset_->setShortcut(QKeySequence(Const::Shortcut::kActionReset));
  undo_ = new QAction(QIcon(":icons/resources/undo.png"),
                      Const::Name::MenuAct::kUndo, menuEdit_);
  undo_->setShortcut(QKeySequence(Const::Shortcut::kActionUndo));

  simpleFilter_ = new QAction(QIcon(":icons/resources/simple_filter.png"),
                              Const::Name::MenuAct::kSimpleFilter, menuEdit_);
  convolutionFilter_ =
      new QAction(QIcon(":icons/resources/kernel_filter.png"),
                  Const::Name::MenuAct::kConvolutionFilter, menuEdit_);
  colorCorrection_ =
      new QAction(QIcon(":icons/resources/color_correction.png"),
                  Const::Name::MenuAct::kColorCorrection, menuEdit_);

  menuEdit_->addAction(simpleFilter_);
  menuEdit_->addSeparator();
  menuEdit_->addAction(convolutionFilter_);
  menuEdit_->addSeparator();
  menuEdit_->addAction(colorCorrection_);
  menuEdit_->addSeparator();
  menuEdit_->addAction(reset_);
  menuEdit_->addAction(undo_);
  undo_->setEnabled(false);
  menuEdit_->setEnabled(false);

  connect(reset_, &QAction::triggered, this, &MainWindow::Reset);
  connect(undo_, &QAction::triggered, this, &MainWindow::CancelOperation);
  connect(simpleFilter_, &QAction::triggered, this,
          &MainWindow::StartOperatingSimpleFilter);
  connect(convolutionFilter_, &QAction::triggered, this,
          &MainWindow::StartOperatingKernelFilter);
  connect(colorCorrection_, &QAction::triggered, this,
          &MainWindow::StartOperatingColorCorrection);
}

void MainWindow::SetupMenuHelp() {
  menuHelp_ = new QMenu(Const::Name::kMenuHelp, menuBar_);
  menuBar_->addMenu(menuHelp_);

  controls_ = new QAction(QIcon(":icons/resources/help.png"),
                          Const::Name::MenuAct::kControls, menuHelp_);
  controls_->setShortcut(QKeySequence(Const::Shortcut::kActionControls));

  menuHelp_->addAction(controls_);

  connect(controls_, &QAction::triggered, this, &MainWindow::ShowControlsHelp);
}
