#include "controller.h"

bool Controller::MakeSimpleFilter(ConstPixBytes prevPixels, PixBytes lastPixels,
                                  size_t sizeInBytes, std::string filter,
                                  std::string param, const BytesBGRA color) {
  return imgProc_.MakeSimpleFilter(std::move(prevPixels), std::move(lastPixels),
                                   sizeInBytes, filter, param, color);
}

std::tuple<Matrix<int>, size_t> Controller::GetFilterKernel(
    std::string kernelName, size_t size) {
  return imgProc_.GetFilterKernel(kernelName, size);
}

bool Controller::MakeKernelFilter(ConstPixBytes prevPixels, PixBytes lastPixels,
                                  size_t width, size_t height,
                                  Matrix<int> kernel, size_t normalizer,
                                  std::atomic<bool> &kernelStop) {
  return imgProc_.MakeKernelFilter(std::move(prevPixels), std::move(lastPixels),
                                   width, height, std::move(kernel), normalizer,
                                   kernelStop);
}

bool Controller::MakeSimpleColorCorrection(ConstPixBytes prevPixels,
                                           PixBytes lastPixels,
                                           size_t sizeInBytes, int brightness,
                                           int contrast) {
  return imgProc_.MakeSimpleColorCorrection(std::move(prevPixels),
                                            std::move(lastPixels), sizeInBytes,
                                            brightness, contrast);
}

bool Controller::MakeHSVColorCorrection(ConstPixBytes prevPixels,
                                        PixBytes lastPixels, size_t sizeInBytes,
                                        int h, int s, int v) {
  return imgProc_.MakeHSVColorCorrection(
      std::move(prevPixels), std::move(lastPixels), sizeInBytes, h, s, v);
}

bool Controller::MakeHSLColorCorrection(ConstPixBytes prevPixels,
                                        PixBytes lastPixels, size_t sizeInBytes,
                                        int h, int s, int l) {
  return imgProc_.MakeHSLColorCorrection(
      std::move(prevPixels), std::move(lastPixels), sizeInBytes, h, s, l);
}
