#pragma once
// Ray.h describes the Ray class, used to trace rays within the scene

// ----------------------------------------------------------------------------
// Include statements
// ----------------------------------------------------------------------------
#include "pch.h"
#include "include\camera\CameraData.h"


namespace VixLib {
namespace world {

/// <summary> 
/// The Ray class describes a single Ray with an origin and a direction.
/// </summary>
class Ray {
public:
  /// <summary>
  /// Construct a new Ray with the given <paramref name='direction'/> and 
  /// <paramref name='origin'/>
  /// </summary>
  /// <param name='direction'> The new direction of this new Ray </param>
  /// <param name='origin'> The new origin of this new Ray </param>
  Ray(DirectX::XMFLOAT3 direction,
       DirectX::XMFLOAT3 origin);

  /// <summary>
  /// Get the direction of this Ray.
  /// </summary>
  /// <returns> The direction as DirectX::XMFLOAT3 </returns>
  DirectX::XMFLOAT3 getDirection() const { return this->direction; }

  /// <summary>
  /// Get the origin of this Ray.
  /// </summary>
  /// <returns> The origin as DirectX::XMFLOAT3 </returns>
  DirectX::XMFLOAT3 getOrigin() const { return this->origin; }

private:
  /// <summary> The direction of this Ray. </summary>
  const DirectX::XMFLOAT3 direction;
  /// <summary> The origin of this Ray </summary>
  const DirectX::XMFLOAT3 origin;
};


/// <summary>
/// Construct a new ray based on the 
/// </summary>
  /// <param name='relative_mouse_pos_x'> Current x-axis location of the mouse cursor
  ///                                     where -1 is the left-most point, and 1 the 
  ///                                     right-most point. </param>
  /// <param name='relative_mouse_pos_y'> Current y-axis location of the mouse cursor
  ///                                     where -1 is the upper-most point, and 1 the 
  ///                                     down-most point. </param>
Ray constructRayFromView(float relative_mouse_pos_x, 
                         float relative_mouse_pos_y,
                         const camera::CameraData& camera_data);
}
}
