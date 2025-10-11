#ifndef CONTROLLER_INTERFACE_H
#define CONTROLLER_INTERFACE_H

#include <atomic>
#include <string>

#include "libs/matrix/matrix.h"

using BytesBGRA = unsigned char[4];
using PixBytes = Matrix<unsigned char, MatrixType::NoCopy>;
using ConstPixBytes = Matrix<const unsigned char, MatrixType::NoCopy>;

class IController {
 public:
  IController() = default;
  virtual ~IController() = 0;
  IController(const IController &) = delete;
  IController &operator=(const IController &) = delete;

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

inline IController::~IController() {}

#endif  // CONTROLLER_INTERFACE_H
