#include "pch.h"
#include "include\camera\transforms\Rotate.h"

#include <iostream>  


namespace VixLib {
namespace camera {
namespace transform {

TurnTableRotate::TurnTableRotate(float factor) : m_factor(factor) {
}


// TODO: Fix local rotation problem
void TurnTableRotate::rotate(CameraData& data, float dx, float dy) const {
  // Camera x-axis rotations
  float angle_x = -1.0 * dy / this->m_factor;
  float angle_y = -1.0 * dx / this->m_factor;

  DirectX::XMMATRIX look_at = DirectX::XMLoadFloat4x4(&(data.look_at_matrix));
  
  static const DirectX::XMVECTORF32 v = { 1.0f,  0.0f, 0.0f, 0.0f };
  DirectX::XMVECTOR camera_x_axis = DirectX::XMVector4Transform(v, XMMatrixTranspose(look_at));

  static const DirectX::XMVECTORF32 world_y_axis = { 0.0f,  1.0f, 0.0f, 0.0f };
  
  DirectX::XMMATRIX rotation =
    DirectX::XMMatrixRotationAxis(world_y_axis, angle_y) *
    DirectX::XMMatrixRotationAxis(camera_x_axis, angle_x);

  DirectX::XMVECTOR up =  DirectX::XMLoadFloat4(&(data.up));
  DirectX::XMVECTOR eye = DirectX::XMLoadFloat4(&(data.eye));
  DirectX::XMVECTOR center = DirectX::XMLoadFloat4(&(data.center));

  up =  DirectX::XMVector4Transform(up,  rotation);
  eye = DirectX::XMVector4Transform(eye, rotation);

  DirectX::XMVECTOR rot_center = DirectX::XMVector4Transform(center, rotation);

  DirectX::XMMATRIX translation =
    DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSubtract(rot_center,
                                                                     center));

  // update LookAt matrix
  look_at = translation * rotation * look_at;

  // update vectors


  DirectX::XMStoreFloat4x4(&data.look_at_matrix, look_at);
  DirectX::XMStoreFloat4(&(data.up),  up);
  DirectX::XMStoreFloat4(&(data.eye), eye);
}

}
}
}
