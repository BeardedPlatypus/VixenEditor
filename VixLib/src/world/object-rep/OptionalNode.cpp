#include "pch.h"
#include "include\world\object-rep\OptionalNode.h"

#include <stdexcept>


namespace VixLib {
namespace world {

OptionalNode::OptionalNode() : m_has_value(false),
                               p_node(nullptr) {
}


OptionalNode::OptionalNode(Node* p_node) {
  if (!p_node) {
    throw std::invalid_argument("p_node is NULL");
  }

  this->m_has_value = true;
  this->p_node = p_node;
}


const Node& OptionalNode::getValue() const {
  if (!this->hasValue()) {
    throw std::logic_error("Requesting value of empty OptionalNode");
  }

  return *(this->p_node);
}


}
}