#include "pch.h"
#include "include\camera\transforms\Zoom.h"

using namespace DirectX;

namespace VixLib {
namespace camera {
namespace transform {


Zoom::Zoom(float factor) : m_factor(factor) {
}


// TODO Make this scale properly with distance
void Zoom::zoom(CameraData& data, float d) const {
  float factor = -d / this->m_factor;  // data.zoom / this->m_factor;

  //data.look_at_matrix._44 += (data.look_at_matrix._44 * d) / this->m_factor;
  XMMATRIX look_at = XMLoadFloat4x4(&(data.look_at_matrix));

  XMVECTOR eye    = XMLoadFloat4(&data.eye);
  XMVECTOR center = XMLoadFloat4(&data.center);

  static const XMVECTORF32 i_z = { 0.0f, 0.0f, 1.0f, 0.0f };

  XMVECTOR camera_z_axis = XMVector4Normalize(XMVector4Transform(i_z, XMMatrixTranspose(look_at)));
  XMVECTOR z_translation = XMVectorScale(camera_z_axis, factor);
  XMMATRIX translation = XMMatrixTranslationFromVector(z_translation);

  data.zoom += factor;
  look_at = translation * look_at;
 
  eye    = XMVector4Transform(eye,    translation);
  center = XMVector4Transform(center, translation);

  XMStoreFloat4x4(&data.look_at_matrix, look_at);
  XMStoreFloat4(&(data.eye), eye);
  XMStoreFloat4(&(data.center), center);
}

}
}
}