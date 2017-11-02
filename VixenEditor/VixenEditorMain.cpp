#include "pch.h"
#include "VixenEditorMain.h"
#include "VixCommon\include\DirectXHelper.h"

using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

namespace VixenEditor {
// Loads and initializes application assets when the application is loaded.

// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
VixenEditorMain::VixenEditorMain(const std::shared_ptr<VixCommon::DeviceResources>& device_resources) :
  m_device_resources(device_resources),
  system_warden(SystemWarden(device_resources)) { //, 
  //m_pointer_location_x(0.0f) {
  // Register to be notified if the Device is lost or recreated
  this->m_device_resources->registerDeviceNotify(this);

  // TODO: Replace this with your app's content initialization.
  //this->m_scene_renderer = std::unique_ptr<Sample3DSceneRenderer>(new Sample3DSceneRenderer(this->m_device_resources));
  //this->m_fps_text_renderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(this->m_device_resources));

  // TODO: Change the timer settings if you want something other than the default variable timestep mode.
  // e.g. for 60 FPS fixed timestep update logic, call:
  /*
  m_timer.SetFixedTimeStep(true);
  m_timer.SetTargetElapsedSeconds(1.0 / 60);
  */
}


// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
VixenEditorMain::~VixenEditorMain() {
  // Deregister device notification
  this->m_device_resources->registerDeviceNotify(nullptr);
}


// ----------------------------------------------------------------------------
// Init methods
// ----------------------------------------------------------------------------
// Updates application state when the window size changes (e.g. device orientation change)
void VixenEditorMain::createWindowSizeDependentResources() {
  // TODO: Replace this with the size-dependent initialization of your app's content.
  //this->m_scene_renderer->createWindowSizeDependentResources();
}


// ----------------------------------------------------------------------------
// render loop methods
// ----------------------------------------------------------------------------
void VixenEditorMain::startRenderLoop() {
  // If the animation render loop is already running then do not start another thread.
  if (this->m_render_loop_worker != nullptr && 
      this->m_render_loop_worker->Status == AsyncStatus::Started) {
    return;
  }

  // Create a task that will be run on a background thread.
  auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction ^ action) {
    // Calculate the updated frame and render once per vertical blanking interval.
    while (action->Status == AsyncStatus::Started) {
      critical_section::scoped_lock lock(this->m_critical_section);
      this->update();
      if (this->render()) {
        this->m_device_resources->present();
      }
    }
  });

  // Run task on a dedicated high priority background thread.
  this->m_render_loop_worker = ThreadPool::RunAsync(workItemHandler, 
                                                    WorkItemPriority::High, 
                                                    WorkItemOptions::TimeSliced);
}


void VixenEditorMain::stopRenderLoop() {
  this->m_render_loop_worker->Cancel();
}


// Updates the application state once per frame.
void VixenEditorMain::update() {
  //this->processInput();

  // Update scene objects.
  this->m_timer.Tick([&]() {
    // TODO: Replace this with your app's content update functions.
    //m_scene_renderer->update(m_timer);
    //m_fps_text_renderer->update(m_timer);
  });
}


// Process all input from the user before updating game state

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool VixenEditorMain::render() {
  // Don't try to render anything before the first Update.
  if (this->m_timer.GetFrameCount() == 0) {
    return false;
  }

  auto context = this->m_device_resources->getD3DDeviceContext();

  // Reset the viewport to target the whole screen.
  auto viewport = m_device_resources->getScreenViewport();
  context->RSSetViewports(1, &viewport);

  // Reset render targets to the screen.
  ID3D11RenderTargetView *const targets[1] = { m_device_resources->getBackBufferRenderTargetView() };
  context->OMSetRenderTargets(1, targets, m_device_resources->getDepthStencilView());

  // Clear the back buffer and depth stencil view.
  context->ClearRenderTargetView(m_device_resources->getBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
  context->ClearDepthStencilView(m_device_resources->getDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

  // Render the scene objects.
  // TODO: Replace this with your app's content rendering functions.
  this->system_warden.getRenderer().render();
  //this->m_fps_text_renderer->render();

  return true;
}

// Notifies renderers that device resources need to be released.
void VixenEditorMain::onDeviceLost()
{
  this->system_warden.getRenderer().releaseDeviceDependentResources();
  //this->m_fps_text_renderer->releaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void VixenEditorMain::onDeviceRestored() {
  this->system_warden.getRenderer().createDeviceDependentResources();
  //this->m_fps_text_renderer->createDeviceDependentResources();
  this->createWindowSizeDependentResources();
}

} // VixenEditor