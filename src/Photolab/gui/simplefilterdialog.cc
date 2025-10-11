#include "simplefilterdialog.h"

#include "const.h"

SimpleFilterDialog::SimpleFilterDialog(QWidget *parent, QMenuBar *menuBar)
    : menuBar_{menuBar} {
  setParent(parent, Qt::Dialog);

  setLayout(new QVBoxLayout(this));
  layout()->setAlignment(Qt::Alignment(Qt::AlignmentFlag::AlignTop));
  layout()->setSizeConstraint(QLayout::SetFixedSize);

  SetupFilterSelector();
  SetupFilterSettings();
  SetupChannelSelector();
  SetupColorSelector();
  SetupDialogButtons();

  setFixedSize(size());

  menuBar_->setDisabled(true);
}

SimpleFilterDialog::~SimpleFilterDialog() { menuBar_->setEnabled(true); }

void SimpleFilterDialog::Show() {
  show();
  SelectFilter(Const::Name::Filter::Simple::kOptNegative);
}

void SimpleFilterDialog::SetupFilterSelector() {
  QComboBox *filterSelector = new QComboBox(this);
  filterSelector->addItem(Const::Name::Filter::Simple::kOptNegative);
  filterSelector->addItem(Const::Name::Filter::Simple::kOptBlackAndWhite);
  filterSelector->addItem(Const::Name::Filter::Simple::kOptToning);
  layout()->addWidget(filterSelector);

  connect(filterSelector, &QComboBox::currentTextChanged, this,
          &SimpleFilterDialog::SelectFilter);
}

void SimpleFilterDialog::SetupFilterSettings() {
  filterSettings_ = new QWidget(this);
  filterSettings_->setWindowTitle(Const::Name::MenuAct::kSimpleFilter);
  filterSettingsLayout_ = new QStackedLayout;
  filterSettings_->setLayout(filterSettingsLayout_);
  filterSettingsLayout_->setAlignment(Qt::AlignmentFlag::AlignCenter);
  filterSettingsLayout_->setContentsMargins(5, 5, 5, 10);

  emptyWidget_ = new QWidget;

  filterSettingsLayout_->addWidget(emptyWidget_);
  layout()->addWidget(filterSettings_);
}

void SimpleFilterDialog::SetupChannelSelector() {
  channelSelector_ = new QWidget(filterSettings_);
  channelSelector_->setLayout(new QVBoxLayout);
  channelSelector_->layout()->setAlignment(Qt::AlignmentFlag::AlignCenter);

  QLabel *label = new QLabel(Const::Label::kChannelSelector, channelSelector_);
  channelSelector_->layout()->addWidget(label);

  channelSelectorBox_ = new QComboBox(filterSettings_);
  channelSelectorBox_->addItem(Const::Name::Filter::Simple::kParamRed);
  channelSelectorBox_->addItem(Const::Name::Filter::Simple::kParamGreen);
  channelSelectorBox_->addItem(Const::Name::Filter::Simple::kParamBlue);
  channelSelectorBox_->addItem(Const::Name::Filter::Simple::kParamAverage);
  channelSelectorBox_->addItem(Const::Name::Filter::Simple::kParamBrightness);
  channelSelectorBox_->addItem(Const::Name::Filter::Simple::kParamDesaturation);
  channelSelector_->layout()->addWidget(channelSelectorBox_);
  filterSettingsLayout_->addWidget(channelSelector_);

  connect(channelSelectorBox_, &QComboBox::currentTextChanged, this,
          &SimpleFilterDialog::MakeBlackAndWhiteByChannel);
}

void SimpleFilterDialog::SetupColorSelector() {
  colorSelector_ = new QWidget(filterSettings_);
  colorSelector_->setLayout(new QHBoxLayout);
  colorSelector_->layout()->setAlignment(Qt::AlignmentFlag::AlignCenter);

  colorDialog_ = new QColorDialog(colorSelector_);
  colorDialog_->setModal(true);

  QLabel *label = new QLabel(Const::Label::kColorSelector, colorSelector_);
  colorSelector_->layout()->addWidget(label);

  colorDialogButton_ = new QPushButton(colorSelector_);
  QString styleSheet =
      GetColorSelectorButtonStylesheet(colorDialog_->currentColor());
  colorDialogButton_->setStyleSheet(styleSheet);
  colorDialogButton_->setFixedSize(Const::Size::kColorSelectorButton);
  colorSelector_->layout()->addWidget(colorDialogButton_);

  filterSettingsLayout_->addWidget(colorSelector_);

  connect(colorDialogButton_, &QPushButton::clicked, this,
          &SimpleFilterDialog::OpenColorPicker);
  connect(colorDialog_, &QColorDialog::rejected, this,
          &SimpleFilterDialog::CloseColorPicker);
  connect(colorDialog_, &QColorDialog::currentColorChanged, this,
          &SimpleFilterDialog::OperateNewColor);
}

void SimpleFilterDialog::SetupDialogButtons() {
  QDialogButtonBox *dialogButtons =
      new QDialogButtonBox(Qt::Orientation::Horizontal, this);
  dialogButtons->addButton(QDialogButtonBox::StandardButton::Ok);
  dialogButtons->addButton(QDialogButtonBox::StandardButton::Cancel);
  layout()->addWidget(dialogButtons);
  connect(dialogButtons, &QDialogButtonBox::accepted, this,
          &SimpleFilterDialog::Ok);
  connect(dialogButtons, &QDialogButtonBox::rejected, this,
          &SimpleFilterDialog::Cancel);
}

void SimpleFilterDialog::closeEvent(QCloseEvent *) {
  if (!closed_) {
    Cancel();
  }
}

void SimpleFilterDialog::Ok() {
  emit Accepted();
  closed_ = true;
  close();
}

void SimpleFilterDialog::Cancel() {
  emit Canceled();
  closed_ = true;
  close();
}

void SimpleFilterDialog::SelectFilter(QString feature) {
  if (feature == Const::Name::Filter::Simple::kOptNegative) {
    filterSettingsLayout_->setCurrentWidget(emptyWidget_);
    emit MakeFilter(Const::Name::Filter::Simple::kOptNegative);

  } else if (feature == Const::Name::Filter::Simple::kOptBlackAndWhite) {
    filterSettingsLayout_->setCurrentWidget(channelSelector_);
    MakeBlackAndWhiteByChannel(channelSelectorBox_->currentText());

  } else if (feature == Const::Name::Filter::Simple::kOptToning) {
    filterSettingsLayout_->setCurrentWidget(colorSelector_);
    currentColor_ = colorDialog_->currentColor();
    OperateNewColor(currentColor_);
  }

  filterSettings_->layout()->invalidate();
  filterSettings_->adjustSize();
  adjustSize();
}

void SimpleFilterDialog::MakeBlackAndWhiteByChannel(QString param) {
  emit MakeFilter(Const::Name::Filter::Simple::kOptBlackAndWhite, param);
}

QString SimpleFilterDialog::GetColorSelectorButtonStylesheet(QColor bg) {
  QString sheet = Const::Style::kColorSelector;
  char v[15];
  std::sprintf(v, "%d, %d, %d", bg.red(), bg.green(), bg.blue());
  sheet.replace("custom-bg", QString(v));

  return sheet;
}

void SimpleFilterDialog::OperateNewColor(QColor color) {
  QString styleSheet = GetColorSelectorButtonStylesheet(color);
  colorDialogButton_->setStyleSheet(styleSheet);
  emit MakeFilter(Const::Name::Filter::Simple::kOptToning, "", color);
}

void SimpleFilterDialog::OpenColorPicker() {
  colorDialog_->show();
  emit colorDialog_->currentColorChanged(colorDialog_->currentColor());
}

void SimpleFilterDialog::CloseColorPicker() { OperateNewColor(currentColor_); }
