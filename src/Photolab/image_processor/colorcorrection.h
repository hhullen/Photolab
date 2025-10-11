#ifndef COLORCORRECTION_H
#define COLORCORRECTION_H

#include <future>

#include "channel.h"
#include "libs/matrix/matrix.h"
#include "runner_interface.h"

using Task = std::packaged_task<void()>;
using TaskChan = Channel<Task>;
using uchar = unsigned char;
using PixBytes = Matrix<uchar, MatrixType::NoCopy>;
using ConstPixBytes = Matrix<const uchar, MatrixType::NoCopy>;

class ColorCorrection {
public:
  explicit ColorCorrection(IRunner &runner) : runner_{runner} {}

  bool MakeSimpleColorCorrection(ConstPixBytes prevPixels, PixBytes lastPixels,
                                 size_t sizeInBytes, int brightness,
                                 int contrast);

  bool MakeHSVColorCorrection(ConstPixBytes prevPixels, PixBytes lastPixels,
                              size_t sizeInBytes, int h, int s, int v);

  bool MakeHSLColorCorrection(ConstPixBytes prevPixels, PixBytes lastPixels,
                              size_t sizeInBytes, int h, int s, int l);

private:
  IRunner &runner_;

  static constexpr size_t kContrastScale{512};

  std::tuple<float, float, float> RGBToHSV(uchar r, uchar g, uchar b);
  std::tuple<uchar, uchar, uchar> HSVToRGB(float h, float s, float v);
  std::tuple<float, float, float> RGBToHSL(uchar r, uchar g, uchar b);
  std::tuple<uchar, uchar, uchar> HSLToRGB(float h, float s, float l);
};

#endif // COLORCORRECTION_H
