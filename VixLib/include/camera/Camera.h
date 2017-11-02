#pragma once

#include <memory>

#include "CameraData.h"

#include "transforms\Translate.h"
#include "transforms\Rotate.h"
#include "transforms\Zoom.h"


namespace VixLib {
namespace camera {

enum class Axis { X, Y, Z };
enum class Direction { POS, NEG };


class Camera {
public:
  // -------------------------------------------------------------------------
  // Constructor | Destructor
  // -------------------------------------------------------------------------
  Camera(std::unique_ptr<transform::ITranslate> f_translate,
         std::unique_ptr<transform::IRotate>    f_rotate,
         std::unique_ptr<transform::IZoom>      f_zoom,
         DirectX::XMFLOAT4 eye,
         DirectX::XMFLOAT4 center,
         DirectX::XMFLOAT4 up,
         float fov_angle_y,
         float aspect_ratio,
         float z_near,
         float z_far,
         DirectX::XMFLOAT4X4 orientation);
  ~Camera();

  // -------------------------------------------------------------------------
  // Getter methods
  // -------------------------------------------------------------------------
  const CameraData& getCameraDataConst() const { return this->m_data; }

  // -------------------------------------------------------------------------
  // Modification functions
  // -------------------------------------------------------------------------
  void translate(float dx, float dy);
  void rotate(float dx, float dy);
  void zoom(float d);

  void snapToAxis(Axis axis, Direction dir);

  void setFoVAngleY(float fov_angle_y);
  void setAspectRatio(float aspect_ratio);
  void setZNear(float z_near);
  void setZFar(float z_far);
  void setZ(float z_near, float z_far);
  void setOrientation(DirectX::XMFLOAT4X4 orientation);

private:
  // -------------------------------------------------------------------------
  // Transform functions
  // -------------------------------------------------------------------------
  std::unique_ptr<transform::ITranslate> m_translate;
  std::unique_ptr<transform::IRotate>    m_rotate;
  std::unique_ptr<transform::IZoom>      m_zoom;

  // -------------------------------------------------------------------------
  // Camera data
  // -------------------------------------------------------------------------
  CameraData m_data;
};

} // camera
} // VixLib
