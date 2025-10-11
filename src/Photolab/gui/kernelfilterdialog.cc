#include "kernelfilterdialog.h"

#include "gui/const.h"

namespace ConstOpt = Const::Name::Filter::Kernel;

KernelFilterDialog::KernelFilterDialog(QWidget *parent, QMenuBar *menuBar)
    : QDialog{parent}, menuBar_{menuBar} {
  setParent(parent, Qt::Dialog);

  setLayout(new QVBoxLayout(this));
  layout()->setAlignment(Qt::Alignment(Qt::AlignmentFlag::AlignTop));

  reset_ = new QPushButton(Const::Name::kResetKernel);
  normalizerValue_ = new QLabel(MakeNormalizerLabelText(1));
  filterSelector_ = new QComboBox(this);
  matrixWidget_ = new QWidget(this);
  matrixLayout_ = new QGridLayout();
  matrixSizeSetter_ = new QSpinBox();

  SetupFilterSelector();
  SetupMatrixSetter();
  SetupMatrixOptPanel();
  SetupDialogButtons();
}

KernelFilterDialog::~KernelFilterDialog() { menuBar_->setDisabled(false); }

void KernelFilterDialog::Show() {
  menuBar_->setDisabled(true);
  show();
  setFixedSize(size());
  emit filterSelector_->currentTextChanged(filterSelector_->currentText());
}

void KernelFilterDialog::SetupFilterSelector() {
  filterSelector_->addItem(ConstOpt::kOptCustom);
  filterSelector_->addItem(ConstOpt::kOptEmboss);
  filterSelector_->addItem(ConstOpt::kOptSharpen);
  filterSelector_->addItem(ConstOpt::kOptBoxBlur);
  filterSelector_->addItem(ConstOpt::kOptLaplacian);
  filterSelector_->addItem(ConstOpt::kOptGauss);
  filterSelector_->addItem(ConstOpt::kSobelRight);
  filterSelector_->addItem(ConstOpt::kSobelLeft);

  layout()->addWidget(filterSelector_);
  connect(filterSelector_, &QComboBox::currentTextChanged, this,
          [this](QString name) { emit FilterChanged(name); });
}

void KernelFilterDialog::SetupMatrixSetter() {
  matrixLayout_->setSpacing(0);
  matrixLayout_->setContentsMargins(0, 0, 0, 0);
  matrixWidget_->setLayout(matrixLayout_);

  for (int i = 0; i < Const::Digit::kMaxKernelSize; i++) {
    for (int j = 0; j < Const::Digit::kMaxKernelSize; j++) {
      MatrixCell *cell = nullptr;
      if (i == Const::Digit::kMaxKernelSize / 2 &&
          j == Const::Digit::kMaxKernelSize / 2) {
        cell = new MatrixCell(i, j, 1);
      } else {
        cell = new MatrixCell(i, j, 0);
      }

      cell->setFixedSize(cell->sizeHint().height() + 20,
                         cell->sizeHint().height());
      cell->setAlignment(Qt::AlignmentFlag::AlignCenter);
      cell->setButtonSymbols(QSpinBox::NoButtons);
      cell->setMaximum(Const::Digit::kMaxkernelCellValue);
      cell->setMinimum(-Const::Digit::kMaxkernelCellValue);
      cell->setStyleSheet(Const::Style::kKernelCell);
      cells_.emplace_back(cell);
      matrixLayout_->addWidget(cell, i, j);

      connect(cell, &MatrixCell::CellChanged, this,
              &KernelFilterDialog::ChangeMatrixValues);
    }
  }

  layout()->addWidget(matrixWidget_);
}

void KernelFilterDialog::SetupMatrixOptPanel() {
  QWidget *sizeSetterWidget = new QWidget(this);
  QHBoxLayout *sizeSetterLayout = new QHBoxLayout(sizeSetterWidget);

  sizeSetterLayout->setAlignment(Qt::AlignmentFlag::AlignCenter);

  matrixSizeSetter_->setMaximum(Const::Digit::kMaxKernelSize);
  matrixSizeSetter_->setMinimum(Const::Digit::kMinKernelSize);
  matrixSizeSetter_->setSingleStep(2);
  matrixSizeSetter_->setValue(Const::Digit::kMinKernelSize);
  sizeSetterLayout->addWidget(normalizerValue_);
  sizeSetterLayout->addWidget(matrixSizeSetter_);
  sizeSetterLayout->addWidget(reset_);
  reset_->setFocusPolicy(Qt::FocusPolicy::NoFocus);

  connect(matrixSizeSetter_, &QSpinBox::valueChanged, this,
          &KernelFilterDialog::ChangeMatrixArea);
  connect(reset_, &QPushButton::released, this,
          &KernelFilterDialog::ResetMatrix);

  layout()->addWidget(sizeSetterWidget);
}

void KernelFilterDialog::SetupDialogButtons() {
  dialogButtons_ = new QDialogButtonBox(Qt::Orientation::Horizontal, this);
  dialogButtons_->addButton(QDialogButtonBox::StandardButton::Ok);
  dialogButtons_->addButton(QDialogButtonBox::StandardButton::Cancel);
  layout()->addWidget(dialogButtons_);
  connect(dialogButtons_, &QDialogButtonBox::accepted, this,
          &KernelFilterDialog::Ok);
  connect(dialogButtons_, &QDialogButtonBox::rejected, this,
          &KernelFilterDialog::Cancel);
}

void KernelFilterDialog::SetFilterOpts(QString name, Matrix<int> &kernel,
                                       size_t normalizer) {
  bool resetOnSettingValues = true;
  if (name == ConstOpt::kOptCustom) {
    resetOnSettingValues = false;
    reset_->setEnabled(true);
    SetReadOnly(false);
  } else {
    reset_->setEnabled(false);
    SetReadOnly(true);
  }

  int areaSize = kernel.rows();
  matrixSizeSetter_->setValue(areaSize);
  SetMatrixArea(areaSize);
  SetMatrixValues(kernel, normalizer, resetOnSettingValues);
}

int KernelFilterDialog::GetCurrentArealSize() {
  return matrixSizeSetter_->value();
}

void KernelFilterDialog::SetButtonsEnebled(bool v) {
  dialogButtons_->setEnabled(v);
}

std::tuple<Matrix<int>, size_t> KernelFilterDialog::GetCustomKernelOpts() {
  int size = GetCurrentArealSize();
  int max = Const::Digit::kMaxKernelSize;
  int topLeft = max / 2 - size / 2;
  int downRight = topLeft + size;

  Matrix<int> matrix(size, size);
  size_t normalizer = 0;
  for (int i = topLeft; i < downRight; i++) {
    for (int j = topLeft; j < downRight; j++) {
      const int idx = i * max + j;
      int ii = i - topLeft;
      int jj = j - topLeft;
      matrix[ii][jj] = cells_[idx]->GetValue();
      normalizer += matrix[ii][jj];
    }
  }

  if (normalizer == 0) {
    normalizer = 1;
  }

  return {std::move(matrix), normalizer};
}

void KernelFilterDialog::SetMatrixArea(int size) {
  int max = Const::Digit::kMaxKernelSize;
  int topLeftCorner = max / 2 - size / 2;
  const QRect enabled(topLeftCorner, topLeftCorner, size, size);

  for (int i = 0; i < max; i++) {
    for (int j = 0; j < max; j++) {
      int idx = i * max + j;
      if (enabled.contains(i, j)) {
        cells_[idx]->setEnabled(true);
      } else {
        cells_[idx]->setEnabled(false);
      }
    }
  }
}

void KernelFilterDialog::SetMatrixValues(Matrix<int> &matrix, size_t normalizer,
                                         bool reset) {
  if (reset) {
    ResetMatrix();
  }
  int size = matrix.rows();
  int max = Const::Digit::kMaxKernelSize;
  int topLeft = max / 2 - size / 2;
  int downRight = topLeft + size;

  for (int i = topLeft; i < downRight; i++) {
    for (int j = topLeft; j < downRight; j++) {
      const int idx = i * max + j;
      int ii = i - topLeft;
      int jj = j - topLeft;
      cells_[idx]->blockSignals(true);
      cells_[idx]->SetValue(matrix[ii][jj]);
      cells_[idx]->blockSignals(false);
    }
  }
  cells_[0][0].SetValue(cells_[0][0].value());
  normalizerValue_->setText(MakeNormalizerLabelText(normalizer));
}

QString KernelFilterDialog::MakeNormalizerLabelText(size_t v) {
  char buf[20];
  std::sprintf(buf, "%lld", v);
  return QString(Const::Label::kKernelNormalizer) + " " + buf;
}

void KernelFilterDialog::ChangeMatrixArea() {
  int value = matrixSizeSetter_->value();
  if ((value & 1) == 0) {
    matrixSizeSetter_->setValue(value + 1);
  }

  emit FilterChanged(filterSelector_->currentText());
}

void KernelFilterDialog::ChangeMatrixValues(int, int, int) {
  if (filterSelector_->currentText() == ConstOpt::kOptCustom) {
    emit FilterChanged(filterSelector_->currentText());
  }
}

void KernelFilterDialog::SetReadOnly(bool flag) {
  for (int i = 0; i < Const::Digit::kMaxKernelSize; i++) {
    for (int j = 0; j < Const::Digit::kMaxKernelSize; j++) {
      int cellidx = i * Const::Digit::kMaxKernelSize + j;
      cells_[cellidx]->setReadOnly(flag);
    }
  }
}

void KernelFilterDialog::ResetMatrix() {
  for (int i = 0; i < Const::Digit::kMaxKernelSize; i++) {
    for (int j = 0; j < Const::Digit::kMaxKernelSize; j++) {
      int cellidx = i * Const::Digit::kMaxKernelSize + j;
      cells_[cellidx]->blockSignals(true);
      cells_[cellidx]->SetValue(0);
      cells_[cellidx]->blockSignals(false);
    }
  }
  int center = Const::Digit::kMaxKernelSize * Const::Digit::kMaxKernelSize / 2;
  cells_[center]->SetValue(1);
}

void KernelFilterDialog::closeEvent(QCloseEvent *) {
  if (!closed_) {
    Cancel();
  }
}

void KernelFilterDialog::Ok() {
  emit Accepted();
  closed_ = true;
  close();
}

void KernelFilterDialog::Cancel() {
  emit Canceled();
  closed_ = true;
  close();
}
