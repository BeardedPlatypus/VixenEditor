#pragma once
// FlatObjectRep.h contains the FlatObjectRep class, which is an
// implementation of the IObjectRep

#include <unordered_map>

#include "IObjectRep.h"


namespace VixLib {
namespace world {


/// <summary> 
/// An IObjectRep implementation based on an unordered_map.
/// </summary>
/// <seealso cref='IObjectRep'/>
class FlatObjectRep : public IObjectRep {
public:
  // --------------------------------------------------------------------------
  //  Constructor | Destructor
  // --------------------------------------------------------------------------
  FlatObjectRep();
  ~FlatObjectRep();

  // --------------------------------------------------------------------------
  //  IObjectRep methods
  // --------------------------------------------------------------------------
  virtual OptionalNode queryNode(Position pos) const override; 
  virtual void setNode(Position pos, DirectX::XMFLOAT3 col) override;
  virtual void removeNode(Position pos) override;

private:
  /// <summary> unordered map containing the nodes </summary>
  std::unordered_map<Position, std::unique_ptr<Node>> m_node_grid;
};


}
}