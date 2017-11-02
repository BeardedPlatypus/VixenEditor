#pragma once

#include "pch.h"

namespace VixenEditor {

class AppResources {
public:
  // -------------------------------------------------------------------------
  //  Constructor
  AppResources();

  // -------------------------------------------------------------------------
  //  Getter functions

  // -------------------------------------------------------------------------
  //  Setter functions
  void setSwapChainPanel(Windows::UI::Xaml::Controls::SwapChainPanel^ panel);
  void setLogicalSize(Windows::Foundation::Size logical_size);
  void setCurrentOrientation(Windows::Graphics::Display::DisplayOrientations current_orientation);
  void setDPI(float dpi);
  void setCompositionScale(float composition_scale_x, float composition_scale_y);

private:
  Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swap_chain;

  Windows::Foundation::Size m_d3d_render_target_size;
  Windows::Foundation::Size m_output_size;
  Windows::Foundation::Size m_logical_size;

  Windows::Graphics::Display::DisplayOrientations m_native_orientation;
  Windows::Graphics::Display::DisplayOrientations m_current_orientation;

  float m_dpi;
  float m_composition_scale_x;
  float m_composition_scale_y;

  float m_effective_dpi;
  float m_effective_composition_scale_x;
  float m_effective_composition_scale_y;

  D2D1::Matrix3x2F m_orientation_transform_2D;
  DirectX::XMFLOAT4X4 m_orientation_transform_3D;
};

}
