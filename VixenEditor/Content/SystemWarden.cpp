#include "pch.h"
#include "Content\SystemWarden.h"

#include "VixLib\include\camera\transforms\Rotate.h"
#include "VixLib\include\camera\transforms\Translate.h"
#include "VixLib\include\camera\transforms\Zoom.h"

using namespace Windows::Foundation;

namespace VixenEditor {

SystemWarden::SystemWarden(std::shared_ptr<VixCommon::DeviceResources> device_resources) :
  m_device_resources(device_resources) {
  // Construct Camera
  Size output_size = this->m_device_resources->getOutputSize();

  float aspect_ratio = 1.288317256167; // 1920.0f / 1080.0f; // output_size.Width / output_size.Height;
  float fov_angle_y = 70.0f * DirectX::XM_PI / 180.0f;

  if (aspect_ratio < 1.0f) fov_angle_y *= 2.0f;

  DirectX::XMFLOAT4 eye = { 0.0f,  0.0f,  -10.0f, 0.0f };
  DirectX::XMFLOAT4 at  = { 0.0f, -0.5f,  0.0f, 0.0f };
  DirectX::XMFLOAT4 up  = { 0.0f,  1.0f,  0.0f, 0.0f };

  this->p_camera = std::make_unique<VixLib::camera::Camera>(
    std::make_unique<VixLib::camera::transform::Translate>(280.0f),
    std::make_unique<VixLib::camera::transform::TurnTableRotate>(280.0f),
    std::make_unique<VixLib::camera::transform::Zoom>(280.0f),
    eye, at, up,
    fov_angle_y,
    aspect_ratio,
    0.01f,
    100.f,
    this->m_device_resources->getOrientationTransform3D());

  // Construct UserInputInterpreter
  this->p_input_interpreter = std::make_unique<UserInputInterpreter>(*(this->p_camera.get()));

  // Construct Renderer
  this->p_renderer = std::make_unique<VixLib::graphics::Renderer>(this->m_device_resources,
                                                                  this->p_camera->getCameraDataConst());
}

}
