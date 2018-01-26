#pragma once

#include "../IObjectView.h"
#include "NaiveFace.h"

#include "include/renderer/shaders/ShaderStructures.h"

#include <unordered_map>

namespace VixLib {
namespace world {

class NaiveObjectView : public IObjectView {
public:
  // --------------------------------------------------------------------------
  //  Constructor
  // --------------------------------------------------------------------------
  NaiveObjectView();
  virtual ~NaiveObjectView() override;

  // --------------------------------------------------------------------------
  //  Getters
  // --------------------------------------------------------------------------
  virtual const std::vector<graphics::VertexPositionNormalColor>& getVertices() const override { return this->vertices; }
  virtual const std::vector<unsigned short>& getIndices() const override { return this->indices; }

  // --------------------------------------------------------------------------
  //  Interaction Methods
  // --------------------------------------------------------------------------
  virtual void addFace(Position pos, Direction dir, DirectX::XMFLOAT3 col) override;
  virtual void removeFace(Position pos, Direction dir) override;

  virtual void setColour(Position pos, Direction dir, DirectX::XMFLOAT3 col);

private:
  std::vector<graphics::VertexPositionNormalColor> vertices;
  std::vector<unsigned short> indices;

  std::unordered_map<NaiveFaceKey, NaiveFace*> face_map;
  std::vector<NaiveFace*> faces;
};

}
}
