#pragma once
// IObjectView.h contains the IObjectView interface which contains the 
// data necessary for rendering an Object with the Renderer

#include "pch.h"
#include "VixLib\include\renderer\shaders\ShaderStructures.h"

namespace VixLib {
namespace world {


class IObjectViewRead {
public:
  // --------------------------------------------------------------------------
  // Destructor
  // --------------------------------------------------------------------------
  ///<summary> Destruct this IObjectView. </summary>
  virtual ~IObjectViewRead() {}

  // --------------------------------------------------------------------------
  // Getters
  // --------------------------------------------------------------------------
  ///<summary> Get the vertices contained in this IObjectView. </summary>
  ///<returns> The vertices descrbining an Object. </returns>
  virtual const std::vector<graphics::VertexPositionNormalColor>& getVertices() const = 0;
  ///<Summary> Get the indices contained in this IObjectView. </summary>
  ///<returns> The indices descrbining an Object. </returns>
  virtual const std::vector<unsigned short>& getIndices() const = 0;
};

}
}