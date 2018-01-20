#include "pch.h"
#include "include\world\object-rep\FlatObjectRep.h"


namespace VixLib {
namespace world {

FlatObjectRep::FlatObjectRep() :
    m_node_grid(std::unordered_map<Position,
                                   std::unique_ptr<Node>>()) {
}


FlatObjectRep::~FlatObjectRep() {
}


OptionalNode FlatObjectRep::queryNode(Position pos) const {
  try {
    return OptionalNode(this->m_node_grid.at(pos).get());
  } catch (const std::out_of_range& e) {
    return OptionalNode();
  }
}


void FlatObjectRep::setNode(Position pos, DirectX::XMFLOAT3 col) {
  this->removeNode(pos);
  this->m_node_grid[pos] = std::make_unique<Node>(col);
}

void FlatObjectRep::removeNode(Position pos) {
  try {
    this->m_node_grid.at(pos).reset();
  } catch (const std::out_of_range& e) {
  }
}


}
}