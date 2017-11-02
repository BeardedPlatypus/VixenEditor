#include "pch.h"
#include "include\camera\transforms\Translate.h"


using namespace DirectX;

namespace VixLib {
namespace camera {
namespace transform {

Translate::Translate(float factor) : m_factor(factor) {
}


void Translate::translate(CameraData& data, float dx, float dy) const {
  float factor_x = dx / this->m_factor;
  float factor_y = -dy / this->m_factor;

  XMMATRIX look_at = XMLoadFloat4x4(&(data.look_at_matrix));
  XMVECTOR eye    = XMLoadFloat4(&data.eye);
  XMVECTOR center = XMLoadFloat4(&data.center);

  // Construct directions.
  static const XMVECTORF32 i_x = { 1.0f, 0.0f, 0.0f, 0.0f };
  XMVECTOR camera_x_axis = XMVector4Normalize(XMVector4Transform(i_x, look_at));
  XMVECTOR x_translation = XMVectorScale(camera_x_axis, factor_x);

  static const XMVECTORF32 i_y = { 0.0f, 1.0f, 0.0f, 0.0f };
  XMVECTOR camera_y_axis = XMVector4Normalize(XMVector4Transform(i_y, look_at));
  XMVECTOR y_translation = XMVectorScale(camera_y_axis, factor_y);

  XMMATRIX translation = XMMatrixTranspose(XMMatrixTranslationFromVector(x_translation) *
                                           XMMatrixTranslationFromVector(y_translation));

  // Translate lookat matrix
  look_at *= translation;
  eye    = XMVector4Transform(eye,    translation);
  center = XMVector4Transform(center, translation);

  XMStoreFloat4x4(&data.look_at_matrix, look_at);
  XMStoreFloat4(&(data.eye), eye);
  XMStoreFloat4(&(data.center), center);
}

}
}
}