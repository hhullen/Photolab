#ifndef MATRIX_H
#define MATRIX_H

#include <algorithm>
#include <cstddef>

enum class MatrixType { Copyable, NoCopy };

template <typename T, MatrixType Type = MatrixType::Copyable> class Matrix {
public:
  Matrix() : data_{nullptr}, rows_{nullptr}, row_(0), col_(0) {}

  Matrix(size_t row, size_t col, T val = 0) { MakeNewMatrix(row, col, val); }

  Matrix(T *data, size_t row, size_t col)
    requires(Type == MatrixType::NoCopy)
  {
    row_ = row;
    col_ = col;
    data_ = data;
    rows_ = new T *[row];

    for (size_t i = 0; i < row; i++) {
      rows_[i] = &data_[i * col];
    }
  }

  Matrix(const Matrix &other)
    requires(Type == MatrixType::NoCopy)
  = delete;

  Matrix &operator=(const Matrix &other)
    requires(Type == MatrixType::NoCopy)
  = delete;

  Matrix &operator=(Matrix &other)
    requires(Type == MatrixType::NoCopy)
  {
    if (this != &other) {
      DeleteThisMatrix();
      MoveToThis(other);
    }
    return *this;
  }

  Matrix(const Matrix &other) {
    if (this != &other) {
      CopyMatrix(other);
    }
  }

  Matrix(Matrix &&other) {
    if (this != &other) {
      DeleteThisMatrix();
      MoveToThis(other);
    }
  }

  Matrix &operator=(const Matrix &other) {
    if (this != &other) {
      DeleteThisMatrix();
      CopyMatrix(other);
    }
    return *this;
  }

  Matrix &operator=(Matrix &other) {
    if (this != &other) {
      DeleteThisMatrix();
      CopyMatrix(other);
    }
    return *this;
  };

  T *operator[](size_t row) { return rows_[row]; }

  const T *operator[](size_t row) const { return rows_[row]; }

  ~Matrix() { DeleteThisMatrix(); }

  ~Matrix()
    requires(Type == MatrixType::NoCopy)
  {
    if (rows_ != nullptr) {
      delete[] rows_;
      rows_ = nullptr;
    }
  }

  T &get(size_t row, size_t col) { return rows_[row][col]; }

  const T &get(size_t row, size_t col) const { return rows_[row][col]; }

  void set(T v, size_t row, size_t col) { rows_[row][col] = v; }

  const T *getData() const { return data_; }

  T *getData() { return data_; }

  size_t cols() const { return col_; }

  size_t rows() const { return row_; }

private:
  T *data_{nullptr};
  T **rows_{nullptr};
  size_t row_, col_;

  void MakeNewMatrix(size_t row, size_t col, T val) {
    row_ = row;
    col_ = col;

    if constexpr (std::is_const_v<T>) {
      using NonConstT = std::remove_const_t<T>;
      size_t size = col_ * row_;
      NonConstT *data = new NonConstT[size]{};
      for (size_t i = 0; i < size; ++i) {
        data[i] = val;
      }
      data_ = data;
    } else {
      size_t size = col_ * row_;
      data_ = new T[size];
      for (size_t i = 0; i < size; ++i) {
        data_[i] = val;
      }
    }

    rows_ = new T *[row_] { nullptr };

    for (size_t i = 0; i < row_; i++) {
      rows_[i] = &data_[i * col];
    }
  }

  void CopyMatrix(const Matrix &other) {
    row_ = other.row_;
    col_ = other.col_;

    data_ = CopyData(other);

    rows_ = new T *[row_] { nullptr };

    for (size_t i = 0; i < row_; i++) {
      rows_[i] = &data_[i * col_];
    }
  }

  T *CopyData(const Matrix &other) {
    if constexpr (std::is_const_v<T>) {
      using NonConstT = std::remove_const_t<T>;
      size_t size = other.col_ * other.row_;
      NonConstT *data = new NonConstT[size]{};

      for (size_t i = 0; i < size; ++i) {
        data[i] = other.data_[i];
      }

      return data;
    } else {
      size_t size = other.col_ * other.row_;
      T *data = new T[size]{};
      for (size_t i = 0; i < size; ++i) {
        data[i] = other.data_[i];
      }
      return data;
    }
  }

  void DeleteThisMatrix() {
    if (rows_ != nullptr) {
      delete[] rows_;
      rows_ = nullptr;
    }
    if (data_ != nullptr) {
      data_ = nullptr;
      delete[] data_;
    }
  }

  inline void MoveToThis(Matrix &other) {
    data_ = other.data_;
    rows_ = other.rows_;
    row_ = other.row_;
    col_ = other.col_;
    other.data_ = nullptr;
    other.rows_ = nullptr;
    other.row_ = 0;
    other.col_ = 0;
  }
};

#endif // MATRIX_H
