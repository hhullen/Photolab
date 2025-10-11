#include <cstddef>
#include <cstring>
#include <exception>
#include <gtest/gtest.h>

#include "runner.h"

const int delay = 400;

TEST(Runner, construct) {

  EXPECT_NO_THROW([] { Runner r = Runner(4); }());
}

TEST(Runner, getWorkers) {

  Runner r = Runner(4);

  EXPECT_NO_THROW({ r.getWorkers(); });
}

TEST(Runner, RunPerPixInWorkers) {

  size_t sizeInBytes = 12;
  char arr[sizeInBytes];
  memset(arr, 0, sizeInBytes);

  Runner r = Runner(4);

  char newVal = 50;
  auto func = [&arr, newVal](size_t i) {
    arr[i] = newVal;
    arr[i + 1] = newVal;
    arr[i + 2] = newVal;
    arr[i + 3] = newVal;
  };

  r.RunPerPixInWorkers(sizeInBytes, func);

  for (size_t i = 0; i < sizeInBytes; i++) {
    EXPECT_EQ(arr[i], newVal);
  }
}

TEST(Runner, RunPerPixInWorkersWrongBytes) {

  size_t sizeInBytes = 10;
  char arr[sizeInBytes];
  memset(arr, 0, sizeInBytes);

  Runner r = Runner(4);

  char newVal = 50;
  auto func = [&arr, newVal](size_t i) {
    arr[i] = newVal;
    arr[i + 1] = newVal;
    arr[i + 2] = newVal;
    arr[i + 3] = newVal;
  };

  r.RunPerPixInWorkers(sizeInBytes, func);

  for (size_t i = 0; i < sizeInBytes; i++) {
    EXPECT_EQ(arr[i], 0);
  }
}
