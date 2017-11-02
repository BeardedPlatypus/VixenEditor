#pragma once

#include <string>

#include "VixCommon\include\DeviceResources.h"
#include "VixCommon\include\StepTimer.h"

namespace VixenEditor {
// Renders the current FPS value in the bottom right corner of the screen using Direct2D and DirectWrite.
class SampleFpsTextRenderer {
public:
  SampleFpsTextRenderer(const std::shared_ptr<VixCommon::DeviceResources>& device_resources);
  
  void createDeviceDependentResources();
  void releaseDeviceDependentResources();
  void update(DX::StepTimer const& timer);
  void render();

private:
  // Cached pointer to device resources.
  std::shared_ptr<VixCommon::DeviceResources> m_device_resources;
  
  // Resources related to text rendering.
  std::wstring                                    m_text;
  DWRITE_TEXT_METRICS	                            m_text_metrics;
  Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_white_brush;
  Microsoft::WRL::ComPtr<ID2D1DrawingStateBlock1> m_state_block;
  Microsoft::WRL::ComPtr<IDWriteTextLayout3>      m_text_layout;
  Microsoft::WRL::ComPtr<IDWriteTextFormat2>      m_text_format;
};
} // VixenEditor