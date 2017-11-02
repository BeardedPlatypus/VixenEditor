#pragma once

#include "VixCommon\include\StepTimer.h"
#include "VixCommon\include\DeviceResources.h"

#include "Content\SystemWarden.h"

//#include "Content\Sample3DSceneRenderer.h"
//#include "Content\SampleFpsTextRenderer.h"

// Renders Direct2D and 3D content on the screen.
namespace VixenEditor {
class VixenEditorMain : public VixCommon::IDeviceNotify {
public:
  VixenEditorMain(const std::shared_ptr<VixCommon::DeviceResources>& device_resources);
  ~VixenEditorMain();

  void createWindowSizeDependentResources();

  //void startTracking() { this->m_scene_renderer->startTracking(); }
  //void trackingUpdate(float position_x) { this->m_pointer_location_x = position_x; }
  //void stopTracking() { this->m_scene_renderer->stopTracking(); }
  //bool isTracking()   { return m_scene_renderer->isTracking(); }

  void startRenderLoop();
  void stopRenderLoop();

  Concurrency::critical_section& getCriticalSection() { return this->m_critical_section; }
  
  // IDeviceNotify
  virtual void onDeviceLost();
  virtual void onDeviceRestored();

  SystemWarden system_warden;

private:
  //void processInput();
  void update();
  bool render();
  
  // Cached pointer to device resources.
  std::shared_ptr<VixCommon::DeviceResources> m_device_resources;
  
  Windows::Foundation::IAsyncAction^ m_render_loop_worker;
  Concurrency::critical_section m_critical_section;
  
  // Rendering loop timer.
  DX::StepTimer m_timer;
};
}