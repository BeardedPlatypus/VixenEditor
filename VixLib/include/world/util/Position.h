#pragma once
// Position.h contains the position object used to refer to position within
// an Object.


// ----------------------------------------------------------------------------
// Include statements
// ----------------------------------------------------------------------------
#include "pch.h"
#include <functional>

#include "include\world\util\hash_combine.h"

namespace VixLib {
namespace world {


class Position {
public:
  Position(int x, int y, int z);
  Position(DirectX::XMINT3 pos);
  Position();

  int x;
  int y;
  int z;

  DirectX::XMINT3 toXMINT3() const { return DirectX::XMINT3(x, y, z); }

  int& operator[](int index) {
    if (index == 0) return this->x;
    else if (index == 1) return this->y;
    else if (index == 2) return this->z;
    else throw std::out_of_range("value smaller than zero or greater than 2.");
  }

  bool operator==(const Position& rhs) const {
    return (this->x == rhs.x &&
            this->y == rhs.y &&
            this->z == rhs.z);
  }

  Position& operator+=(const Position& rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;
    return *this;
  }

  friend Position operator+(Position lhs,
                            const Position& rhs) {
    lhs += rhs;
    return lhs;
  }

  Position& operator-=(const Position& rhs) {
    this->x -= rhs.x;
    this->y -= rhs.y;
    this->z -= rhs.z;
    return *this;
  }

  friend Position operator-(Position lhs,
                            const Position& rhs) {
    lhs -= rhs;
    return lhs;
  }
};


}
}


namespace std {
template<> struct hash<VixLib::world::Position> {
  typedef VixLib::world::Position argument_type;
  typedef std::size_t result_type;

  result_type operator()(argument_type const& s) const noexcept {
    result_type seed = 0;

    VixLib::util::hash_combine(seed, s.x);
    VixLib::util::hash_combine(seed, s.y);
    VixLib::util::hash_combine(seed, s.z);
    return seed;
  }
};
}

