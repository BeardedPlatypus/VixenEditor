#pragma once
// VoxelFace.h contains the definition of the VoxelFace class, responsible for 
// keeping track of a single face of a single VoxelNode


// ----------------------------------------------------------------------------
// Include statements
// ----------------------------------------------------------------------------
#include "include\world\util\Position.h"
#include "include\world\util\Direction.h"
#include "include\world\object-rep\Node.h"


namespace VixLib {
namespace world {

/// <summary> 
/// The VoxelFace class describes a single face of a Voxel
/// </summary>
class VoxelFace {
public:
  /// <summary> 
  /// Construct a new VoxelFace with the given <paramref name='pos'/> and 
  /// <paramref name='dir'/>.
  /// </summary>
  /// <param name='pos'> The position of the voxel to which this VoxelFace belongs </param>
  /// <param name='dir'> The direction of the face on the specified voxel. </param>
  VoxelFace(Position pos, Direction dir, const Node& node);

  // --------------------------------------------------------------------------
  // Getters
  // --------------------------------------------------------------------------
  /// <summary> Get the position of this VoxelFace </summary>
  /// <returns> The Position of this VoxelFace. </returns>
  Position getPosition() const { return this->m_pos; }

  /// <summary> Get the direction of this VoxelFace </summary>
  /// <returns> The direction of this VoxelFace. </returns>
  Direction getDirection() const { return this->m_dir; }

  /// <summary> Get the Node associated with this VoxelFace </summary>
  /// <returns> The Node associated with this VoxelFace. </returns>
  const Node& getNode() const { return this->m_node; }

  // --------------------------------------------------------------------------
  // Operators
  // --------------------------------------------------------------------------
  bool operator==(const VoxelFace& rhs) const {
    return (this->m_pos == rhs.getPosition() && this->m_dir == rhs.getDirection());
  }

private:
  /// <summary> The Position of the voxel this VoxelFace belongs to. </summary>
  Position m_pos;
  /// <summary> The Direction of this VoxelFace on the voxel. </summary>
  Direction m_dir;
  /// <summary> The Node associated with this VoxelFace. </summary>
  const Node& m_node;
};


}
}