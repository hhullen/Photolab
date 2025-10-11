#ifndef COLORCORRECTIONDIALOG_H
#define COLORCORRECTIONDIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QStackedLayout>
#include <QVBoxLayout>

class Adjuster : public QWidget {
  Q_OBJECT

 public:
  explicit Adjuster(QWidget *parent, Qt::Orientation orient, QWidget *label)
      : QWidget{parent}, label_{label} {
    spinbox_ = new QSpinBox(this);
    spinbox_->setButtonSymbols(QSpinBox::NoButtons);
    spinbox_->setAlignment(Qt::AlignmentFlag::AlignCenter);

    slider_ = new QSlider(orient, this);

    setLayout(new QVBoxLayout());
    layout()->addWidget(spinbox_);
    layout()->addWidget(slider_);
    layout()->addWidget(label_);
    layout()->setAlignment(spinbox_, Qt::AlignmentFlag::AlignHCenter);
    layout()->setAlignment(slider_, Qt::AlignmentFlag::AlignHCenter);
    layout()->setAlignment(label_, Qt::AlignmentFlag::AlignHCenter);

    slider_->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    label_->setFocusPolicy(Qt::FocusPolicy::NoFocus);

    connect(slider_, &QSlider::sliderMoved, spinbox_, &QSpinBox::setValue);
    connect(slider_, &QSlider::sliderReleased, this,
            [this]() { spinbox_->setValue(slider_->value()); });
    connect(spinbox_, &QSpinBox::valueChanged, slider_,
            &QSlider::setSliderPosition);
    connect(spinbox_, &QSpinBox::valueChanged, this,
            [this](int v) { emit ValueChanged(v); });
  }
  void SetRange(int low, int high) {
    spinbox_->setRange(low, high);
    slider_->setRange(low, high);
  }

  void SetValue(int value) { spinbox_->setValue(value); }

  int GetValue() { return spinbox_->value(); }

 signals:
  void ValueChanged(int);

 private:
  QSpinBox *spinbox_;
  QSlider *slider_;
  QWidget *label_;
};

class ColorCorrectionDialog : public QDialog {
  Q_OBJECT
 public:
  ColorCorrectionDialog(QWidget *parent, QMenuBar *menuBar);

  ~ColorCorrectionDialog();

 signals:
  void MakeSimple(int, int);
  void MakeHSV(int, int, int);
  void MakeHSL(int, int, int);
  void Canceled();
  void Accepted();

 private:
  QMenuBar *menuBar_;

  QComboBox *correctSelector_;
  QWidget *adjustersPanel_;
  QStackedLayout *adjustersPanelLayout_;

  QWidget *simpleMode_;
  QWidget *modeHSV_;
  QWidget *modeHSL_;

  std::function<void()> emitSimpleValues_{nullptr};
  std::function<void()> emitHSVValues_{nullptr};
  std::function<void()> emitHSLValues_{nullptr};

  bool closed_{false};

  void ShooseCorrectionMode(QString);
  void SetupSimpleMode();
  void SetupHSVMode();
  void SetupHSLMode();

  void closeEvent(QCloseEvent *) override;
  void Ok();
  void Cancel();
};

#endif  // COLORCORRECTIONDIALOG_H
