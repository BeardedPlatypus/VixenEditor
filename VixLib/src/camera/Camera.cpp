#include "pch.h"
#include "include\camera\Camera.h"

using namespace DirectX;

namespace VixLib {
namespace camera {

Camera::Camera(std::unique_ptr<transform::ITranslate> f_translate,
               std::unique_ptr<transform::IRotate>    f_rotate,
               std::unique_ptr<transform::IZoom>      f_zoom,
               XMFLOAT4 eye,
               XMFLOAT4 center,
               XMFLOAT4 up,
               float fov_angle_y,
               float aspect_ratio,
               float z_near,
               float z_far,
               DirectX::XMFLOAT4X4 orientation) :
  m_translate(),
  m_rotate(),
  m_zoom(),
  m_data(CameraData(eye, center, up, fov_angle_y, aspect_ratio, z_near, z_far, orientation)) {
  this->m_translate = std::move(f_translate);
  this->m_rotate    = std::move(f_rotate);
  this->m_zoom      = std::move(f_zoom);
}


Camera::~Camera() {
}


// ---------------------------------------------------------------------------
// transforms
// ---------------------------------------------------------------------------
void Camera::translate(float dx, float dy) {
  this->m_translate->translate(this->m_data, dx, dy);
}


void Camera::rotate(float dx, float dy) {
  this->m_rotate->rotate(this->m_data, dx, dy);
}


void Camera::zoom(float d) {
  this->m_zoom->zoom(this->m_data, d);
}


void Camera::snapToAxis(Axis axis, Direction dir) {
  XMVECTOR center = DirectX::XMLoadFloat4(&(this->m_data.center));
  XMVECTOR eye;
  XMVECTOR up;

  static const XMVECTORF32 up_xz = { 0.f, 1.f, 0.f, 0.f };
  static const XMVECTORF32 up_y = { 0.f, 0.f, 1.f, 0.f };

  XMVECTORF32 offset;

  float factor;
  if (dir == Direction::POS) factor = -1.0f;
  else                       factor = +1.0f;

  switch (axis) {
  case Axis::X:
    up = up_xz;
    offset = { factor * this->m_data.zoom, 0.f, 0.f, 0.f };
    break;
  case Axis::Y:
    up = up_y;
    offset = { 0.0f, factor * this->m_data.zoom, 0.f, 0.f };
    break;
  case Axis::Z:
    up = up_xz;
    offset = { 0.0f, 0.0f, factor * this->m_data.zoom, 0.f };
    break;
  }

  eye = center - offset;
  XMStoreFloat4x4(&(this->m_data.look_at_matrix),
                  XMMatrixTranspose(XMMatrixLookAtRH(eye, center, up)));
  XMStoreFloat4(&(this->m_data.eye), eye);
  XMStoreFloat4(&(this->m_data.up), up);
}


void Camera::setFoVAngleY(float fov_angle_y) {
  this->m_data.fov_angle_y = fov_angle_y;
  this->m_data.recalculatePerspectiveMatrix();
}


void Camera::setAspectRatio(float aspect_ratio) {
  this->m_data.aspect_ratio = aspect_ratio;
  this->m_data.recalculatePerspectiveMatrix();
}


void Camera::setZNear(float z_near) {
  this->m_data.z_near = z_near;
  this->m_data.recalculatePerspectiveMatrix();
}


void Camera::setZFar(float z_far) {
  this->m_data.z_far= z_far;
  this->m_data.recalculatePerspectiveMatrix();
}


void Camera::setZ(float z_near, float z_far) {
  this->m_data.z_near = z_near;
  this->m_data.z_far= z_far;
  this->m_data.recalculatePerspectiveMatrix();
}


void Camera::setOrientation(XMFLOAT4X4 orientation) {
  this->m_data.orientation_matrix = orientation;

	XMMATRIX perspective_matrix = XMLoadFloat4x4(&(this->m_data.perspective_matrix));
	XMMATRIX orientation_matrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(&(this->m_data.projection_matrix),
                  XMMatrixTranspose(perspective_matrix * orientation_matrix));
}

}
}