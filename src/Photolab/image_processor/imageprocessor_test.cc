#include <gtest/gtest.h>

#include "imageprocessor.cc"

TEST(ImageProcessor, construct) {
  EXPECT_NO_THROW({ ImageProcessor ip; });
}

TEST(ImageProcessor, MakeSimpleFilterNegative) {
  ImageProcessor ip;

  size_t w = 20;
  size_t h = 10;
  size_t sizeInBytes = w * h * 4;
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(1, sizeInBytes);
  Matrix<uchar, MatrixType::NoCopy> lastPixels(1, sizeInBytes);
  const unsigned char color[4]{0, 0, 0, 255};

  EXPECT_NO_THROW({
    EXPECT_TRUE(ip.MakeSimpleFilter(std::move(prevPixels),
                                    std::move(lastPixels), sizeInBytes,
                                    SimpleFilter::kNegative, "Blue", color));
  });
}

TEST(ImageProcessor, MakeSimpleFilterBlackAndWhite) {
  ImageProcessor ip;

  size_t w = 20;
  size_t h = 10;
  size_t sizeInBytes = w * h * 4;
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(1, sizeInBytes);
  Matrix<uchar, MatrixType::NoCopy> lastPixels(1, sizeInBytes);
  const unsigned char color[4]{0, 0, 0, 255};

  EXPECT_NO_THROW({
    EXPECT_TRUE(ip.MakeSimpleFilter(
        std::move(prevPixels), std::move(lastPixels), sizeInBytes,
        SimpleFilter::kBlackAndWhite, "Blue", color));
  });
}

TEST(ImageProcessor, MakeSimpleFilterToning) {
  ImageProcessor ip;

  size_t w = 20;
  size_t h = 10;
  size_t sizeInBytes = w * h * 4;
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(1, sizeInBytes);
  Matrix<uchar, MatrixType::NoCopy> lastPixels(1, sizeInBytes);
  const unsigned char color[4]{0, 0, 0, 255};

  EXPECT_NO_THROW({
    EXPECT_TRUE(ip.MakeSimpleFilter(std::move(prevPixels),
                                    std::move(lastPixels), sizeInBytes,
                                    SimpleFilter::kToning, "Blue", color));
  });
}

TEST(ImageProcessor, GetFilterKernelGauss) {
  ImageProcessor ip;

  size_t kernelSize = 3;
  std::tuple<Matrix<int>, size_t> t;
  EXPECT_NO_THROW(
      { t = ip.GetFilterKernel(KernelFilter::kGauss, kernelSize); });

  auto [kernel, norm] = t;
  EXPECT_TRUE(norm > 0);
  EXPECT_EQ(kernel.cols(), kernelSize);
  EXPECT_EQ(kernel.rows(), kernelSize);
}

TEST(ImageProcessor, GetFilterKernelBoxBlur) {
  ImageProcessor ip;

  size_t kernelSize = 3;

  std::tuple<Matrix<int>, size_t> t;
  EXPECT_NO_THROW(
      { t = ip.GetFilterKernel(KernelFilter::kBoxBlur, kernelSize); });

  auto [kernel, norm] = t;
  EXPECT_TRUE(norm > 0);
  EXPECT_EQ(kernel.cols(), kernelSize);
  EXPECT_EQ(kernel.rows(), kernelSize);
}

TEST(ImageProcessor, GetFilterKernelEmboss) {
  ImageProcessor ip;

  size_t kernelSize = 3;

  std::tuple<Matrix<int>, size_t> t;
  EXPECT_NO_THROW(
      { t = ip.GetFilterKernel(KernelFilter::kEmboss, kernelSize); });

  auto [kernel, norm] = t;
  EXPECT_TRUE(norm > 0);
  EXPECT_EQ(kernel.cols(), kernelSize);
  EXPECT_EQ(kernel.rows(), kernelSize);
}

TEST(ImageProcessor, GetFilterKernelLaplacian) {
  ImageProcessor ip;

  size_t kernelSize = 3;

  std::tuple<Matrix<int>, size_t> t;
  EXPECT_NO_THROW(
      { t = ip.GetFilterKernel(KernelFilter::kLaplacian, kernelSize); });

  auto [kernel, norm] = t;
  EXPECT_TRUE(norm > 0);
  EXPECT_EQ(kernel.cols(), kernelSize);
  EXPECT_EQ(kernel.rows(), kernelSize);
}

TEST(ImageProcessor, GetFilterKernelSharpen) {
  ImageProcessor ip;

  size_t kernelSize = 3;

  std::tuple<Matrix<int>, size_t> t;
  EXPECT_NO_THROW(
      { t = ip.GetFilterKernel(KernelFilter::kSharpen, kernelSize); });

  auto [kernel, norm] = t;
  EXPECT_TRUE(norm > 0);
  EXPECT_EQ(kernel.cols(), kernelSize);
  EXPECT_EQ(kernel.rows(), kernelSize);
}

TEST(ImageProcessor, GetFilterKernelSobelLeft) {
  ImageProcessor ip;

  size_t kernelSize = 3;

  std::tuple<Matrix<int>, size_t> t;
  EXPECT_NO_THROW(
      { t = ip.GetFilterKernel(KernelFilter::kSobelLeft, kernelSize); });

  auto [kernel, norm] = t;
  EXPECT_TRUE(norm > 0);
  EXPECT_EQ(kernel.cols(), kernelSize);
  EXPECT_EQ(kernel.rows(), kernelSize);
}

TEST(ImageProcessor, GetFilterKernelSobelRight) {
  ImageProcessor ip;

  size_t kernelSize = 3;

  std::tuple<Matrix<int>, size_t> t;
  EXPECT_NO_THROW(
      { t = ip.GetFilterKernel(KernelFilter::kSobelRight, kernelSize); });

  auto [kernel, norm] = t;
  EXPECT_TRUE(norm > 0);
  EXPECT_EQ(kernel.cols(), kernelSize);
  EXPECT_EQ(kernel.rows(), kernelSize);
}

TEST(ImageProcessor, MakeKernelFilter) {
  ImageProcessor ip;

  size_t w = 20;
  size_t h = 10;
  size_t sizeInBytes = w * h * 4;
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(1, sizeInBytes);
  Matrix<uchar, MatrixType::NoCopy> lastPixels(1, sizeInBytes);

  size_t kernelSize = 3;
  std::atomic<bool> kernelStop;
  auto [kernel, norm] = ip.GetFilterKernel(KernelFilter::kBoxBlur, kernelSize);

  EXPECT_NO_THROW({
    EXPECT_TRUE(ip.MakeKernelFilter(std::move(prevPixels),
                                    std::move(lastPixels), w, h, kernel, norm,
                                    kernelStop));
  });
}

TEST(ImageProcessor, MakeSimpleColorCorrection) {
  ImageProcessor ip;

  size_t w = 20;
  size_t h = 10;
  size_t sizeInBytes = w * h * 4;
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(1, sizeInBytes);
  Matrix<uchar, MatrixType::NoCopy> lastPixels(1, sizeInBytes);

  size_t kernelSize = 3;
  std::atomic<bool> kernelStop;
  auto [kernel, norm] = ip.GetFilterKernel(KernelFilter::kBoxBlur, kernelSize);

  int brightness = 200;
  int contrast = 301;
  EXPECT_NO_THROW({
    EXPECT_TRUE(ip.MakeSimpleColorCorrection(std::move(prevPixels),
                                             std::move(lastPixels), sizeInBytes,
                                             brightness, contrast));
  });
}

TEST(ImageProcessor, MakeHSVColorCorrection) {
  ImageProcessor ip;

  size_t wi = 20;
  size_t hi = 10;
  size_t sizeInBytes = wi * hi * 4;
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(1, sizeInBytes);
  Matrix<uchar, MatrixType::NoCopy> lastPixels(1, sizeInBytes);

  size_t kernelSize = 3;
  std::atomic<bool> kernelStop;
  auto [kernel, norm] = ip.GetFilterKernel(KernelFilter::kBoxBlur, kernelSize);

  int h = 69;
  int s = 700;
  int v = 800;
  EXPECT_NO_THROW({
    EXPECT_TRUE(ip.MakeHSVColorCorrection(
        std::move(prevPixels), std::move(lastPixels), sizeInBytes, h, s, v));
  });
}

TEST(ImageProcessor, MakeHSLColorCorrection) {
  ImageProcessor ip;

  size_t wi = 20;
  size_t hi = 10;
  size_t sizeInBytes = wi * hi * 4;
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(1, sizeInBytes);
  Matrix<uchar, MatrixType::NoCopy> lastPixels(1, sizeInBytes);

  size_t kernelSize = 3;
  std::atomic<bool> kernelStop;
  auto [kernel, norm] = ip.GetFilterKernel(KernelFilter::kBoxBlur, kernelSize);

  int h = 69;
  int s = 700;
  int l = 800;
  EXPECT_NO_THROW({
    EXPECT_TRUE(ip.MakeHSVColorCorrection(
        std::move(prevPixels), std::move(lastPixels), sizeInBytes, h, s, l));
  });
}
