#pragma once
// Node.h contains the definition of Node, describing a single Voxel node.

// ----------------------------------------------------------------------------
// Include statements
// ----------------------------------------------------------------------------
#include "pch.h"

namespace VixLib {
namespace world {

/// <summary> 
/// The Node class describes a single voxel node within an Object.
/// </summary>
class Node {
public:
  // --------------------------------------------------------------------------
  // Constructor | Destructor 
  // --------------------------------------------------------------------------
  /// <summary> Construct a new Node with the given <paramref name='col'/>.
  /// <param name='col'> The colour of this new Node. </param>
  Node(DirectX::XMFLOAT3 col);
  /// <summary> Destruct this Node. </summary>
  ~Node();

  // --------------------------------------------------------------------------
  // Getters
  // --------------------------------------------------------------------------
  /// <summary> Get the colour of this Node. </summary>
  DirectX::XMFLOAT3 getColour() const { return this->m_col; }

private:
  /// <summary> The colour of this Node. </summary>
  DirectX::XMFLOAT3 m_col;
};

}
}
