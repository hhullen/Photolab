#include <cstddef>
#include <gtest/gtest.h>

#include "matrix.h"

TEST(Matrix, constructDefault) {
  auto f = [] { Matrix<int, MatrixType::NoCopy> m; };
  EXPECT_NO_THROW(f());
  auto f2 = [] { Matrix<int, MatrixType::NoCopy> m; };
  EXPECT_NO_THROW(f2());

  Matrix<int, MatrixType::Copyable> m;
  EXPECT_EQ(m.cols(), size_t(0));
  EXPECT_EQ(m.rows(), size_t(0));
}

TEST(Matrix, construcParametrized) {
  size_t rows = 50;
  size_t cols = 600;
  int value = 555;

  auto f = [rows, cols, value] {
    Matrix<int, MatrixType::NoCopy> m(rows, cols, value);
  };
  EXPECT_NO_THROW(f());
  auto f2 = [rows, cols, value] {
    Matrix<int, MatrixType::NoCopy> m(rows, cols, value);
  };
  EXPECT_NO_THROW(f2());

  Matrix<int, MatrixType::Copyable> m(rows, cols, value);
  EXPECT_EQ(m.cols(), cols);
  EXPECT_EQ(m.rows(), rows);

  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      EXPECT_EQ(m.get(i, j), value);
    }
  }
}

TEST(Matrix, construcFromData) {
  size_t rows = 50;
  size_t cols = 600;

  int *data = new int[rows * cols];
  for (size_t i = 0; i < rows * cols; i++) {
    data[i] = i;
  }

  auto f = [rows, cols, &data] {
    Matrix<int, MatrixType::NoCopy> m(data, rows, cols);
  };
  EXPECT_NO_THROW(f());
  auto f2 = [rows, cols, &data] {
    Matrix<int, MatrixType::NoCopy> m(data, rows, cols);
  };
  EXPECT_NO_THROW(f2());

  Matrix<int, MatrixType::NoCopy> m(data, rows, cols);
  EXPECT_EQ(m.cols(), cols);
  EXPECT_EQ(m.rows(), rows);

  for (size_t i = 0; i < rows * cols; i++) {
    EXPECT_EQ(m.getData()[i], int(i));
  }
}

TEST(Matrix, assignOperator) {
  size_t rows = 50;
  size_t cols = 600;
  int value = 555;

  Matrix<int, MatrixType::Copyable> m1(rows, cols, value);
  Matrix<int, MatrixType::Copyable> m2;
  m2 = m1;

  EXPECT_EQ(m2.cols(), cols);
  EXPECT_EQ(m2.rows(), rows);

  for (size_t i = 0; i < rows * cols; i++) {
    EXPECT_EQ(m2.getData()[i], value);
  }
}

TEST(Matrix, copyOperator) {
  size_t rows = 50;
  size_t cols = 600;
  int value = 555;

  Matrix<int, MatrixType::Copyable> m1(rows, cols, value);
  Matrix<int, MatrixType::Copyable> m2(m1);

  EXPECT_EQ(m2.cols(), m1.cols());
  EXPECT_EQ(m2.rows(), m1.rows());

  for (size_t i = 0; i < rows * cols; i++) {
    EXPECT_EQ(m2.getData()[i], value);
  }
}

TEST(Matrix, moveOperator) {
  size_t rows = 50;
  size_t cols = 600;
  int value = 555;

  Matrix<int, MatrixType::NoCopy> m1(rows, cols, value);
  Matrix<int, MatrixType::NoCopy> m2(std::move(m1));

  EXPECT_EQ(m2.cols(), cols);
  EXPECT_EQ(m2.rows(), rows);

  for (size_t i = 0; i < rows * cols; i++) {
    EXPECT_EQ(m2.getData()[i], value);
  }
}
