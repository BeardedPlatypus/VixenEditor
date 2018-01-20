#pragma once

#include "Direction.h"

namespace VixLib {
namespace world {

enum class Direction { XNEG, XPOS, 
                       YNEG, YPOS, 
                       ZNEG, ZPOS };

inline Position getNeighbourPositionFromDirection(Direction dir) {
  switch (dir) {
  case Direction::XNEG: return Position(-1, 0, 0);
  case Direction::XPOS: return Position(+1, 0, 0);
  case Direction::YNEG: return Position(0, -1, 0);
  case Direction::YPOS: return Position(0, +1, 0);
  case Direction::ZNEG: return Position(0, 0, -1);
  case Direction::ZPOS: return Position(0, 0, +1);
  }
}

inline Position getNeighbourPositionFromDirection(Position pos, Direction dir) {
  return pos + getNeighbourPositionFromDirection(dir);
}



}
}
