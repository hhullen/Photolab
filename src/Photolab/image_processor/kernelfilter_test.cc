#include <gtest/gtest.h>

#include "kernelfilter.cc"
#include "runner_mock.h"

TEST(KernelFilter, construct) {
  MockRunner r = MockRunner();
  EXPECT_NO_THROW({ KernelFilter kf(r); });
}

TEST(KernelFilter, MakeKernelFilterGauss) {
  MockRunner r = MockRunner();
  KernelFilter kf(r);

  size_t w = 20;
  size_t h = 10;
  size_t sizeInBytes = w * h * 4;
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(1, sizeInBytes);
  Matrix<uchar, MatrixType::NoCopy> lastPixels(1, sizeInBytes);

  std::atomic<bool> kernelStop;
  auto [kernel, norm] = kf.GetGaussKernel(3);

  EXPECT_CALL(r, RunPerPixInWorkers(testing::_, testing::_))
      .WillOnce(MockRunner::MockRunPerPixInWorkers);

  EXPECT_NO_THROW({
    EXPECT_TRUE(kf.MakeKernelFilter(std::move(prevPixels),
                                    std::move(lastPixels), w, h, kernel, norm,
                                    kernelStop));
  });
}

TEST(KernelFilter, MakeKernelFilterEmboss) {
  MockRunner r = MockRunner();
  KernelFilter kf(r);

  size_t w = 20;
  size_t h = 10;
  size_t sizeInBytes = w * h * 4;
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(1, sizeInBytes);
  Matrix<uchar, MatrixType::NoCopy> lastPixels(1, sizeInBytes);

  std::atomic<bool> kernelStop;
  auto [kernel, norm] = kf.GeEmbossKernel(3);

  EXPECT_CALL(r, RunPerPixInWorkers(testing::_, testing::_))
      .WillOnce(MockRunner::MockRunPerPixInWorkers);

  EXPECT_NO_THROW({
    EXPECT_TRUE(kf.MakeKernelFilter(std::move(prevPixels),
                                    std::move(lastPixels), w, h, kernel, norm,
                                    kernelStop));
  });
}

TEST(KernelFilter, MakeKernelFilterSharpen) {
  MockRunner r = MockRunner();
  KernelFilter kf(r);

  size_t w = 20;
  size_t h = 10;
  size_t sizeInBytes = w * h * 4;
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(1, sizeInBytes);
  Matrix<uchar, MatrixType::NoCopy> lastPixels(1, sizeInBytes);

  std::atomic<bool> kernelStop;
  auto [kernel, norm] = kf.GetSharpenKernel(3);

  EXPECT_CALL(r, RunPerPixInWorkers(testing::_, testing::_))
      .WillOnce(MockRunner::MockRunPerPixInWorkers);

  EXPECT_NO_THROW({
    EXPECT_TRUE(kf.MakeKernelFilter(std::move(prevPixels),
                                    std::move(lastPixels), w, h, kernel, norm,
                                    kernelStop));
  });
}

TEST(KernelFilter, MakeKernelFilterBoxBlur) {
  MockRunner r = MockRunner();
  KernelFilter kf(r);

  size_t w = 20;
  size_t h = 10;
  size_t sizeInBytes = w * h * 4;
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(1, sizeInBytes);
  Matrix<uchar, MatrixType::NoCopy> lastPixels(1, sizeInBytes);

  std::atomic<bool> kernelStop;
  auto [kernel, norm] = kf.GetBoxBlurKernel(3);

  EXPECT_CALL(r, RunPerPixInWorkers(testing::_, testing::_))
      .WillOnce(MockRunner::MockRunPerPixInWorkers);

  EXPECT_NO_THROW({
    EXPECT_TRUE(kf.MakeKernelFilter(std::move(prevPixels),
                                    std::move(lastPixels), w, h, kernel, norm,
                                    kernelStop));
  });
}

TEST(KernelFilter, MakeKernelFilterBoxBlur17x17BilenialInterpolation) {
  MockRunner r = MockRunner();
  KernelFilter kf(r);

  size_t w = 20;
  size_t h = 10;
  size_t sizeInBytes = w * h * 4;
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(1, sizeInBytes);
  Matrix<uchar, MatrixType::NoCopy> lastPixels(1, sizeInBytes);

  std::atomic<bool> kernelStop;
  auto [kernel, norm] = kf.GetBoxBlurKernel(17);

  EXPECT_CALL(r, RunPerPixInWorkers(testing::_, testing::_))
      .WillOnce(MockRunner::MockRunPerPixInWorkers);

  EXPECT_NO_THROW({
    EXPECT_TRUE(kf.MakeKernelFilter(std::move(prevPixels),
                                    std::move(lastPixels), w, h, kernel, norm,
                                    kernelStop));
  });
}

TEST(KernelFilter, MakeKernelLaplacian) {
  MockRunner r = MockRunner();
  KernelFilter kf(r);

  size_t w = 20;
  size_t h = 10;
  size_t sizeInBytes = w * h * 4;
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(1, sizeInBytes);
  Matrix<uchar, MatrixType::NoCopy> lastPixels(1, sizeInBytes);

  std::atomic<bool> kernelStop;
  auto [kernel, norm] = kf.GetLaplacianKernel(3);

  EXPECT_CALL(r, RunPerPixInWorkers(testing::_, testing::_))
      .WillOnce(MockRunner::MockRunPerPixInWorkers);

  EXPECT_NO_THROW({
    EXPECT_TRUE(kf.MakeKernelFilter(std::move(prevPixels),
                                    std::move(lastPixels), w, h, kernel, norm,
                                    kernelStop));
  });
}

TEST(KernelFilter, MakeKernelFilterSobelRight) {
  MockRunner r = MockRunner();
  KernelFilter kf(r);

  size_t w = 20;
  size_t h = 10;
  size_t sizeInBytes = w * h * 4;
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(1, sizeInBytes);
  Matrix<uchar, MatrixType::NoCopy> lastPixels(1, sizeInBytes);

  std::atomic<bool> kernelStop;
  auto [kernel, norm] = kf.GetSobelRightKernel(3);

  EXPECT_CALL(r, RunPerPixInWorkers(testing::_, testing::_))
      .WillOnce(MockRunner::MockRunPerPixInWorkers);

  EXPECT_NO_THROW({
    EXPECT_TRUE(kf.MakeKernelFilter(std::move(prevPixels),
                                    std::move(lastPixels), w, h, kernel, norm,
                                    kernelStop));
  });
}

TEST(KernelFilter, MakeKernelFilterSobelLeft) {
  MockRunner r = MockRunner();
  KernelFilter kf(r);

  size_t w = 20;
  size_t h = 10;
  size_t sizeInBytes = w * h * 4;
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(1, sizeInBytes);
  Matrix<uchar, MatrixType::NoCopy> lastPixels(1, sizeInBytes);

  std::atomic<bool> kernelStop;
  auto [kernel, norm] = kf.GetSobelLeftKernel(3);

  EXPECT_CALL(r, RunPerPixInWorkers(testing::_, testing::_))
      .WillOnce(MockRunner::MockRunPerPixInWorkers);

  EXPECT_NO_THROW({
    EXPECT_TRUE(kf.MakeKernelFilter(std::move(prevPixels),
                                    std::move(lastPixels), w, h, kernel, norm,
                                    kernelStop));
  });
}

TEST(KernelFilter, MakeKernelFilterCustomKernel) {
  MockRunner r = MockRunner();
  KernelFilter kf(r);

  size_t w = 20;
  size_t h = 10;
  size_t sizeInBytes = w * h * 4;
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(1, sizeInBytes);
  Matrix<uchar, MatrixType::NoCopy> lastPixels(1, sizeInBytes);

  std::atomic<bool> kernelStop;
  Matrix<int> kernel(3, 3);
  kernel[0][0] = 4;
  kernel[1][1] = 1;
  kernel[2][2] = 4;
  size_t norm = 9;

  EXPECT_CALL(r, RunPerPixInWorkers(testing::_, testing::_))
      .WillOnce(MockRunner::MockRunPerPixInWorkers);

  EXPECT_NO_THROW({
    EXPECT_TRUE(kf.MakeKernelFilter(std::move(prevPixels),
                                    std::move(lastPixels), w, h, kernel, norm,
                                    kernelStop));
  });
}
