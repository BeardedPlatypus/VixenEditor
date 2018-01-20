#pragma once

#include <functional>

namespace VixLib {
namespace util {

inline void hash_combine(std::size_t& seed, int const& v) {
  seed ^= std::hash<int>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

}
}