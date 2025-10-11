#include <gtest/gtest.h>

#include "colorcorrection.cc"
#include "runner_mock.h"

const int delay = 400;

TEST(ColorCorrection, construct) {
  MockRunner r = MockRunner();
  EXPECT_NO_THROW({ ColorCorrection cc(r); });
}

TEST(ColorCorrection, MakeSimpleColorCorrection) {
  MockRunner r = MockRunner();
  ColorCorrection cc(r);

  size_t sizeInBytes = 12;
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(1, sizeInBytes);
  Matrix<uchar, MatrixType::NoCopy> lastPixels(1, sizeInBytes);

  EXPECT_CALL(r, RunPerPixInWorkers(testing::_, testing::_))
      .WillOnce(MockRunner::MockRunPerPixInWorkers);

  EXPECT_NO_THROW({
    EXPECT_TRUE(cc.MakeSimpleColorCorrection(std::move(prevPixels),
                                             std::move(lastPixels), sizeInBytes,
                                             int(255), int(512)));
  });
}

TEST(ColorCorrection, MakeHSVColorCorrection) {
  MockRunner r = MockRunner();
  ColorCorrection cc(r);

  size_t sizeInBytes = 100000;
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(1, sizeInBytes);
  Matrix<uchar, MatrixType::NoCopy> lastPixels(1, sizeInBytes);

  EXPECT_CALL(r, RunPerPixInWorkers(testing::_, testing::_))
      .WillOnce(MockRunner::MockRunPerPixInWorkers);

  EXPECT_NO_THROW({
    cc.MakeHSVColorCorrection(std::move(prevPixels), std::move(lastPixels),
                              sizeInBytes, int(100), int(700), int(700));
  });
}

TEST(ColorCorrection, MakeHSLColorCorrection) {
  MockRunner r = MockRunner();
  ColorCorrection cc(r);

  size_t sizeInBytes = 100000;
  Matrix<const uchar, MatrixType::NoCopy> prevPixels(1, sizeInBytes);
  Matrix<uchar, MatrixType::NoCopy> lastPixels(1, sizeInBytes);

  EXPECT_CALL(r, RunPerPixInWorkers(testing::_, testing::_))
      .WillOnce(MockRunner::MockRunPerPixInWorkers);

  EXPECT_NO_THROW({
    cc.MakeHSLColorCorrection(std::move(prevPixels), std::move(lastPixels),
                              sizeInBytes, int(100), int(700), int(700));
  });
}
