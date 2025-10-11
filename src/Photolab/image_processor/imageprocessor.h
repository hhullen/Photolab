#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <atomic>
#include <future>

#include "controller/controller.h"
#include "image_processor/channel.h"
#include "image_processor/colorcorrection.h"
#include "image_processor/kernelfilter.h"
#include "image_processor/runner.h"
#include "image_processor/simplefilter.h"

using Task = std::packaged_task<void()>;
using TaskChan = Channel<Task>;
using BytesBGRA = unsigned char[4];
using PixBytes = Matrix<unsigned char, MatrixType::NoCopy>;
using ConstPixBytes = Matrix<const unsigned char, MatrixType::NoCopy>;

class ImageProcessor : public IImageProcessor {
public:
  ImageProcessor();
  ~ImageProcessor();

  bool MakeSimpleFilter(ConstPixBytes prevPixels, PixBytes lastPixels,
                        size_t sizeInBytes, std::string filter,
                        std::string param, const BytesBGRA color) override;

  std::tuple<Matrix<int>, size_t> GetFilterKernel(std::string, size_t) override;

  bool MakeKernelFilter(ConstPixBytes prevPixels, PixBytes lastPixels,
                        size_t width, size_t height, Matrix<int> kernel,
                        size_t normalizer,
                        std::atomic<bool> &kernelStop) override;

  bool MakeSimpleColorCorrection(ConstPixBytes prevPixels, PixBytes lastPixels,
                                 size_t sizeInBytes, int brightness,
                                 int contrast) override;

  bool MakeHSVColorCorrection(ConstPixBytes prevPixels, PixBytes lastPixels,
                              size_t sizeInBytes, int h, int s, int v) override;

  bool MakeHSLColorCorrection(ConstPixBytes prevPixels, PixBytes lastPixels,
                              size_t sizeInBytes, int h, int s, int l) override;

private:
  Runner runner_;
  SimpleFilter simpleFilter_;
  KernelFilter kernelFilter_;
  ColorCorrection colorCorrection_;
};

#endif // IMAGEPROCESSOR_H
