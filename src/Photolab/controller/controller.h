#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <gui/controller_interface.h>

#include <atomic>

using BytesBGRA = unsigned char[4];
using PixBytes = Matrix<unsigned char, MatrixType::NoCopy>;
using ConstPixBytes = Matrix<const unsigned char, MatrixType::NoCopy>;

class IImageProcessor {
public:
  IImageProcessor() = default;
  virtual ~IImageProcessor() = 0;

  virtual bool MakeSimpleFilter(ConstPixBytes prevPixels, PixBytes lastPixels,
                                size_t sizeInBytes, std::string filter,
                                std::string param, const BytesBGRA color) = 0;

  virtual std::tuple<Matrix<int>, size_t> GetFilterKernel(std::string,
                                                          size_t) = 0;

  virtual bool MakeKernelFilter(ConstPixBytes prevPixels, PixBytes lastPixels,
                                size_t width, size_t height, Matrix<int> kernel,
                                size_t normalizer,
                                std::atomic<bool> &kernelStop) = 0;

  virtual bool MakeSimpleColorCorrection(ConstPixBytes prevPixels,
                                         PixBytes lastPixels,
                                         size_t sizeInBytes, int brightness,
                                         int contrast) = 0;

  virtual bool MakeHSVColorCorrection(ConstPixBytes prevPixels,
                                      PixBytes lastPixels, size_t sizeInBytes,
                                      int h, int s, int v) = 0;

  virtual bool MakeHSLColorCorrection(ConstPixBytes prevPixels,
                                      PixBytes lastPixels, size_t sizeInBytes,
                                      int h, int s, int l) = 0;
};

inline IImageProcessor::~IImageProcessor() {}

class Controller : public IController {
public:
  explicit Controller(IImageProcessor &imgProc) : imgProc_{imgProc} {}
  ~Controller() {}

  Controller(const Controller &) = delete;
  Controller &operator=(const Controller &) = delete;

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
  IImageProcessor &imgProc_;
};

#endif // CONTROLLER_H
