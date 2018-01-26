#include "pch.h"
#include "include\world\selection\NaiveSelection.h"


#define FACE_COLOUR { 1.2f, 0.84f, 0.336f }

namespace VixLib {
namespace world {

// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
NaiveSelection::NaiveSelection() : current_selection(std::vector<VoxelFace>()) {
}


// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------
bool NaiveSelection::isSelected(const VoxelFace& face) const {
  for (const VoxelFace& f : this->current_selection) {
    if (f == face) {
      return true;
    }
  }
  return false;
}


void NaiveSelection::addNewSelection(const std::vector<VoxelFace>& selection,
                                     IObjectView& obj_view) {
  // TODO: replace this with a more elegant solution.
  this->removeSelection(obj_view);
  for (const VoxelFace& f : selection) {
    this->current_selection.push_back(f);
    obj_view.setColour(f.getPosition(), f.getDirection(), FACE_COLOUR);
  }
}

void NaiveSelection::removeSelection(IObjectView& obj_view) {
  for (const VoxelFace& f : this->current_selection) {
    obj_view.setColour(f.getPosition(), f.getDirection(), f.getNode().getColour());
  }
  this->current_selection.clear();
}

void NaiveSelection::extendSelection(const std::vector<VoxelFace>& selection,
                                     IObjectView& obj_view) {
  bool in_current_selection;
  for (const VoxelFace& f1 : selection) {
    in_current_selection = false;
    for (const VoxelFace& f2 : this->current_selection) {
      if (f1 == f2) {
        in_current_selection = true;
        break;
      }
    }

    if (!in_current_selection) {
      obj_view.setColour(f1.getPosition(), f1.getDirection(), FACE_COLOUR);
      // TODO fix this so it does not iterate twice
      this->current_selection.push_back(f1);
    }
  }
}

}
}