#pragma once
// OptionalNode.h contains the OptionalNode class used to retrieve Nodes
// without knowing if they actually exist.


// ----------------------------------------------------------------------------
// Include statements
// ----------------------------------------------------------------------------
#include "Node.h"


namespace VixLib {
namespace world {

/// <summary> 
/// The OptionalNode is used to retrieve Nodes from an Object without 
/// knowing if they exist at a given position.
/// </summary>
class OptionalNode {
public:
  // --------------------------------------------------------------------------
  //  Constructor | Destructor
  // --------------------------------------------------------------------------
  /// <summary> Construct a new empty OptionalNode. </summary>
  OptionalNode();
  /// <summary>
  /// Construct a new OptionalNode containing <paramref name='node'/>.
  /// </summary>
  /// <param name='node'> Pointer to the Node contained in this new OptionalNode. </param>
  OptionalNode(Node* node);


  // --------------------------------------------------------------------------
  //  Methods
  // --------------------------------------------------------------------------
  /// <summary> Check whether this OptionalNode contains a reference to a Node </summary>
  /// <returns> True if this OptionalNode contains a node, False otherwise. </returns>
  inline bool hasValue() const { return this->m_has_value; }
  /// <summary> Get the value of this OptionalNode, if it exists. </summary>
  /// <returns> A constant reference to the value of this OptionalNode if it exists </returns>
  const Node& getValue() const;

private:
  /// <summary> Whether this OptionalNode contains a reference to a Node. </summary>
  bool m_has_value;
  /// <summary> Pointer to the Node contained in this OptionalNode, Null if empty. </summary>
  Node* p_node;
};

}
}
