#include <chrono>
#include <gtest/gtest.h>
#include <thread>

#include "channel.h"

TEST(ChannelTest, construct) {
  EXPECT_NO_THROW([] { Channel<int> ch = Channel<int>(); }());
  EXPECT_NO_THROW([] { Channel<int> ch = Channel<int>(100000); }());
  EXPECT_ANY_THROW([] { Channel<int> ch = Channel<int>(0); }());
}

TEST(ChannelTest, SendAndRecieve) {
  Channel<int> channel(3);
  channel.Send(1);
  channel.Send(2);
  channel.Send(3);

  auto v1 = channel.Recv();
  EXPECT_TRUE(v1.has_value());

  auto v2 = channel.Recv();
  EXPECT_TRUE(v2.has_value());

  auto v3 = channel.Recv();
  EXPECT_TRUE(v3.has_value());

  EXPECT_EQ(v1.value(), 1);
  EXPECT_EQ(v2.value(), 2);
  EXPECT_EQ(v3.value(), 3);
}

TEST(ChannelTest, SendWhenFull) {
  Channel<int> channel(1);
  channel.Send(10);

  bool done = false;
  auto t = std::thread([&done, &channel]() {
    channel.Send(20);
    done = true;
  });

  auto waitTime = 100;
  std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
  EXPECT_FALSE(done);

  channel.Recv();
  std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
  EXPECT_TRUE(done);
  t.join();
}

TEST(ChannelTest, SendAndRecieveIfClosed) {
  Channel<int> channel(3);
  channel.Send(1);
  channel.Close();
  channel.Send(2);
  channel.Send(3);
  channel.Send(4);

  auto v = channel.Recv();
  auto v1 = channel.Recv();
  EXPECT_FALSE(v.has_value());
  EXPECT_FALSE(v1.has_value());
  EXPECT_TRUE(channel.IsClosed());
}
