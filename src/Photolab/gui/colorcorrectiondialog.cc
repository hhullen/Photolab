#include "colorcorrectiondialog.h"

#include "gui/const.h"

namespace ConstNum = Const::Digit::ColorCorrection;

ColorCorrectionDialog::ColorCorrectionDialog(QWidget *parent, QMenuBar *menuBar)
    : QDialog{parent}, menuBar_{menuBar} {
  setLayout(new QVBoxLayout(this));
  layout()->setAlignment(Qt::Alignment(Qt::AlignmentFlag::AlignTop));

  correctSelector_ = new QComboBox(this);
  correctSelector_->addItem(Const::Name::ColorCorrection::kModeSimple);
  correctSelector_->addItem(Const::Name::ColorCorrection::kModeHSV);
  correctSelector_->addItem(Const::Name::ColorCorrection::kModeHSL);
  connect(correctSelector_, &QComboBox::currentTextChanged, this,
          &::ColorCorrectionDialog::ShooseCorrectionMode);
  layout()->addWidget(correctSelector_);

  adjustersPanel_ = new QWidget(this);
  layout()->addWidget(adjustersPanel_);
  QDialogButtonBox *dialogButtons =
      new QDialogButtonBox(Qt::Orientation::Horizontal, this);
  dialogButtons->addButton(QDialogButtonBox::StandardButton::Ok);
  dialogButtons->addButton(QDialogButtonBox::StandardButton::Cancel);
  layout()->addWidget(dialogButtons);
  connect(dialogButtons, &QDialogButtonBox::accepted, this,
          &ColorCorrectionDialog::Ok);
  connect(dialogButtons, &QDialogButtonBox::rejected, this,
          &ColorCorrectionDialog::Cancel);

  adjustersPanelLayout_ = new QStackedLayout(adjustersPanel_);
  adjustersPanel_->setLayout(adjustersPanelLayout_);

  SetupSimpleMode();
  SetupHSVMode();
  SetupHSLMode();

  setFixedHeight(Const::Digit::ColorCorrection::kDialogHeight);
  setFixedWidth(sizeHint().width());
  menuBar_->setDisabled(true);
}

ColorCorrectionDialog::~ColorCorrectionDialog() { menuBar_->setEnabled(true); }

void ColorCorrectionDialog::ShooseCorrectionMode(QString mode) {
  if (mode == Const::Name::ColorCorrection::kModeSimple) {
    adjustersPanelLayout_->setCurrentWidget(simpleMode_);
    emitSimpleValues_();
  } else if (mode == Const::Name::ColorCorrection::kModeHSL) {
    adjustersPanelLayout_->setCurrentWidget(modeHSL_);
    emitHSLValues_();
  } else if (mode == Const::Name::ColorCorrection::kModeHSV) {
    adjustersPanelLayout_->setCurrentWidget(modeHSV_);
    emitHSVValues_();
  }
}

void ColorCorrectionDialog::SetupSimpleMode() {
  simpleMode_ = new QWidget(adjustersPanel_);
  adjustersPanelLayout_->addWidget(simpleMode_);
  simpleMode_->setLayout(new QHBoxLayout);

  Adjuster *brightnessSlider =
      new Adjuster(simpleMode_, Qt::Orientation::Vertical,
                   new QLabel(Const::Name::ColorCorrection::kParamBrightness));
  brightnessSlider->SetRange(ConstNum::kBrightnessMin,
                             ConstNum::kBrightnessMax);
  brightnessSlider->SetValue(ConstNum::kBrightnessDefault);
  simpleMode_->layout()->addWidget(brightnessSlider);

  Adjuster *contrastSlider =
      new Adjuster(simpleMode_, Qt::Orientation::Vertical,
                   new QLabel(Const::Name::ColorCorrection::kParamContrast));

  contrastSlider->SetRange(ConstNum::kContrastMin, ConstNum::kContrastMax);
  contrastSlider->SetValue(ConstNum::kContrastDefault);
  simpleMode_->layout()->addWidget(contrastSlider);

  emitSimpleValues_ = [this, brightnessSlider, contrastSlider]() {
    emit MakeSimple(brightnessSlider->GetValue(),
                    contrastSlider->GetValue() + (-ConstNum::kContrastMin));
  };

  connect(brightnessSlider, &Adjuster::ValueChanged, this,
          [this]() { emitSimpleValues_(); });
  connect(contrastSlider, &Adjuster::ValueChanged, this,
          [this]() { emitSimpleValues_(); });
}

void ColorCorrectionDialog::SetupHSVMode() {
  modeHSV_ = new QWidget(adjustersPanel_);
  adjustersPanelLayout_->addWidget(modeHSV_);
  modeHSV_->setLayout(new QHBoxLayout);

  Adjuster *hueSlider =
      new Adjuster(modeHSV_, Qt::Orientation::Vertical,
                   new QLabel(Const::Name::ColorCorrection::kParamHue));
  hueSlider->SetRange(ConstNum::kHueHSVMin, ConstNum::kHueHSVMax);
  hueSlider->SetValue(ConstNum::kHueHSVDefault);
  modeHSV_->layout()->addWidget(hueSlider);

  Adjuster *saturationSlider =
      new Adjuster(modeHSV_, Qt::Orientation::Vertical,
                   new QLabel(Const::Name::ColorCorrection::kParamSaturation));
  saturationSlider->SetRange(ConstNum::kSaturationHSVMin,
                             ConstNum::kSaturationHSVMax);
  saturationSlider->SetValue(ConstNum::kSaturationHSVDefault);
  modeHSV_->layout()->addWidget(saturationSlider);

  Adjuster *valueSlider =
      new Adjuster(modeHSV_, Qt::Orientation::Vertical,
                   new QLabel(Const::Name::ColorCorrection::kParamValue));
  valueSlider->SetRange(ConstNum::kValueHSVMin, ConstNum::kValueHSVMax);
  valueSlider->SetValue(ConstNum::kValueHSVDefault);
  modeHSV_->layout()->addWidget(valueSlider);

  emitHSVValues_ = [this, hueSlider, saturationSlider, valueSlider]() {
    emit MakeHSV(hueSlider->GetValue(), saturationSlider->GetValue(),
                 valueSlider->GetValue());
  };

  connect(hueSlider, &Adjuster::ValueChanged, this,
          [this]() { emitHSVValues_(); });
  connect(saturationSlider, &Adjuster::ValueChanged, this,
          [this]() { emitHSVValues_(); });
  connect(valueSlider, &Adjuster::ValueChanged, this,
          [this]() { emitHSVValues_(); });
}

void ColorCorrectionDialog::SetupHSLMode() {
  modeHSL_ = new QWidget(adjustersPanel_);
  adjustersPanelLayout_->addWidget(modeHSL_);
  modeHSL_->setLayout(new QHBoxLayout);

  Adjuster *hueSlider =
      new Adjuster(modeHSL_, Qt::Orientation::Vertical,
                   new QLabel(Const::Name::ColorCorrection::kParamHue));
  hueSlider->SetRange(ConstNum::kHueHSLMin, ConstNum::kHueHSLMax);
  hueSlider->SetValue(ConstNum::kHueHSLDefault);
  modeHSL_->layout()->addWidget(hueSlider);

  Adjuster *saturationSlider =
      new Adjuster(modeHSL_, Qt::Orientation::Vertical,
                   new QLabel(Const::Name::ColorCorrection::kParamSaturation));
  saturationSlider->SetRange(ConstNum::kSaturationHSLMin,
                             ConstNum::kSaturationHSLMax);
  saturationSlider->SetValue(ConstNum::kSaturationHSLDefault);
  modeHSL_->layout()->addWidget(saturationSlider);

  Adjuster *lightnessSlider =
      new Adjuster(modeHSL_, Qt::Orientation::Vertical,
                   new QLabel(Const::Name::ColorCorrection::kParamLightness));
  lightnessSlider->SetRange(ConstNum::kLightnessHSLMin,
                            ConstNum::kLightnessHSLMax);
  lightnessSlider->SetValue(ConstNum::kLightnessHSLDefault);
  modeHSL_->layout()->addWidget(lightnessSlider);

  emitHSLValues_ = [this, hueSlider, saturationSlider, lightnessSlider]() {
    emit MakeHSL(hueSlider->GetValue(),
                 saturationSlider->GetValue() - ConstNum::kSaturationHSLMin,
                 lightnessSlider->GetValue() - ConstNum::kLightnessHSLMin);
  };

  connect(hueSlider, &Adjuster::ValueChanged, this,
          [this]() { emitHSLValues_(); });
  connect(saturationSlider, &Adjuster::ValueChanged, this,
          [this]() { emitHSLValues_(); });
  connect(lightnessSlider, &Adjuster::ValueChanged, this,
          [this]() { emitHSLValues_(); });
}

void ColorCorrectionDialog::closeEvent(QCloseEvent *) {
  if (!closed_) {
    Cancel();
  }
}

void ColorCorrectionDialog::Ok() {
  emit Accepted();
  closed_ = true;
  close();
}

void ColorCorrectionDialog::Cancel() {
  emit Canceled();
  closed_ = true;
  close();
}
