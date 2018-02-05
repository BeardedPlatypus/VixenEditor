#include "pch.h"
#include "include\world\selection\Ray.h"


namespace VixLib {
namespace world {

// ----------------------------------------------------------------------------
//  Constructor 
// ----------------------------------------------------------------------------
Ray::Ray(DirectX::XMFLOAT3 direction,
  DirectX::XMFLOAT3 origin) :
  direction(direction),
  origin(origin) {
}


// --------------------------------------------------------------------------
//  Methods
// --------------------------------------------------------------------------
Ray constructRayFromView(float mouse_pos_x,
                         float mouse_pos_y,
                         const camera::CameraData& camera_data) {
  DirectX::XMMATRIX look_at = DirectX::XMLoadFloat4x4(&camera_data.look_at_matrix);

  static const DirectX::XMVECTORF32 x_axis = { 1.0f, 0.0f, 0.0f, 0.0f };
  DirectX::XMVECTOR x_axis_local = DirectX::XMVector4Transform(x_axis, look_at);
  DirectX::XMVECTOR y_axis_local = DirectX::XMLoadFloat4(&camera_data.up);
  
  static const DirectX::XMVECTORF32 z_axis = { 0.0f, 0.0f, 1.0f, 0.0f };
  DirectX::XMVECTOR z_axis_local = DirectX::XMVector4Transform(z_axis, look_at);

  // Angles in radians.
  float rot_angle_y = 0.5 * camera_data.fov_angle_y * mouse_pos_y;
  float rot_angle_x = 0.5 * camera_data.fov_angle_y * camera_data.aspect_ratio * mouse_pos_x;

  DirectX::XMMATRIX rot_matrix_y_trans = DirectX::XMMatrixRotationAxis(x_axis_local, rot_angle_y);
  DirectX::XMMATRIX rot_matrix_x_trans = DirectX::XMMatrixRotationAxis(z_axis_local, rot_angle_x);

  DirectX::XMVECTOR direction_vec = 
    DirectX::XMVector4Transform(
      DirectX::XMVector4Transform(z_axis_local, rot_matrix_y_trans),
      rot_matrix_x_trans);

  DirectX::XMFLOAT4 direction_vec4;
  DirectX::XMStoreFloat4(&direction_vec4, direction_vec);

  DirectX::XMFLOAT3 dir_ray = DirectX::XMFLOAT3(direction_vec4.x,
                                                direction_vec4.y,
                                                direction_vec4.z);
  DirectX::XMFLOAT3 dir_orig = DirectX::XMFLOAT3(camera_data.eye.x / camera_data.eye.z,
                                                 camera_data.eye.y / camera_data.eye.z,
                                                 camera_data.eye.z / camera_data.eye.z);

  return Ray(dir_ray, dir_orig);
}


}
}
