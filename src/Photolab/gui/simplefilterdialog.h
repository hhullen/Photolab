#ifndef SIMPLEFILTERDIALOG_H
#define SIMPLEFILTERDIALOG_H

#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedLayout>
#include <QStyle>
#include <QVBoxLayout>
#include <QWidget>

#include "qmenubar.h"

class SimpleFilterDialog : public QDialog {
  Q_OBJECT

 public:
  explicit SimpleFilterDialog(QWidget *parent, QMenuBar *menuBar);
  ~SimpleFilterDialog();

  void Show();

 signals:
  void MakeFilter(QString filter = "", QString channel = "",
                  QColor color = QColor());
  void Canceled();
  void Accepted();

 private:
  QMenuBar *menuBar_;

  // QComboBox *filterSelector_;
  QStackedLayout *filterSettingsLayout_;
  QWidget *filterSettings_;
  QWidget *emptyWidget_;

  QWidget *channelSelector_;
  QComboBox *channelSelectorBox_;

  QWidget *colorSelector_;
  QPushButton *colorDialogButton_;
  QColorDialog *colorDialog_;
  QColor currentColor_;

  bool closed_{false};

  void SetupFilterSelector();
  void SetupFilterSettings();
  void SetupChannelSelector();
  void SetupColorSelector();
  void SetupDialogButtons();
  void SelectFilter(QString);
  void MakeBlackAndWhiteByChannel(QString);
  void ClearFilterSettingsWidget();
  QString GetColorSelectorButtonStylesheet(QColor);
  void OperateNewColor(QColor);

  void OpenColorPicker();
  void CloseColorPicker();

  void closeEvent(QCloseEvent *) override;
  void Ok();
  void Cancel();
};

#endif  // SIMPLEFILTERDIALOG_H
