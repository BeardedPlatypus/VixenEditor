#pragma once

#include "pch.h"
#include <functional>
#include "include\world\util\Position.h"
#include "include\world\util\Direction.h"

#include "include\world\util\hash_combine.h"

namespace VixLib {
namespace world {

struct NaiveFace {
  NaiveFace(int vert_offset,
            int index_offset);

  int vert_offset;
  int index_offset;
};


struct NaiveFaceKey {
  NaiveFaceKey(Position pos, Direction dir);

  Position pos;
  Direction dir;

  bool operator==(const NaiveFaceKey& other) const {
    return (this->pos == other.pos &&
            this->dir == other.dir);
  }
};

}
}


namespace std {
template<> struct hash<VixLib::world::NaiveFaceKey> {
  typedef VixLib::world::NaiveFaceKey argument_type;
  typedef std::size_t result_type;

  result_type operator()(argument_type const& s) const noexcept {
    result_type seed = 0;

    VixLib::util::hash_combine(seed, std::hash<VixLib::world::Position>()(s.pos));

    int h4_val;
    switch (s.dir) {
    case VixLib::world::Direction::XNEG: h4_val = 0;
                                         break;
    case VixLib::world::Direction::XPOS: h4_val = 1;
                                         break;
    case VixLib::world::Direction::YNEG: h4_val = 2;
                                         break;
    case VixLib::world::Direction::YPOS: h4_val = 3;
                                         break;
    case VixLib::world::Direction::ZNEG: h4_val = 4;
                                         break;
    case VixLib::world::Direction::ZPOS: h4_val = 5;
                                         break;
    }
    VixLib::util::hash_combine(seed, h4_val);
    return seed;
  }
};
}
