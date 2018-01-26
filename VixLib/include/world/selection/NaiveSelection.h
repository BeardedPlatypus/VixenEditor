#pragma once

// ----------------------------------------------------------------------------
// Include statements
// ----------------------------------------------------------------------------
#include "ISelection.h"


namespace VixLib {
namespace world {

class NaiveSelection : public ISelection {
public:
  // --------------------------------------------------------------------------
  // Constructor 
  // --------------------------------------------------------------------------
  NaiveSelection();

  // --------------------------------------------------------------------------
  // Getters
  // --------------------------------------------------------------------------
  virtual bool isSelected(const VoxelFace& face) const override;

  // --------------------------------------------------------------------------
  // Interaction Methods
  // --------------------------------------------------------------------------
  virtual void addNewSelection(const std::vector<VoxelFace>& selection, 
                               IObjectView& obj_view) override;

  virtual void removeSelection(IObjectView& obj_view) override;

  virtual void extendSelection(const std::vector<VoxelFace>& selection, 
                               IObjectView& obj_view) override;

private:
  /// <summary> A vector containing the current selection </summary>
  std::vector<VoxelFace> current_selection;
};


}
}
