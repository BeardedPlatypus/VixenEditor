#pragma once
// IObjectView.h contains the IObjectView interface which contains the 
// data necessary for rendering an Object with the Renderer

#include "pch.h"
#include "IObjectViewRead.h"
#include "VixLib\include\world\util\Position.h"
#include "VixLib\include\world\util\Direction.h"


namespace VixLib {
namespace world {

/// <summary>
/// The IObjectView is the interface describing an Object to be used by 
/// the Renderer.
/// </summary>
class IObjectView : public IObjectViewRead {
public:
  // --------------------------------------------------------------------------
  // Destructor
  // --------------------------------------------------------------------------
  ///<summary> Destruct this IObjectView. </summary>
  virtual ~IObjectView() {}

  // --------------------------------------------------------------------------
  // Getters
  // --------------------------------------------------------------------------
  ///<summary> Get the vertices contained in this IObjectView. </summary>
  ///<returns> The vertices descrbining an Object. </returns>
  virtual const std::vector<graphics::VertexPositionNormalColor>& getVertices() const = 0;
  ///<Summary> Get the indices contained in this IObjectView. </summary>
  ///<returns> The indices descrbining an Object. </returns>
  virtual const std::vector<unsigned short>& getIndices() const = 0;

  // --------------------------------------------------------------------------
  // Interaction Methods
  // --------------------------------------------------------------------------
  ///<summary> 
  ///Add a voxel face at the specified <paramref name='pos'/> and 
  ///<paramref name='dir'/> with the specified <paramref name='col/>. 
  ///</summary>
  ///<param name='pos'> The Position at which a face should be created. </param>
  ///<param name='pos'> The Direction of the face at the position which a face should be created. </param>
  ///<param name='col'> The colour of the created face. </param>
  virtual void addFace(Position pos, Direction dir, DirectX::XMFLOAT3 col) = 0;

  ///<summary>
  /// Remove the face at the specified <paramref name='pos'/> and <paramref name='dir'/>.
  /// </summary>
  virtual void removeFace(Position pos, Direction dir) = 0;

  /// <summary>
  /// Set the colour of the face to the specified colour. 
  /// </summary>
  /// <param name='col'> The new colour of the specified face. </param>
  virtual void setColour(Position pos, Direction dir, DirectX::XMFLOAT3 col) = 0;
};

}
}