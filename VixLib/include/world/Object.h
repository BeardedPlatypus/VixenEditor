#pragma once
// Object.h describes the Object class, used to store a single Object 
// consisting of multiple voxels.


// ----------------------------------------------------------------------------
// Include statements
// ----------------------------------------------------------------------------
#include "pch.h"

#include "include\world\util\Position.h"

#include "./object-rep/Node.h"
#include "./object-rep/OptionalNode.h"

#include "./object-rep/IObjectRep.h"
#include "./object-view/IObjectView.h"


namespace VixLib {
namespace world {

/// <summary>
/// The Object class is used to store a single Object containing a set of 
/// voxels. The object class serves as an entry point to manipulate the voxels
/// contained in the Object.
/// </summary>
/// <remarks>
/// The Object provides method to query, set and remove nodes, as well as 
/// query a view which can be used within the Renderer.
/// </remarks>
class Object {
public:
  // --------------------------------------------------------------------------
  //  Constructor | Destructor
  // --------------------------------------------------------------------------
  /// <summary> Construct a new Object. </summary>
  Object();
  /// <summary> Destruct this Object. </summary>
  ~Object();

  // --------------------------------------------------------------------------
  //  Methods
  // --------------------------------------------------------------------------
  /// <summary> 
  /// Get an OptionalNode which contains the Node at <paramref name='position'/> 
  /// if it exists, else  it will be empty.
  /// </summary>
  /// <param name='position'> The position of the Node to be retrieved. </param>
  /// <returns> 
  /// An OptionalNode which contains the node at <paramref name='position'/> 
  /// if such a node exists
  /// </returns>
  OptionalNode queryNode(Position position) const;

  /// <summary>
  /// Set the node at <paramref name='position'/> to the given <paramref name='col'/>
  /// </summary>
  /// <param name='position'> The position of the Node to be set. </param>
  /// <param name='col'> The new colour of the Node at <paramref name='position'/>. </param>
  /// <post-condition> (new this).queryNode(position).getValue()->getColour == col  </post-condition>
  void setNode(Position position, DirectX::XMFLOAT3 col);
  /// <summary> 
  /// Remove the Node at <paramref name='position'/> in this Object. 
  /// </summary>
  /// <param name='position'> The position of the Node to be removed. </param>
  /// <post-condition> Not (new this).queryNode(position).hasValue </post-condition>
  void removeNode(Position position);

  // --------------------------------------------------------------------------
  // TODO: change this to the appropriate type
  /// <summary>
  /// Get the view of this Object to be used within the Renderer.
  /// <summary>
  void queryView() const;

private:
  /// <summary> The IObjectRep of this Object. </summary>
  std::unique_ptr<IObjectRep> m_obj_rep;
  /// <summary> The IObjectView of this Object. </summary>
  std::unique_ptr<IObjectView> m_obj_view;
};
   

}
}
