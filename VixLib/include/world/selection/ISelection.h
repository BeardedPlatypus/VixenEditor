#pragma once
// ISelection.h contains the Selection interface used within the Object


// ----------------------------------------------------------------------------
// Include statements
// ----------------------------------------------------------------------------
#include <vector>
#include "VoxelFace.h"
#include "include\world\object-view\IObjectView.h"


namespace VixLib {
namespace world {

/// <summary>
/// The ISelection interface describes all the function which need to 
/// be implemented by any selection class used within the Object. 
/// The Selection object manages active selection within the object
/// </summary>
class ISelection {
public:
  /// <summary> Destruct this ISelection interface </summary>
  virtual ~ISelection() { }

  // --------------------------------------------------------------------------
  // Getters
  // --------------------------------------------------------------------------
  /// <summary> 
  /// Check if the specified <paramref name='face'/> is part of the current 
  /// selection.
  /// </summary>
  /// <param name='face'> The face to be checked. </param>
  /// <returns> 
  /// True if <paramref name='face'/> is part of the selection
  /// False otherwise.
  /// </returns>
  virtual bool isSelected(const VoxelFace& face) const = 0;

  // --------------------------------------------------------------------------
  // Interaction Methods
  // --------------------------------------------------------------------------
  /// <summary> 
  /// Create a new selection based on the given <paramref name='selection'/>
  /// and update the <paramref name='obj_view'/> accordingly.
  /// </summary>
  /// <param name='selection'> A vector containing all faces part of the new
  ///                          selection. </param>
  /// <param name='obj_view'> The ObjectView which will be updated with the 
  ///                         specifiied selection. </param>
  /// <post-condition> 
  /// Forall f : VoxelFace (new this).isSelected(f) == (f in selection) 
  /// </post-condition>
  virtual void addNewSelection(const std::vector<VoxelFace>& selection, 
                               IObjectView& obj_view) = 0;

  /// <summary> Remove the current selection. </summary>
  /// <param name='obj_view'> The ObjectView which will be updated. </param>
  /// <post-condition>
  /// Forall f : VoxelFace Not (new this).isSelected(f)
  /// </post-condition>
  virtual void removeSelection(IObjectView& obj_view) = 0;

  /// <summary> 
  /// Extend the current selection with the specified 
  /// <paramref name='selection'/>.
  /// </summary>
  /// <param name='selection'> A vector containing all faces which will be 
  ///                          added to the selection. </param>
  /// <param name='obj_view'> The ObjectView which will be updated with the 
  ///                         extended selection. </param>
  /// <post-condition> 
  /// Forall f : VoxelFace ((new this).isSelected(f) And 
  ///                       (this.isSelected(f) Or f In selection)) ||
  ///                      (Not (new this).isSelected(f) And
  ///                       Not this.isSelected(f) And Not f in selection)
  /// </post-condition>
  virtual void extendSelection(const std::vector<VoxelFace>& selection, 
                               IObjectView& obj_view) = 0;
};

}
}
