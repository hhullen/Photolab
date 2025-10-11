#ifndef SUPPORT_H
#define SUPPORT_H

#include <cstddef>
#include <tuple>

namespace Support {

constexpr size_t kBlueIdx{0};
constexpr size_t kGreenIdx{1};
constexpr size_t kRedIdx{2};

template <typename T> std::tuple<T &, T &, T &> GetPixChannsBGR(T *data) {
  return {data[kBlueIdx], data[kGreenIdx], data[kRedIdx]};
}

} // namespace Support

#endif // SUPPORT_H
