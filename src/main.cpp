#define ANKERL_NANOBENCH_IMPLEMENT
#include <cstdint>
#include <limits>
#include <map>
#include <nanobench.h>
#include <random>
#include <unordered_map>

template <typename T, typename R = std::mt19937_64>
T random(const T &min, const T &max) {
  R rng{std::random_device{}()};
  if constexpr (std::is_integral<T>::value) {
    std::uniform_int_distribution<T> dist(min, max);
    return dist(rng);

  } else if (std::is_floating_point<T>::value) {
    std::uniform_real_distribution<T> dist(min, max);
    return dist(rng);
  } else {
    // If T is unsupported type...
    return T{};
  }
}

template <typename T> void fill_random(std::vector<T> &vec) {
  for (auto &slot : vec) {
    T value =
        random<T>(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    slot = value;
  }
}

int main(void) {

  std::vector<size_t> size_t_values_100(100);
  std::vector<size_t> size_t_values_1000(1000);
  std::vector<size_t> size_t_values_10000(10000);
  fill_random<size_t>(size_t_values_100);
  std::vector<int8_t> int8_t_keys_100(100);
  fill_random<int8_t>(int8_t_keys_100);

  std::unordered_map<int8_t, size_t> std_unordered_map_int8_t;
  ankerl::nanobench::Bench().run(
      "std::unordered_map insert 100 random int8_t", [&] {
        for (int i = 0; i < 100; i++) {
          std_unordered_map_int8_t[int8_t_keys_100[i]] = size_t_values_100[i];
        }
      });

  std::map<int8_t, size_t> std_map_int8_t;
  ankerl::nanobench::Bench().run("std::map insert 100 random int8_t", [&] {
    for (int i = 0; i < 100; i++) {
      std_map_int8_t[int8_t_keys_100[i]] = size_t_values_100[i];
    }
  });

  ankerl::nanobench::Bench().run(
      "std::unordered_map iterate 100 random int8_t", [&] {
        int8_t key_sum = 0;
        size_t value_sum = 0;
        for (const auto &[key, value] : std_unordered_map_int8_t) {
          key_sum += key;
          value_sum += value;
        }
      });
  ankerl::nanobench::Bench().run("std::map iterate 100 random int8_t", [&] {
    int8_t key_sum = 0;
    size_t value_sum = 0;
    for (const auto &[key, value] : std_map_int8_t) {
      key_sum += key;
      value_sum += value;
    }
  });
  return 0;
}
