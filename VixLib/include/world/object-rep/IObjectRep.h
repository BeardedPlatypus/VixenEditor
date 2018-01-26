#pragma once
// IObjectRep.h contains the IObject class used to represent all the object
// representations used within the Object.

// ----------------------------------------------------------------------------
// Include statements
// ----------------------------------------------------------------------------
#include "pch.h"
#include "Node.h"
#include "OptionalNode.h"

#include "VixLib\include\world\util\Position.h"

namespace VixLib {
namespace world {

/// <summary> 
/// The IObjectRep is an interface which should be implemented by any class
/// which represents the internal structure of an object. 
/// Every object consists out of a number of cubes, called <see cref="Nodes"/>.
/// Each Node has an implicit position. 
/// IObjectRep is responsible for retrieving said <see cref="Nodes"/> as well as
/// constructing and deleting them.
/// </summary>
class IObjectRep {
public:
  /// <summary> Destruct this IObjectRep </summary>
  virtual ~IObjectRep() {}

  /// <summary> Attempt to query the Node at the <paramref name='pos'/> </summary>
  /// <param name='pos'> The position to retrieve the node from </param>
  virtual OptionalNode queryNode(Position pos) const = 0;

  /// <summary> Set the node at the given <paramref name='pos'> to <paramref name='col'/>.</summary>
  /// <param name='pos'> The position at which the node should be set. </param>
  /// <param name='col'> The new colour of the specified node. </param>
  /// <post-condition> (new this).queryNode(pos).getValue().getColour() == col </post-condition>
  virtual void setNode(Position pos,
                       DirectX::XMFLOAT3 col) = 0;
  /// <summary> Remove the node at the given <paramref name='pos'>.</summary>
  /// <param name='pos'> The position at which the node should be removed. </param>
  /// <post-condition> Not (new this).queryNode(pos).hasValue() </post-condition>
  virtual void removeNode(Position pos) = 0;
};

}
}
