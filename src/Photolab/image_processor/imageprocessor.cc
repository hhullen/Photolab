#include "imageprocessor.h"

ImageProcessor::ImageProcessor()
    : simpleFilter_{runner_}, kernelFilter_{runner_},
      colorCorrection_{runner_} {}

ImageProcessor::~ImageProcessor() {}

bool ImageProcessor::MakeSimpleFilter(ConstPixBytes prevPixels,
                                      PixBytes lastPixels, size_t sizeInBytes,
                                      std::string filter, std::string param,
                                      const BytesBGRA color) {
  bool res = false;
  if (filter == SimpleFilter::kNegative) {
    res = simpleFilter_.MakeNegative(std::move(prevPixels),
                                     std::move(lastPixels), sizeInBytes);
  } else if (filter == SimpleFilter::kBlackAndWhite) {
    res = simpleFilter_.MakeBlackAndWhite(
        std::move(prevPixels), std::move(lastPixels), sizeInBytes, param);

  } else if (filter == SimpleFilter::kToning) {
    res = simpleFilter_.MakeToning(std::move(prevPixels), std::move(lastPixels),
                                   sizeInBytes, color);
  } else {
    return false;
  }

  return res;
}

std::tuple<Matrix<int>, size_t>
ImageProcessor::GetFilterKernel(std::string kernelName, size_t size) {
  if (kernelName == KernelFilter::kGauss) {
    return kernelFilter_.GetGaussKernel(size);
  } else if (kernelName == KernelFilter::kBoxBlur) {
    return kernelFilter_.GetBoxBlurKernel(size);
  } else if (kernelName == KernelFilter::kEmboss) {
    return kernelFilter_.GeEmbossKernel(size);
  } else if (kernelName == KernelFilter::kLaplacian) {
    return kernelFilter_.GetLaplacianKernel(size);
  } else if (kernelName == KernelFilter::kSharpen) {
    return kernelFilter_.GetSharpenKernel(size);
  } else if (kernelName == KernelFilter::kSobelLeft) {
    return kernelFilter_.GetSobelLeftKernel(size);
  } else if (kernelName == KernelFilter::kSobelRight) {
    return kernelFilter_.GetSobelRightKernel(size);
  }

  return {Matrix<int>(0, 0), 0};
}

bool ImageProcessor::MakeKernelFilter(ConstPixBytes prevPixels,
                                      PixBytes lastPixels, size_t width,
                                      size_t height, Matrix<int> kernel,
                                      size_t normalizer,
                                      std::atomic<bool> &kernelStop) {
  return kernelFilter_.MakeKernelFilter(
      std::move(prevPixels), std::move(lastPixels), width, height,
      std::move(kernel), normalizer, kernelStop);
}

bool ImageProcessor::MakeSimpleColorCorrection(ConstPixBytes prevPixels,
                                               PixBytes lastPixels,
                                               size_t sizeInBytes,
                                               int brightness, int contrast) {
  return colorCorrection_.MakeSimpleColorCorrection(
      std::move(prevPixels), std::move(lastPixels), sizeInBytes, brightness,
      contrast);
}

bool ImageProcessor::MakeHSVColorCorrection(ConstPixBytes prevPixels,
                                            PixBytes lastPixels,
                                            size_t sizeInBytes, int h, int s,
                                            int v) {
  return colorCorrection_.MakeHSVColorCorrection(
      std::move(prevPixels), std::move(lastPixels), sizeInBytes, h, s, v);
}

bool ImageProcessor::MakeHSLColorCorrection(ConstPixBytes prevPixels,
                                            PixBytes lastPixels,
                                            size_t sizeInBytes, int h, int s,
                                            int l) {
  return colorCorrection_.MakeHSLColorCorrection(
      std::move(prevPixels), std::move(lastPixels), sizeInBytes, h, s, l);
}
