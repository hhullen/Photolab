#ifndef KERNELFILTERDIALOG_H
#define KERNELFILTERDIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

#include "libs/matrix/matrix.h"

class MatrixCell : public QSpinBox {
  Q_OBJECT

 public:
  explicit MatrixCell(int col, int row, int value)
      : col_{col}, row_{row}, value_{value} {
    setValue(value);
    connect(this, &QSpinBox::editingFinished, this, &MatrixCell::Changed);
  }

  int GetCol() { return col_; }
  int GetRow() { return row_; }
  void SetValue(int v) { setValue(v); }
  int GetValue() { return value(); }

 signals:
  void CellChanged(int col, int row, int value);

 private:
  int col_, row_, value_;

  void Changed() {
    int newValue = value();
    if (newValue != value_) {
      emit CellChanged(col_, row_, newValue);
    }
    value_ = newValue;
  }
};

class KernelFilterDialog : public QDialog {
  Q_OBJECT

 public:
  explicit KernelFilterDialog(QWidget *parent, QMenuBar *menuBar);
  ~KernelFilterDialog();
  void SetFilterOpts(QString name, Matrix<int> &kernel, size_t kernelSize);
  int GetCurrentArealSize();
  void SetButtonsEnebled(bool);
  std::tuple<Matrix<int>, size_t> GetCustomKernelOpts();

  void Show();

 signals:
  void FilterChanged(QString);
  void Canceled();
  void Accepted();

 private:
  QSpinBox *matrixSizeSetter_;
  QComboBox *filterSelector_;
  QLabel *normalizerValue_;
  QPushButton *reset_;
  QMenuBar *menuBar_;
  QDialogButtonBox *dialogButtons_;

  std::vector<MatrixCell *> cells_;
  QGridLayout *matrixLayout_;
  QWidget *matrixWidget_;

  bool closed_{false};

  void SetupFilterSelector();
  void SetupMatrixSetter();
  void SetupMatrixOptPanel();
  void SetupDialogButtons();
  void SetMatrixArea(int size);
  void SetMatrixValues(Matrix<int> &, size_t, bool);
  QString MakeNormalizerLabelText(size_t);
  void ChangeMatrixArea();
  void ChangeMatrixValues(int, int, int);
  void SetReadOnly(bool);
  void ResetMatrix();

  void closeEvent(QCloseEvent *) override;
  void Ok();
  void Cancel();
};

#endif  // KERNELFILTERDIALOG_H
