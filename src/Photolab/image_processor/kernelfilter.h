#ifndef KERNELFILTER_H
#define KERNELFILTER_H

#include <future>

#include "channel.h"
#include "libs/matrix/matrix.h"
#include "runner_interface.h"

using Task = std::packaged_task<void()>;
using TaskChan = Channel<Task>;
using uchar = unsigned char;
using PixBytes = Matrix<uchar, MatrixType::NoCopy>;
using ConstPixBytes = Matrix<const uchar, MatrixType::NoCopy>;

class KernelFilter {
public:
  explicit KernelFilter(IRunner &runner) : runner_{runner} {}

  bool MakeKernelFilter(ConstPixBytes prevPixels, PixBytes lastPixels,
                        size_t width, size_t height, Matrix<int> kernel,
                        size_t normalizer, std::atomic<bool> &kernelStop);

  std::tuple<Matrix<int>, size_t> GetGaussKernel(size_t);
  std::tuple<Matrix<int>, size_t> GeEmbossKernel(size_t);
  std::tuple<Matrix<int>, size_t> GetSharpenKernel(size_t);
  std::tuple<Matrix<int>, size_t> GetBoxBlurKernel(size_t);
  std::tuple<Matrix<int>, size_t> GetLaplacianKernel(size_t);
  std::tuple<Matrix<int>, size_t> GetSobelRightKernel(size_t);
  std::tuple<Matrix<int>, size_t> GetSobelLeftKernel(size_t);

  static constexpr const char *kGauss{"Gauss Blur"};
  static constexpr const char *kEmboss{"Emboss"};
  static constexpr const char *kSharpen{"Sharpen"};
  static constexpr const char *kBoxBlur{"Box Blur"};
  static constexpr const char *kLaplacian{"Laplacian"};
  static constexpr const char *kSobelRight{"Sobel Right"};
  static constexpr const char *kSobelLeft{"Sobel Left"};
  static constexpr const char *kCustom{"Custom"};

private:
  IRunner &runner_;

  static constexpr size_t kBlueIdx{0};
  static constexpr size_t kGreenIdx{1};
  static constexpr size_t kRedIdx{2};
  static constexpr size_t kAlphaIdx{3};

  static constexpr const char rawEmboss[3][3] = {
      {-2, -1, 0},
      {-1, 1, 1},
      {0, 1, 2},
  };
  static constexpr const char rawSharpen[3][3] = {
      {0, -1, 0},
      {-1, 5, -1},
      {0, -1, 0},
  };
  static constexpr const char rawBoxBlur[3][3] = {
      {1, 1, 1},
      {1, 1, 1},
      {1, 1, 1},
  };
  static constexpr const char rawLaplacian[3][3] = {
      {-1, -1, -1},
      {-1, 8, -1},
      {-1, -1, -1},
  };
  static constexpr const char rawSobelRight[3][3] = {
      {-1, 0, 1},
      {-2, 0, 2},
      {-1, 0, 1},
  };
  static constexpr const char rawSobelLeft[3][3] = {
      {1, 0, -1},
      {2, 0, -2},
      {1, 0, -1},
  };

  Matrix<int> NewMatrixFromRaw(const char *, size_t row, size_t col);
  std::tuple<Matrix<int>, size_t> UpscaleKernel(Matrix<int> &oldKernel,
                                                size_t newSize);
};

#endif // KERNELFILTER_H
