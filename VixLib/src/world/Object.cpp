#include "pch.h"
#include "include\world\Object.h"

#include "include\world\util\Direction.h"


namespace VixLib {
namespace world {

// ----------------------------------------------------------------------------
//  Constructor | Destructor
// ----------------------------------------------------------------------------
Object::Object() {
}


Object::~Object() {
}


// --------------------------------------------------------------------------
//  Methods
// --------------------------------------------------------------------------
OptionalNode Object::queryNode(Position position) const {
  return this->m_obj_rep->queryNode(position);
}


void Object::setNode(Position position, DirectX::XMFLOAT3 col) {
  this->m_obj_rep->setNode(position, col);

  // Check if any of the 6 faces need to be added or removed.
  Direction faces[6] = { Direction::XNEG, Direction::XPOS,
                         Direction::YNEG, Direction::YPOS,
                         Direction::ZNEG, Direction::ZPOS };

  OptionalNode neighbouring_node;
  for (int i = 0; i < 6; i++) {
    neighbouring_node = this->queryNode(getNeighbourPositionFromDirection(position, faces[i]));
    if (neighbouring_node.hasValue()) {
      this->m_obj_view->removeFace(position, faces[i]);
    } else {
      this->m_obj_view->addFace(position, faces[i], col);
    }
  }
}


}
}
