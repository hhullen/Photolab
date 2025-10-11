#include <gtest/gtest.h>

#include "support.h"

TEST(Support, GetPixChannsBGR) {

  int size = 40;
  int bytesPerPixel = 4;
  char *data = new char[size];
  for (int i = 0; i < size; i += bytesPerPixel) {
    data[i] = 10;
    data[i + 1] = 20;
    data[i + 2] = 30;
    data[i + 3] = 40;
  }

  auto [b, g, r] = Support::GetPixChannsBGR(&data[0]);
  EXPECT_EQ(b, 10);
  EXPECT_EQ(g, 20);
  EXPECT_EQ(r, 30);

  auto [b1, g1, r1] = Support::GetPixChannsBGR(&data[8]);
  EXPECT_EQ(b1, 10);
  EXPECT_EQ(g1, 20);
  EXPECT_EQ(r1, 30);

  auto [b2, g2, r2] = Support::GetPixChannsBGR(&data[24]);
  EXPECT_EQ(b2, 10);
  EXPECT_EQ(g2, 20);
  EXPECT_EQ(r2, 30);
}
