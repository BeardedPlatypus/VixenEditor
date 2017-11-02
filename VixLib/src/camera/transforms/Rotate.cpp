#include "pch.h"
#include "include\camera\transforms\Rotate.h"

#include <iostream>  


namespace VixLib {
namespace camera {
namespace transform {

TurnTableRotate::TurnTableRotate(float factor) : m_factor(factor) {
}


// TODO fix local rotation problem
void TurnTableRotate::rotate(CameraData& data, float dx, float dy) const {
  // Camera x-axis rotations
  float angle_x = -1.0 * dy / this->m_factor;
  float angle_y = -1.0 * dx / this->m_factor;

  DirectX::XMMATRIX look_at = DirectX::XMLoadFloat4x4(&(data.look_at_matrix));
  static const DirectX::XMVECTORF32 v = { 1.0f,  0.0f, 0.0f, 0.0f };
  DirectX::XMVECTOR camera_x_axis = DirectX::XMVector4Transform(v, look_at);

  static const DirectX::XMVECTORF32 world_y_axis = { 0.0f,  1.0f, 0.0f, 0.0f };
  
  DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationAxis(camera_x_axis, angle_x) *
                               DirectX::XMMatrixRotationAxis(world_y_axis,  angle_y);

  look_at *= rotation;

  DirectX::XMVECTOR up = DirectX::XMLoadFloat4(&(data.up));
  up = DirectX::XMVector4Transform(up, rotation);

  DirectX::XMStoreFloat4x4(&data.look_at_matrix, look_at);
  DirectX::XMStoreFloat4(&(data.up), up);
}

}
}
}
