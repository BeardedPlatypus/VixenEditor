#pragma once

#include "VixCommon\include\DeviceResources.h"

namespace VixLib {
namespace camera {

/// <summary> 
/// CameraData holds all data needed to visualise a Camera. 
/// 
/// This includes the look at matrix and the perspective matrix as well as the
/// look at matrix and the depth range.
/// </summary>
/// 
struct CameraData {
  // -------------------------------------------------------------------------
  // Constructor
  // -------------------------------------------------------------------------
  /// <summary> Construct a new CameraData </summary>
  CameraData(DirectX::XMFLOAT4 eye,
             DirectX::XMFLOAT4 center,
             DirectX::XMFLOAT4 up,
             float fov_angle_y,
             float aspect_ratio,
             float z_near,
             float z_far,
             DirectX::XMFLOAT4X4 orientation);

  // -------------------------------------------------------------------------
  // Methods
  // -------------------------------------------------------------------------
  void recalculateLookAtMatrix();

  /// <summary> 
  /// Recalculate the perspective matrix based on the current fields
  /// </summary>
  void recalculatePerspectiveMatrix();

  // -------------------------------------------------------------------------
  // Fields
  // -------------------------------------------------------------------------
  /// <summary> The model to camera coordinates matrix </summary>
  DirectX::XMFLOAT4X4 look_at_matrix;
  /// <summary> The center of this CameraData </summary>
  /// <summary> The zoom of this CameraData </summary>
  float zoom;

  DirectX::XMFLOAT4 eye;
  DirectX::XMFLOAT4 center;
  DirectX::XMFLOAT4 up;
  // -------------------------------------------------------------------------
  /// <summary> The perspective matrix of this Cameradata </summary>
  DirectX::XMFLOAT4X4 perspective_matrix;
  /// <summary> The orientation matrix of this Cameradata </summary>
  DirectX::XMFLOAT4X4 orientation_matrix;
  /// <summary> The projection matrix of this Cameradata </summary>
  DirectX::XMFLOAT4X4 projection_matrix;

  // -------------------------------------------------------------------------
  /// <summary> The Field of View angle </summary>
  float fov_angle_y;
  /// <summary> The aspect ratio </summary>
  float aspect_ratio;
  /// <summary> The z-distance to the near plane of the viewfrustum </summary>>
  float z_near;
  /// <summary> The z-distance to the far plane of the viewfrustum </summary>>
  float z_far;
};


} // camera
} // VixLib
