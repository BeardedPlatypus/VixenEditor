#pragma once

#include <memory>

#include "Content\UserInputInterpreter.h"

#include "VixCommon\include\DeviceResources.h"

#include "VixLib\include\camera\Camera.h"
#include "VixLib\include\renderer\Renderer.h"

namespace VixenEditor {

/// <summary>
/// The SystemWarden class is responsible for creating and deleting all of 
/// the subsystems of the VixenEditor application. It ensures that each of
/// the subsystems are created and deleted in the right order. And owns 
/// each of the instances.
/// </summary>
class SystemWarden {
public:
  SystemWarden(std::shared_ptr<VixCommon::DeviceResources> device_resources);

  UserInputInterpreter& getInputInterpreter() { return *(this->p_input_interpreter.get()); }
  VixLib::graphics::Renderer& getRenderer() { return *(this->p_renderer.get()); }

private:
  std::unique_ptr<VixLib::camera::Camera> p_camera;
  std::unique_ptr<UserInputInterpreter> p_input_interpreter;
  std::unique_ptr<VixLib::graphics::Renderer> p_renderer;

  std::shared_ptr<VixCommon::DeviceResources> m_device_resources;
};

}
