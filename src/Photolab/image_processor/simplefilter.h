#ifndef SIMPLEFILTER_H
#define SIMPLEFILTER_H

#include <future>
#include <string>

#include "channel.h"
#include "libs/matrix/matrix.h"
#include "runner_interface.h"

using Task = std::packaged_task<void()>;
using TaskChan = Channel<Task>;
using uchar = unsigned char;
using BytesBGRA = uchar[4];
using PixBytes = Matrix<uchar, MatrixType::NoCopy>;
using ConstPixBytes = Matrix<const uchar, MatrixType::NoCopy>;

class SimpleFilter {
public:
  explicit SimpleFilter(IRunner &runner) : runner_{runner} {}

  bool MakeNegative(ConstPixBytes prevPixels, PixBytes lastPixels,
                    size_t sizeBytes);
  bool MakeBlackAndWhite(ConstPixBytes prevPixels, PixBytes lastPixels,
                         size_t sizeBytes, std::string param);
  bool MakeToning(ConstPixBytes prevPixels, PixBytes lastPixels,
                  size_t sizeBytes, const BytesBGRA color);

  static constexpr const char *kNegative{"Negative"};
  static constexpr const char *kBlackAndWhite{"Black and White"};
  static constexpr const char *kToning{"Toning"};

private:
  static constexpr size_t kBlueIdx{0};
  static constexpr size_t kGreenIdx{1};
  static constexpr size_t kRedIdx{2};
  static constexpr size_t kAlphaIdx{3};

  static constexpr const char *kBlue{"Blue"};
  static constexpr const char *kGreen{"Green"};
  static constexpr const char *kRed{"Red"};
  static constexpr const char *kAverage{"Average"};
  static constexpr const char *kBrightness{"Brightness"};
  static constexpr const char *kDesaturation{"Desaturation"};

  IRunner &runner_;

  uchar MakeChanTone(uchar chan, uchar tone, uchar alpha);
  void MakeAveragePixel(ConstPixBytes &prevPixels, PixBytes &lastPixels,
                        size_t idx);
  void MakeByBrightnessPixel(ConstPixBytes &prevPixels, PixBytes &lastPixels,
                             size_t idx);
  void MakeByDesaturationPixel(ConstPixBytes &prevPixels, PixBytes &lastPixels,
                               size_t idx);
  void MakeByBlueChanPixel(ConstPixBytes &prevPixels, PixBytes &lastPixels,
                           size_t idx);
  void MakeByGreenChanPixel(ConstPixBytes &prevPixels, PixBytes &lastPixels,
                            size_t idx);
  void MakeByRedChanPixel(ConstPixBytes &prevPixels, PixBytes &lastPixels,
                          size_t idx);
};

#endif // SIMPLEFILTER_H
