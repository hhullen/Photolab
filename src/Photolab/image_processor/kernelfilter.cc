#include "kernelfilter.h"

#include <cmath>

#include "image_processor/support.h"

bool KernelFilter::MakeKernelFilter(ConstPixBytes prevPixels,
                                    PixBytes lastPixels, size_t width,
                                    size_t height, Matrix<int> kernel,
                                    size_t normalizer,
                                    std::atomic<bool> &kernelStop) {
  if (kernel.rows() != kernel.cols() || normalizer < 1) {
    return false;
  }

  static const int bytesPerPix = 4;
  const size_t bytesWidth = width * bytesPerPix;
  const int prevImgColsShift = kernel.cols() / 2 * bytesPerPix;
  const int prevImgRowsShift = kernel.cols() / 2;

  auto processPixel = [&prevPixels, &lastPixels, &kernel, height, normalizer,
                       bytesWidth, prevImgRowsShift, prevImgColsShift,
                       &kernelStop](size_t i) {
    if (kernelStop.load()) {
      return;
    }

    Matrix<const uchar, MatrixType::NoCopy> prevImg(prevPixels.getData(),
                                                    height, bytesWidth);

    long long int sumBlue = 0;
    long long int sumGreen = 0;
    long long int sumRed = 0;
    int imgRow = i / bytesWidth - prevImgRowsShift;

    for (size_t row = 0; row < kernel.rows(); row++, imgRow++) {
      int imgCol = i % bytesWidth - prevImgColsShift;
      for (size_t col = 0; col < kernel.cols(); col++, imgCol += bytesPerPix) {
        long long int k = kernel[row][col];
        if (imgRow < 0 || imgRow > static_cast<int>(height - 1)) {
          sumBlue += k * 255;
          sumGreen += k * 255;
          sumRed += k * 255;
          continue;
        }

        if (imgCol < 0 || imgCol > static_cast<int>(bytesWidth - 1)) {
          sumBlue += k * 255;
          sumGreen += k * 255;
          sumRed += k * 255;
          continue;
        }

        sumBlue += k * prevImg[imgRow][imgCol + kBlueIdx];
        sumGreen += k * prevImg[imgRow][imgCol + kGreenIdx];
        sumRed += k * prevImg[imgRow][imgCol + kRedIdx];
      }
    }

    auto [newBlue, newGreen, newRed] =
        Support::GetPixChannsBGR(&lastPixels.getData()[i]);
    newBlue = std::clamp(static_cast<int>(sumBlue / normalizer), 0, 255);
    newGreen = std::clamp(static_cast<int>(sumGreen / normalizer), 0, 255);
    newRed = std::clamp(static_cast<int>(sumRed / normalizer), 0, 255);
  };

  bool res = runner_.RunPerPixInWorkers(bytesWidth * height, processPixel);

  if (kernelStop.load()) {
    return false;
  }

  return res;
}

std::tuple<Matrix<int>, size_t> KernelFilter::GetGaussKernel(size_t size) {
  std::vector<int> row(size);
  row[0] = 1;
  for (size_t i = 1; i < size; i++) {
    row[i] = row[i - 1] * (size - i) / i;
  }

  size_t normalizer = 0;
  Matrix<int> kernel(size, size, 0);
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size; j++) {
      kernel[i][j] = row[i] * row[j];
      normalizer += kernel[i][j];
    }
  }

  return {std::move(kernel), normalizer};
}

std::tuple<Matrix<int>, size_t> KernelFilter::GeEmbossKernel(size_t size) {
  size_t normalizer = 1;
  Matrix<int> defaultMatrix = NewMatrixFromRaw(rawEmboss[0], 3, 3);
  if (size <= 3) {
    return {std::move(defaultMatrix), normalizer};
  }

  return UpscaleKernel(defaultMatrix, size);
}

std::tuple<Matrix<int>, size_t> KernelFilter::GetSharpenKernel(size_t size) {
  size_t normalizer = 1;
  Matrix<int> defaultMatrix = NewMatrixFromRaw(rawSharpen[0], 3, 3);
  if (size <= 3) {
    return {std::move(defaultMatrix), normalizer};
  }

  return UpscaleKernel(defaultMatrix, size);
}

std::tuple<Matrix<int>, size_t> KernelFilter::GetBoxBlurKernel(size_t size) {
  size_t normalizer = size * size;
  return {Matrix<int>(size, size, 1), normalizer};
}

std::tuple<Matrix<int>, size_t> KernelFilter::GetLaplacianKernel(size_t size) {
  size_t normalizer = 1;
  Matrix<int> defaultMatrix = NewMatrixFromRaw(rawLaplacian[0], 3, 3);
  if (size <= 3) {
    return {std::move(defaultMatrix), normalizer};
  }

  return UpscaleKernel(defaultMatrix, size);
}

std::tuple<Matrix<int>, size_t> KernelFilter::GetSobelRightKernel(size_t size) {
  size_t normalizer = 1;
  Matrix<int> defaultMatrix = NewMatrixFromRaw(rawSobelRight[0], 3, 3);
  if (size <= 3) {
    return {std::move(defaultMatrix), normalizer};
  }

  return UpscaleKernel(defaultMatrix, size);
}

std::tuple<Matrix<int>, size_t> KernelFilter::GetSobelLeftKernel(size_t size) {
  size_t normalizer = 1;
  Matrix<int> defaultMatrix = NewMatrixFromRaw(rawSobelLeft[0], 3, 3);
  if (size <= 3) {
    return {std::move(defaultMatrix), normalizer};
  }

  return UpscaleKernel(defaultMatrix, size);
}

Matrix<int> KernelFilter::NewMatrixFromRaw(const char *data, size_t row,
                                           size_t col) {
  Matrix<int> kernel(row, col);
  for (size_t i = 0; i < row; i++) {
    for (size_t j = 0; j < col; j++) {
      kernel.set(data[i * col + j], i, j);
    }
  }

  return kernel;
}

std::tuple<Matrix<int>, size_t>
KernelFilter::UpscaleKernel(Matrix<int> &oldKernel, size_t newSize) {
  size_t oldSize = oldKernel.rows();
  Matrix<int> newKernel(newSize, newSize, 0);
  size_t normalizer = 0;

  for (size_t i = 0; i < newSize; i++) {
    for (size_t j = 0; j < newSize; j++) {
      float old_i = i * float(oldSize - 1) / float(newSize - 1);
      float old_j = j * float(oldSize - 1) / float(newSize - 1);

      size_t i0 = size_t(floor(old_i));
      size_t j0 = size_t(floor(old_j));
      size_t i1 = std::min(i0 + 1, oldSize - 1);
      size_t j1 = std::min(j0 + 1, oldSize - 1);

      float wi = old_i - i0;
      float wj = old_j - j0;

      float val = oldKernel[i0][j0] * (1 - wi) * (1 - wj) +
                  oldKernel[i1][j0] * wi * (1 - wj) +
                  oldKernel[i0][j1] * (1 - wi) * wj +
                  oldKernel[i1][j1] * wi * wj;

      newKernel[i][j] = char(std::round(val));
      normalizer += newKernel[i][j];
    }
  }
  if (normalizer < 1) {
    normalizer = 1;
  }

  return {std::move(newKernel), normalizer};
}
