#pragma once

#include "pch.h"
#include "DirectXHelper.h"
#include "DisplayMetrics.h"

namespace VixCommon {

interface IDeviceNotify {
  virtual void onDeviceLost() = 0;
  virtual void onDeviceRestored() = 0;
};


class DeviceResources {
public:
  // -------------------------------------------------------------------------
  // Constructor
  // -------------------------------------------------------------------------
  /*! @brief Construct new DeviceResources
   */
  DeviceResources(const DisplayMetrics& display_metrics);

  // -------------------------------------------------------------------------
  // Getters
  // -------------------------------------------------------------------------
  /*! @brief Get a pointer to the d3d device of this DeviceResources
   *
   * @returns The d3d device of this DeviceResources
   */
  ID3D11Device3* getD3DDevice() const { return this->m_d3d_device.Get(); }

  /*! @brief Get a pointer to the d3d device context of this DeviceResources
   *
   * @returns The d3d3 device context of this DeviceResources
  */
  ID3D11DeviceContext3* getD3DDeviceContext() const { return this->m_d3d_context.Get(); }

  IDXGISwapChain3* getSwapChain() const { return this->m_swap_chain.Get(); }

  ID3D11RenderTargetView1* getBackBufferRenderTargetView() const { return this->m_d3d_render_target_view.Get(); }
  ID3D11DepthStencilView* getDepthStencilView() const { return this->m_d3d_depth_stencil_view.Get(); }

  D3D_FEATURE_LEVEL getDeviceFeatureLevel() const { return this->m_d3d_feature_level; }
  D3D11_VIEWPORT getScreenViewport() const { return this->m_screen_viewport; }

  // -------------------------------------------------------------------------
  ID2D1Factory3* getD2DFactory() const { return this->m_d2d_factory.Get(); }
  ID2D1Device2* getD2DDevice() const { return this->m_d2d_device.Get(); }
  ID2D1DeviceContext2* getD2DDeviceContext() const { return this->m_d2d_context.Get(); }
  ID2D1Bitmap1* getD2DTargetBitmap() const { return this->m_d2d_target_bitmap.Get(); }

  IDWriteFactory3* getDWriteFactory() const { return this->m_dwrite_factory.Get(); }
  IWICImagingFactory2* getWicImagingFactory() const { return this->m_wic_factory.Get(); }

  // -------------------------------------------------------------------------
  DirectX::XMFLOAT4X4 getOrientationTransform3D() const { return this->m_orientation_transform_3D; }
  D2D1::Matrix3x2F getOrientationTransform2D() const { return this->m_orientation_transform_2D; }

  Windows::Foundation::Size getLogicalSize() const { return this->m_logical_size; }
  Windows::Foundation::Size getOutputSize() const { return this->m_output_size; }

  // -------------------------------------------------------------------------
  // Setters
  // -------------------------------------------------------------------------
  /*! @brief Set the swap chain panel of this DeviceResources to panel
   */
  void setSwapChainPanel(Windows::UI::Xaml::Controls::SwapChainPanel^ panel);

  /*! @brief Set the logical size of this DeviceResources to logical_size
   */
  void setLogicalSize(Windows::Foundation::Size logical_size);

  /*! @brief Set the current orientation of this DeviceResources to 
   *         current_orientation.
   */

  void setCurrentOrientation(Windows::Graphics::Display::DisplayOrientations current_orientation);
  /*! @brief Set the DPI of this app to dpi
   */
  void setDPI(float dpi);

  /*! @brief Set the composition scale of this DeviceResources 
   */
  void setCompositionScale(float composition_scale_x, float composition_scale_y);
  // -------------------------------------------------------------------------
  // Interaction Methods
  /*! @brief Register device_notify as the interface for notification
   * 
   * @param device_notify The interface to be used when notifying about the 
   *        device
   */
  void registerDeviceNotify(IDeviceNotify* device_notify);

  /*! @brief Validate if the current device is correct, restore if it is not
   */
  void validateDevice();

  /*! @brief Handle the situation where the device is lost
   */
  void handleDeviceLost();

  /*! @brief Notify the driver that the app is in idle state.
   */
  void trim();

  /*! @brief Present the contents of the swapchain to the screen.
   */
  void present();


private:
  // -------------------------------------------------------------------------
  // creation methods
  void create2DResources();
  void create3DResources();

  void createWindowsResources();
  void updateRenderTargetSize();

  DXGI_MODE_ROTATION computeDisplayRotation();

  // -------------------------------------------------------------------------
  // fields

  // App Screen settings
  const DisplayMetrics& m_display_metrics;

  // Direct3D objects
  Microsoft::WRL::ComPtr<ID3D11Device3> m_d3d_device;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext3> m_d3d_context;
  Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swap_chain;

  // Direct3D rendering objects
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView1> m_d3d_render_target_view;
  Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_d3d_depth_stencil_view;
  D3D11_VIEWPORT m_screen_viewport;
	D3D_FEATURE_LEVEL	m_d3d_feature_level;

  // Direct2D drawing components
  Microsoft::WRL::ComPtr<ID2D1Factory3> m_d2d_factory;
  Microsoft::WRL::ComPtr<ID2D1Device2>  m_d2d_device;
  Microsoft::WRL::ComPtr<ID2D1DeviceContext2> m_d2d_context;
  Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_d2d_target_bitmap;

  // DirectWrite drawing components
  Microsoft::WRL::ComPtr<IDWriteFactory3> m_dwrite_factory;
  Microsoft::WRL::ComPtr<IWICImagingFactory2> m_wic_factory;

  // Cached reference to XAML panel
  Windows::UI::Xaml::Controls::SwapChainPanel^ m_swap_chain_panel;

  // Notifier
  IDeviceNotify* m_device_notify;

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

} // VixCommon
