#include "pch.h"
#include <include\camera\CameraData.h>

using namespace DirectX;

namespace VixLib {
namespace camera {

CameraData::CameraData(XMFLOAT4 eye,
                       XMFLOAT4 center,
                       XMFLOAT4 up,
                       float fov_angle_y,
                       float aspect_ratio,
                       float z_near,
                       float z_far,
                       DirectX::XMFLOAT4X4 orientation) :
    look_at_matrix(),
    eye(eye),
    center(center),
    up(up),
    zoom(),
    fov_angle_y(fov_angle_y),
    aspect_ratio(aspect_ratio),
    z_near(z_near),
    z_far(z_far),
    orientation_matrix(orientation) {
  XMVECTOR v_eye =    XMLoadFloat4(&this->eye);
  XMVECTOR v_center = XMLoadFloat4(&this->center);
  XMVECTOR v_up =     XMLoadFloat4(&this->up);

  XMStoreFloat4x4(&(this->look_at_matrix),
                  XMMatrixTranspose(XMMatrixLookAtRH(v_eye, v_center, v_up)));

  XMStoreFloat(&(this->zoom), XMVector4Length(v_eye - v_center));

  this->recalculatePerspectiveMatrix();
}


void CameraData::recalculatePerspectiveMatrix() {
  DirectX::XMMATRIX perspective = DirectX::XMMatrixPerspectiveFovRH(this->fov_angle_y,
                                                                    this->aspect_ratio,
                                                                    this->z_near,
                                                                    this->z_far);

	DirectX::XMMATRIX orientation = XMLoadFloat4x4(&(this->orientation_matrix));

  DirectX::XMStoreFloat4x4(&(this->perspective_matrix), perspective);
  DirectX::XMStoreFloat4x4(&(this->projection_matrix),
                           DirectX::XMMatrixTranspose(perspective * orientation));
}

}
}