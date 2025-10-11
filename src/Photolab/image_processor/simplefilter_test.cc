#include <gtest/gtest.h>

#include "runner_mock.h"
#include "simplefilter.cc"
#include "simplefilter.h"

#include <iostream>

const int delay = 400;

TEST(SimpleFilter, MakeNegative) {
  MockRunner r = MockRunner();
  SimpleFilter sf(r);

  size_t sizeInBytes = 12;
  uchar value = 100;
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(1, sizeInBytes, value);

  uchar *rawPixels = new uchar[sizeInBytes]{};
  Matrix<uchar, MatrixType::NoCopy> lastPixels(rawPixels, 1, sizeInBytes);

  EXPECT_CALL(r, RunPerPixInWorkers(testing::_, testing::_))
      .WillOnce(MockRunner::MockRunPerPixInWorkers);

  EXPECT_NO_THROW({
    EXPECT_TRUE(sf.MakeNegative(std::move(prevPixels), std::move(lastPixels),
                                sizeInBytes));
  });

  for (size_t i = 0; i < sizeInBytes; i += 4) {
    int newVal = 255 - value;

    EXPECT_EQ(int(rawPixels[i]), newVal);
    EXPECT_EQ(int(rawPixels[i + 1]), newVal);
    EXPECT_EQ(int(rawPixels[i + 2]), newVal);
  }
}

TEST(SimpleFilter, MakeBlackAndWhiteByBlue) {
  MockRunner r;
  SimpleFilter sf(r);

  size_t sizeInBytes = 12;
  uchar valueBlue = 50;
  uchar valueGreen = 100;
  uchar valueRed = 200;
  uchar *data = new uchar[sizeInBytes];
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(data, 1, sizeInBytes);
  for (size_t i = 0; i < sizeInBytes; i += 4) {
    data[i] = valueBlue;
    data[i + 1] = valueGreen;
    data[i + 2] = valueRed;
  }

  uchar *rawPixels = new uchar[sizeInBytes]{};
  Matrix<uchar, MatrixType::NoCopy> lastPixels(rawPixels, 1, sizeInBytes);

  EXPECT_CALL(r, RunPerPixInWorkers(testing::_, testing::_))
      .WillOnce(MockRunner::MockRunPerPixInWorkers);

  EXPECT_NO_THROW({
    EXPECT_TRUE(sf.MakeBlackAndWhite(
        std::move(prevPixels), std::move(lastPixels), sizeInBytes, "Blue"));
  });

  for (size_t i = 0; i < sizeInBytes; i += 4) {
    EXPECT_EQ(int(rawPixels[i + 1]), valueBlue);
    EXPECT_EQ(int(rawPixels[i + 2]), valueBlue);
  }
}

TEST(SimpleFilter, MakeBlackAndWhiteByGreen) {
  MockRunner r;
  SimpleFilter sf(r);

  size_t sizeInBytes = 12;
  uchar valueBlue = 50;
  uchar valueGreen = 100;
  uchar valueRed = 200;
  uchar *data = new uchar[sizeInBytes];
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(data, 1, sizeInBytes);
  for (size_t i = 0; i < sizeInBytes; i += 4) {
    data[i] = valueBlue;
    data[i + 1] = valueGreen;
    data[i + 2] = valueRed;
  }

  uchar *rawPixels = new uchar[sizeInBytes]{};
  Matrix<uchar, MatrixType::NoCopy> lastPixels(rawPixels, 1, sizeInBytes);

  EXPECT_CALL(r, RunPerPixInWorkers(testing::_, testing::_))
      .WillOnce(MockRunner::MockRunPerPixInWorkers);

  EXPECT_NO_THROW({
    EXPECT_TRUE(sf.MakeBlackAndWhite(
        std::move(prevPixels), std::move(lastPixels), sizeInBytes, "Green"));
  });

  for (size_t i = 0; i < sizeInBytes; i += 4) {
    EXPECT_EQ(int(rawPixels[i]), valueGreen);
    EXPECT_EQ(int(rawPixels[i + 2]), valueGreen);
  }
}

TEST(SimpleFilter, MakeBlackAndWhiteByRed) {
  MockRunner r;
  SimpleFilter sf(r);

  size_t sizeInBytes = 12;
  uchar valueBlue = 50;
  uchar valueGreen = 100;
  uchar valueRed = 200;
  uchar *data = new uchar[sizeInBytes];
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(data, 1, sizeInBytes);
  for (size_t i = 0; i < sizeInBytes; i += 4) {
    data[i] = valueBlue;
    data[i + 1] = valueGreen;
    data[i + 2] = valueRed;
  }

  uchar *rawPixels = new uchar[sizeInBytes]{};
  Matrix<uchar, MatrixType::NoCopy> lastPixels(rawPixels, 1, sizeInBytes);

  EXPECT_CALL(r, RunPerPixInWorkers(testing::_, testing::_))
      .WillOnce(MockRunner::MockRunPerPixInWorkers);

  EXPECT_NO_THROW({
    EXPECT_TRUE(sf.MakeBlackAndWhite(
        std::move(prevPixels), std::move(lastPixels), sizeInBytes, "Red"));
  });

  for (size_t i = 0; i < sizeInBytes; i += 4) {
    EXPECT_EQ(int(rawPixels[i]), valueRed);
    EXPECT_EQ(int(rawPixels[i + 1]), valueRed);
  }
}

TEST(SimpleFilter, MakeBlackAndWhiteByAverage) {
  MockRunner r;
  SimpleFilter sf(r);

  size_t sizeInBytes = 12;
  uchar valueBlue = 50;
  uchar valueGreen = 100;
  uchar valueRed = 200;
  uchar *data = new uchar[sizeInBytes];
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(data, 1, sizeInBytes);
  for (size_t i = 0; i < sizeInBytes; i += 4) {
    data[i] = valueBlue;
    data[i + 1] = valueGreen;
    data[i + 2] = valueRed;
  }

  uchar *rawPixels = new uchar[sizeInBytes]{};
  Matrix<uchar, MatrixType::NoCopy> lastPixels(rawPixels, 1, sizeInBytes);

  EXPECT_CALL(r, RunPerPixInWorkers(testing::_, testing::_))
      .WillOnce(MockRunner::MockRunPerPixInWorkers);

  EXPECT_NO_THROW({
    EXPECT_TRUE(sf.MakeBlackAndWhite(
        std::move(prevPixels), std::move(lastPixels), sizeInBytes, "Average"));
  });

  for (size_t i = 0; i < sizeInBytes; i += 4) {
    int avg =
        rawPixels[i] / 3.0 + rawPixels[i + 1] / 3.0 + rawPixels[i + 2] / 3.0;
    EXPECT_EQ(int(rawPixels[i]), avg);
    EXPECT_EQ(int(rawPixels[i + 1]), avg);
    EXPECT_EQ(int(rawPixels[i + 2]), avg);
  }
}

TEST(SimpleFilter, MakeBlackAndWhiteByBrightness) {
  MockRunner r;
  SimpleFilter sf(r);

  size_t sizeInBytes = 12;
  uchar valueBlue = 50;
  uchar valueGreen = 100;
  uchar valueRed = 200;
  uchar *data = new uchar[sizeInBytes];
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(data, 1, sizeInBytes);
  for (size_t i = 0; i < sizeInBytes; i += 4) {
    data[i] = valueBlue;
    data[i + 1] = valueGreen;
    data[i + 2] = valueRed;
  }

  uchar *rawPixels = new uchar[sizeInBytes]{};
  Matrix<uchar, MatrixType::NoCopy> lastPixels(rawPixels, 1, sizeInBytes);

  EXPECT_CALL(r, RunPerPixInWorkers(testing::_, testing::_))
      .WillOnce(MockRunner::MockRunPerPixInWorkers);

  EXPECT_NO_THROW({
    EXPECT_TRUE(sf.MakeBlackAndWhite(std::move(prevPixels),
                                     std::move(lastPixels), sizeInBytes,
                                     "Brightness"));
  });

  for (size_t i = 0; i < sizeInBytes; i += 4) {
    static const float magicRedNumber = 0.299;
    static const float magicGreenNumber = 0.587;
    static const float magicBlueNumber = 0.114;
    int avg = rawPixels[i] * magicRedNumber +
              rawPixels[i + 1] * magicGreenNumber +
              rawPixels[i + 2] * magicBlueNumber;
    EXPECT_EQ(int(rawPixels[i]), avg);
    EXPECT_EQ(int(rawPixels[i + 1]), avg);
    EXPECT_EQ(int(rawPixels[i + 2]), avg);
  }
}

TEST(SimpleFilter, MakeBlackAndWhiteByDesaturation) {
  MockRunner r;
  SimpleFilter sf(r);

  size_t sizeInBytes = 12;
  uchar valueBlue = 50;
  uchar valueGreen = 100;
  uchar valueRed = 200;
  uchar *data = new uchar[sizeInBytes];
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(data, 1, sizeInBytes);
  for (size_t i = 0; i < sizeInBytes; i += 4) {
    data[i] = valueBlue;
    data[i + 1] = valueGreen;
    data[i + 2] = valueRed;
  }

  uchar *rawPixels = new uchar[sizeInBytes]{};
  Matrix<uchar, MatrixType::NoCopy> lastPixels(rawPixels, 1, sizeInBytes);

  EXPECT_CALL(r, RunPerPixInWorkers(testing::_, testing::_))
      .WillOnce(MockRunner::MockRunPerPixInWorkers);

  EXPECT_NO_THROW({
    EXPECT_TRUE(sf.MakeBlackAndWhite(std::move(prevPixels),
                                     std::move(lastPixels), sizeInBytes,
                                     "Desaturation"));
  });

  for (size_t i = 0; i < sizeInBytes; i += 4) {
    uchar min =
        std::min(std::min(rawPixels[i], rawPixels[i + 1]), rawPixels[i + 2]);
    uchar max =
        std::max(std::max(rawPixels[i], rawPixels[i + 1]), rawPixels[i + 2]);
    uchar v = (min + max) / 2;

    EXPECT_EQ(int(rawPixels[i]), v);
    EXPECT_EQ(int(rawPixels[i + 1]), v);
    EXPECT_EQ(int(rawPixels[i + 2]), v);
  }
}

TEST(SimpleFilter, MakeToning) {
  MockRunner r;
  SimpleFilter sf(r);

  size_t sizeInBytes = 12;
  uchar value = 100;
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(1, sizeInBytes, value);

  uchar *rawPixels = new uchar[sizeInBytes]{};
  Matrix<uchar, MatrixType::NoCopy> lastPixels(rawPixels, 1, sizeInBytes);

  EXPECT_CALL(r, RunPerPixInWorkers(testing::_, testing::_))
      .WillOnce(MockRunner::MockRunPerPixInWorkers);

  BytesBGRA color{0, 0, 0, 255};
  EXPECT_NO_THROW({
    EXPECT_TRUE(sf.MakeToning(std::move(prevPixels), std::move(lastPixels),
                              sizeInBytes, color));
  });

  for (size_t i = 0; i < sizeInBytes; i += 4) {
    EXPECT_EQ(int(rawPixels[i]), int(0));
    EXPECT_EQ(int(rawPixels[i + 1]), int(0));
    EXPECT_EQ(int(rawPixels[i + 2]), int(0));
  }
}
