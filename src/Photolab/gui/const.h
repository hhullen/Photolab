#ifndef CONST_H
#define CONST_H

#include <QColor>
#include <QSize>

namespace Const {

using CStr = const char *const;

namespace Digit {
constexpr float kMaxScale{30};
constexpr float kMinScale{0.05};
constexpr float kCutterWidth{4};
constexpr int kCutterHandlerRadius{7};
constexpr int kSizeTitlePt{21};
constexpr int kMessageDelayMs{7000};
constexpr int kMaxKernelSize{17};
constexpr int kMinKernelSize{3};
constexpr int kMaxkernelCellValue{12870};
constexpr int kPocessIndicatorWidth{500};
constexpr int kPocessIndicatorShiftDelayMs{16};
constexpr int kPocessIndicatorShift{100};

namespace ColorCorrection {
constexpr int kDialogHeight{450};
constexpr int kBrightnessMin{-255};
constexpr int kBrightnessMax{255};
constexpr int kBrightnessDefault{0};
constexpr int kContrastMin{-512};
constexpr int kContrastMax{512};
constexpr int kContrastDefault{0};

constexpr int kHueHSVMin{-180};
constexpr int kHueHSVMax{180};
constexpr int kHueHSVDefault{0};
constexpr int kSaturationHSVMin{0};
constexpr int kSaturationHSVMax{1000};
constexpr int kSaturationHSVDefault{1000};
constexpr int kValueHSVMin{0};
constexpr int kValueHSVMax{1000};
constexpr int kValueHSVDefault{1000};

constexpr int kHueHSLMin{-180};
constexpr int kHueHSLMax{180};
constexpr int kHueHSLDefault{0};
constexpr int kSaturationHSLMin{-1000};
constexpr int kSaturationHSLMax{1000};
constexpr int kSaturationHSLDefault{0};
constexpr int kLightnessHSLMin{-1000};
constexpr int kLightnessHSLMax{1000};
constexpr int kLightnessHSLDefault{0};

} // namespace ColorCorrection

} // namespace Digit

namespace Label {
constexpr CStr kOpenFileDialog{"Open image"};
constexpr CStr kSelectFolderFileDialog{"Select folder to save"};
constexpr CStr kConstolsWindow{"Controls"};
constexpr CStr kColorSelector{"Color:"};
constexpr CStr kChannelSelector{"By parameter:"};
constexpr CStr kKernelNormalizer{"Kernel normalizer:"};

} // namespace Label

namespace Size {
constexpr QSize kMainWindowMininum{640, 480};
constexpr QSize kControlsHelp{600, 600};
constexpr QSize kColorSelectorButton{40, 40};
} // namespace Size

namespace Color {
constexpr QColor kTileLight{255, 255, 255};
constexpr QColor kTileDark{230, 230, 230};
constexpr QColor kCutterColor{240, 30, 255};
} // namespace Color

namespace Msg {
constexpr CStr kFailedLoadFile{"File hasn't been loaded"};
constexpr CStr kSuccessfulyLoadedFile{"File loaded successfuly"};
constexpr CStr kFailedSaveFile{"Failed to save:"};
constexpr CStr kSuccessfulySavedFile{"Saved"};
constexpr CStr kFailedLoadInfo{"Failed loading information"};
constexpr CStr kFailedOperatingHistory{"Failed operating history stack"};
constexpr CStr kFailedFailedGettingKernel{"Failed getting kernel"};
} // namespace Msg

namespace Name {
constexpr CStr kDialogAccept{"ok"};
constexpr CStr kDialogReject{"cancel"};
constexpr CStr kMenuFile{"File"};
constexpr CStr kMenuEdit{"Edit"};
constexpr CStr kMenuHelp{"Help"};
constexpr CStr kResetKernel{"Reset"};

namespace MenuAct {
constexpr CStr kLoad{"Load"};
constexpr CStr kSave{"Save"};
constexpr CStr kWinx{"Winx"};
constexpr CStr kKukoyaka{"Кукояка"};
constexpr CStr kCyberPunkPoster{"Cyberpunk 2077"};
constexpr CStr kJohnySilverhand{"Johny Silverhand"};
constexpr CStr kNightCity{"Night City"};
constexpr CStr kReset{"Reset"};
constexpr CStr kUndo{"Undo"};
constexpr CStr kSimpleFilter{"Simple filter"};
constexpr CStr kConvolutionFilter{"Convolution filter"};
constexpr CStr kColorCorrection{"Color correction"};
constexpr CStr kControls{"Controls"};
} // namespace MenuAct

namespace Filter {
namespace Simple {
constexpr CStr kOptNegative{"Negative"};
constexpr CStr kOptBlackAndWhite{"Black and White"};
constexpr CStr kOptToning{"Toning"};
constexpr CStr kParamRed{"Red"};
constexpr CStr kParamGreen{"Green"};
constexpr CStr kParamBlue{"Blue"};
constexpr CStr kParamAverage{"Average"};
constexpr CStr kParamBrightness{"Brightness"};
constexpr CStr kParamDesaturation{"Desaturation"};
} // namespace Simple

namespace Kernel {
constexpr CStr kOptEmboss{"Emboss"};
constexpr CStr kOptSharpen{"Sharpen"};
constexpr CStr kOptBoxBlur{"Box Blur"};
constexpr CStr kOptGauss{"Gauss Blur"};
constexpr CStr kOptLaplacian{"Laplacian"};
constexpr CStr kSobelRight{"Sobel Right"};
constexpr CStr kSobelLeft{"Sobel Left"};
constexpr CStr kOptCustom{"Custom"};
} // namespace Kernel
} // namespace Filter

namespace ColorCorrection {
constexpr CStr kModeSimple{"Simple"};
constexpr CStr kModeHSV{"HSV"};
constexpr CStr kModeHSL{"HSL"};
constexpr CStr kParamBrightness{"Brightness"};
constexpr CStr kParamContrast{"Contrast"};
constexpr CStr kParamHue{"Hue"};
constexpr CStr kParamSaturation{"Saturation"};
constexpr CStr kParamValue{"Value"};
constexpr CStr kParamLightness{"Lightness"};
} // namespace ColorCorrection

} // namespace Name

namespace Shortcut {
constexpr CStr kActionLoad{"Ctrl+o"};
constexpr CStr kActionSave{"Ctrl+s"};
constexpr CStr kActionReset{"Ctrl+Shift+z"};
constexpr CStr kActionUndo{"Ctrl+z"};
constexpr CStr kActionControls{"Ctrl+h"};
} // namespace Shortcut

namespace Resource {
constexpr CStr kFilePathWinx{":photos/resources/winx.jpg"};
constexpr CStr kFilePathKukoyaka{":photos/resources/kukoyaka.png"};
constexpr CStr kFilePathCuberpunkPoster{
    ":/photos/resources/Cyberpunk_2077.jpg"};
constexpr CStr kFilePathJohnySilverhand{
    ":/photos/resources/Johnny_Silverhand.jpg"};
constexpr CStr kFilePathNightCity{":/photos/resources/Night_City.jpg"};
constexpr CStr kFilePathControlsHelp{":htmls/resources/controls_help.html"};
} // namespace Resource

namespace Constrain {
constexpr CStr kImageFormats{"*.png *.jpg *.jpeg *.bmp"};
}

namespace Style {
constexpr CStr kColorSelector{"QWidget:hover {"
                              "background-color: rgb(custom-bg);"
                              "border: 3px solid rgb(200, 200, 200);"
                              "border-bottom-color: rgb(240, 30, 255);"
                              "border-top-color: rgb(240, 30, 255);"
                              "border-left-color: rgb(255, 255, 255);"
                              "border-right-color: rgb(255, 255, 255);"
                              "border-radius: 20px;"
                              "}"
                              "QWidget {"
                              "background-color: rgb(custom-bg);"
                              "border: 3px solid rgb(200, 200, 200);"
                              "border-left-color: #f01effff;"
                              "border-right-color: rgb(240, 30, 255);"
                              "border-top-color: rgb(255, 255, 255);"
                              "border-bottom-color: rgb(255, 255, 255);"
                              "border-radius: 20px;"
                              "}"};
constexpr CStr kKernelCell{"QWidget:hover {"
                           "background-color: rgb(100, 100, 100);"
                           "border: 1px solid rgb(150, 120, 170);"
                           "color: rgb(255, 255, 255);"
                           "}"
                           "QWidget {"
                           "background-color: rgb(30, 30, 30);"
                           "border: 1px solid rgb(150, 120, 170);"
                           "color: rgb(255, 255, 255);"
                           "}"
                           "QWidget:disabled {"
                           "background-color: rgb(20, 20, 20);"
                           "border: 1px solid rgb(70, 0, 90);"
                           "color: rgb(255, 255, 255);"
                           "}"};
} // namespace Style

} // namespace Const

#endif // CONST_H
