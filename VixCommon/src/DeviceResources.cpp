#include "pch.h"

#include "include\DeviceResources.h"
#include "include\DirectXHelper.h"
#include <windows.ui.xaml.media.dxinterop.h>

using namespace D2D1;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml::Controls;
using namespace Platform;


// Constants used to calculate screen rotations.
namespace ScreenRotation {
	// 0-degree Z-rotation
	static const XMFLOAT4X4 Rotation0( 
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);

	// 90-degree Z-rotation
	static const XMFLOAT4X4 Rotation90(
		0.0f, 1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);

	// 180-degree Z-rotation
	static const XMFLOAT4X4 Rotation180(
		-1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);

	// 270-degree Z-rotation
	static const XMFLOAT4X4 Rotation270( 
		0.0f, -1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
};


namespace VixCommon {

DeviceResources::DeviceResources(const DisplayMetrics& display_metrics) :
    m_display_metrics(display_metrics),
		m_screen_viewport(),
		m_d3d_feature_level(D3D_FEATURE_LEVEL_9_1),
		m_d3d_render_target_size(),
		m_output_size(),
		m_logical_size(),
		m_native_orientation(DisplayOrientations::None),
		m_current_orientation(DisplayOrientations::None),
		m_dpi(-1.0f),
		m_effective_dpi(-1.0f),
		m_composition_scale_x(1.0f),
		m_composition_scale_y(1.0f),
    m_device_notify(nullptr) {
  this->create2DResources();
  this->create3DResources();
}


void DeviceResources::create2DResources() {
  // initialise Direct2D resources
  D2D1_FACTORY_OPTIONS options;
  ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
  options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

  DX::ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
                                      __uuidof(ID2D1Factory3),
                                      &options,
                                      &(this->m_d2d_factory)));
  DX::ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
                                        __uuidof(IDWriteFactory3),
                                        &(this->m_dwrite_factory)));
  DX::ThrowIfFailed(CoCreateInstance(CLSID_WICImagingFactory2,
                                     nullptr,
                                     CLSCTX_INPROC_SERVER,
                                     IID_PPV_ARGS(&(this->m_wic_factory))));

}


void DeviceResources::create3DResources() {
  UINT creation_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
  if (DX::SdkLayersAvailable()) creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_12_1,
                                         D3D_FEATURE_LEVEL_12_0,
                                         D3D_FEATURE_LEVEL_11_1,
                                         D3D_FEATURE_LEVEL_11_0,
                                         D3D_FEATURE_LEVEL_10_1,
                                         D3D_FEATURE_LEVEL_10_0,
                                         D3D_FEATURE_LEVEL_9_3,
                                         D3D_FEATURE_LEVEL_9_2,
                                         D3D_FEATURE_LEVEL_9_1 };

  // create the Direct3D API device object and a corresponding context
  ComPtr<ID3D11Device> device;
  ComPtr<ID3D11DeviceContext> context;

	HRESULT hr = D3D11CreateDevice(nullptr,					// Specify nullptr to use the default adapter.
                                 D3D_DRIVER_TYPE_HARDWARE,	// Create a device using the hardware graphics driver.
                                 0,							// Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
                                 creation_flags,				// Set debug and Direct2D compatibility flags.
                                 feature_levels,				// List of feature levels this app can support.
                                 ARRAYSIZE(feature_levels),	// Size of the list above.
                                 D3D11_SDK_VERSION,			// Always set this to D3D11_SDK_VERSION for Windows Store apps.
                                 &device,					// Returns the Direct3D device created.
                                 &(this->m_d3d_feature_level),			// Returns feature level of device created.
                                 &context);					// Returns the device immediate context.

  if (FAILED(hr)) {
	DX::ThrowIfFailed(D3D11CreateDevice(nullptr,					// Specify nullptr to use the default adapter.
                                      D3D_DRIVER_TYPE_WARP,	// Create a device using the hardware graphics driver.
                                      0,							// Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
                                      creation_flags,				// Set debug and Direct2D compatibility flags.
                                      feature_levels,				// List of feature levels this app can support.
                                      ARRAYSIZE(feature_levels),	// Size of the list above.
                                      D3D11_SDK_VERSION,			// Always set this to D3D11_SDK_VERSION for Windows Store apps.
                                      &device,					// Returns the Direct3D device created.
                                      &(this->m_d3d_feature_level),			// Returns feature level of device created.
                                      &context));					// Returns the device immediate context.
  }

  // store pointers to the Direct3D 11.3 API device and immediate context
  DX::ThrowIfFailed(device.As(&(this->m_d3d_device)));
  DX::ThrowIfFailed(context.As(&(this->m_d3d_context)));

  ComPtr<IDXGIDevice3> dxgi_device;

  DX::ThrowIfFailed(this->m_d3d_device.As(&dxgi_device));
  DX::ThrowIfFailed(this->m_d2d_factory->CreateDevice(dxgi_device.Get(),
                                                      &(this->m_d2d_device)));
  DX::ThrowIfFailed(this->m_d2d_device->CreateDeviceContext(
    D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
    &(this->m_d2d_context)));
}


void DeviceResources::createWindowsResources() {
  // clear the previous window size specific context
  ID3D11RenderTargetView* null_views[] = { nullptr };
  this->m_d3d_context->OMSetRenderTargets(ARRAYSIZE(null_views), 
                                          null_views, 
                                          nullptr);
  this->m_d3d_render_target_view = nullptr;

  this->m_d2d_context->SetTarget(nullptr);
  this->m_d2d_target_bitmap = nullptr;
  this->m_d3d_depth_stencil_view = nullptr;
  this->m_d3d_context->Flush1(D3D11_CONTEXT_TYPE_ALL, nullptr);

  this->updateRenderTargetSize();

  // The width and height of the swap chain must be based on the window's
  // natively-oriented width and heigh. If the window is not in the native
  // orientation, the dimensions must be reversed.
  DXGI_MODE_ROTATION display_rotation = this->computeDisplayRotation();

  bool swap_dimensions = display_rotation == DXGI_MODE_ROTATION_ROTATE90 || 
                         display_rotation == DXGI_MODE_ROTATION_ROTATE270;
  this->m_d3d_render_target_size.Width =
    swap_dimensions ? this->m_output_size.Height : this->m_output_size.Width;
  this->m_d3d_render_target_size.Height = 
    swap_dimensions ? this->m_output_size.Width : this->m_output_size.Height;

  if (this->m_swap_chain != nullptr) {
    // if the swap chain already exists, resize it
    HRESULT hr = this->m_swap_chain->ResizeBuffers(
      2,
      lround(this->m_d3d_render_target_size.Width),
      lround(this->m_d3d_render_target_size.Height),
      DXGI_FORMAT_B8G8R8A8_UNORM,
      0);

    if (hr == DXGI_ERROR_DEVICE_REMOVED ||
        hr == DXGI_ERROR_DEVICE_RESET) {
      this->handleDeviceLost();
      return;
    } else {
      DX::ThrowIfFailed(hr);
    }
  }
  else {
    // otherwise, create a new one using the same adapter as the existing 
    // Direct3D device.
    DXGI_SCALING scaling = 
      this->m_display_metrics.m_support_high_resolutions ? DXGI_SCALING_NONE : 
                                                           DXGI_SCALING_STRETCH;
    DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = { 0 };

    swap_chain_desc.Width  = lround(this->m_d3d_render_target_size.Width);  // Match the size of the window.
    swap_chain_desc.Height = lround(this->m_d3d_render_target_size.Height); 
    swap_chain_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;                    // This is the most common swap chain format.
    swap_chain_desc.Stereo = false;

    swap_chain_desc.SampleDesc.Count = 1;   // Do not use multi-sampling
    swap_chain_desc.SampleDesc.Quality = 0;

    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.BufferCount = 2;   // Use double-buffering to minimize latency.
    
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All windows store apps must use _FLIP_ SwapEffects
    swap_chain_desc.Flags = 0;
    swap_chain_desc.Scaling = scaling;

    swap_chain_desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

    // This sequence obtains the DXGI factory that was used to create the 
    // Direct3D device above.
    ComPtr<IDXGIDevice3> dxgi_device;
    DX::ThrowIfFailed(this->m_d3d_device.As(&dxgi_device));

    ComPtr<IDXGIAdapter> dxgi_adapter;
    DX::ThrowIfFailed(dxgi_device->GetAdapter(&dxgi_adapter));

    ComPtr<IDXGIFactory4> dxgi_factory;
    DX::ThrowIfFailed(dxgi_adapter->GetParent(IID_PPV_ARGS(&dxgi_factory)));

    // When using XAML interop, the swap chain must be created for composition
    ComPtr<IDXGISwapChain1> swap_chain;
    DX::ThrowIfFailed(
      dxgi_factory->CreateSwapChainForComposition(this->m_d3d_device.Get(),
                                                  &swap_chain_desc,
                                                  nullptr,
                                                  &swap_chain));

    DX::ThrowIfFailed(swap_chain.As(&m_swap_chain));

    // Associate swap chain with SwapChainPanel
    // UI changes will need to be dispatched back to the UI thread
    this->m_swap_chain_panel->Dispatcher->RunAsync(CoreDispatcherPriority::High,
                                                   ref new DispatchedHandler([=]() {
			ComPtr<ISwapChainPanelNative> panel_native;
      DX::ThrowIfFailed(
        reinterpret_cast<IUnknown*>(this->m_swap_chain_panel)->QueryInterface(IID_PPV_ARGS(&panel_native)));
      DX::ThrowIfFailed(panel_native->SetSwapChain(this->m_swap_chain.Get()));
    }, CallbackContext::Any));

    // Ensure that DXGI does not queue more than one frame at a time. 
    // This both reduces latency and ensures that the application will
    // only render after each VSync, minimizing power consumption.
    DX::ThrowIfFailed(dxgi_device->SetMaximumFrameLatency(1));
  }

  // Set the proper orientation for the swap chain, and generate 2D and
  // 3D matrix transformations for rendering to the rotated swap chain.
  // Note the rotation angle for the 2D and 3D transforms are different.
  // This is due to the difference in coordinate spaces. Additionally, the
  // 3D matrix is specified explicitly to avoid rounding errors.

  switch (display_rotation) {
    case DXGI_MODE_ROTATION_IDENTITY:
      this->m_orientation_transform_2D = Matrix3x2F::Identity();
      this->m_orientation_transform_3D = ScreenRotation::Rotation0;
      break;

    case DXGI_MODE_ROTATION_ROTATE90:
      this->m_orientation_transform_2D = 
        Matrix3x2F::Rotation(90.0f) * 
        Matrix3x2F::Translation(this->m_logical_size.Height, 0.0f);
      this->m_orientation_transform_3D = ScreenRotation::Rotation270;
      break;

    case DXGI_MODE_ROTATION_ROTATE180:
      this->m_orientation_transform_2D = 
        Matrix3x2F::Rotation(180.0f) * 
        Matrix3x2F::Translation(this->m_logical_size.Width, 
                                this->m_logical_size.Height);
      this->m_orientation_transform_3D = ScreenRotation::Rotation180;
      break;

    case DXGI_MODE_ROTATION_ROTATE270:
      this->m_orientation_transform_2D = 
        Matrix3x2F::Rotation(270.0f) * 
        Matrix3x2F::Translation(0.0f, this->m_logical_size.Width);
      this->m_orientation_transform_3D = ScreenRotation::Rotation90;
      break;

    default:
      throw ref new FailureException();
  }

  DX::ThrowIfFailed(this->m_swap_chain->SetRotation(display_rotation));

  // Setup inverse scale on the swap chain
  DXGI_MATRIX_3X2_F inverse_scale = { 0 };
  inverse_scale._11 = 1.0f / this->m_effective_composition_scale_x;
  inverse_scale._22 = 1.0f / this->m_effective_composition_scale_y;
  
  ComPtr<IDXGISwapChain2> sp_swap_chain2;
  DX::ThrowIfFailed(this->m_swap_chain.As<IDXGISwapChain2>(&sp_swap_chain2));
  DX::ThrowIfFailed(sp_swap_chain2->SetMatrixTransform(&inverse_scale));

  // Create a render target view of the swap chain back buffer.
  ComPtr<ID3D11Texture2D1> back_buffer;
  DX::ThrowIfFailed(
    this->m_swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer)));
  DX::ThrowIfFailed(
    this->m_d3d_device->CreateRenderTargetView1(back_buffer.Get(),
                                                nullptr,
                                                &(this->m_d3d_render_target_view)));

  // Create a depth stencil view for use with 3D rendering if needed
  CD3D11_TEXTURE2D_DESC1 depth_stencil_desc(
    DXGI_FORMAT_D24_UNORM_S8_UINT,
    lround(this->m_d3d_render_target_size.Width),
    lround(this->m_d3d_render_target_size.Height),
    1, // This depth stencil view has only one texture
    1, // Use a single mipmap level
    D3D11_BIND_DEPTH_STENCIL);

  ComPtr<ID3D11Texture2D1> depth_stencil;
  DX::ThrowIfFailed(this->m_d3d_device->CreateTexture2D1(&depth_stencil_desc,
                                                         nullptr,
                                                         &depth_stencil));

  CD3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc(D3D11_DSV_DIMENSION_TEXTURE2D); 
  DX::ThrowIfFailed(this->m_d3d_device->CreateDepthStencilView(
    depth_stencil.Get(),
    &depth_stencil_view_desc,
    &(this->m_d3d_depth_stencil_view)));

  // Set the 3D rendering viewport to target the entire window
  this->m_screen_viewport = CD3D11_VIEWPORT(0.0f,
                                            0.0f,
                                            this->m_d3d_render_target_size.Width,
                                            this->m_d3d_render_target_size.Height);

  this->m_d3d_context->RSSetViewports(1, &(this->m_screen_viewport));

  // Create a Direct2D target bitmap associated with the swap chain back buffer
  // and set it as the current target
  D2D1_BITMAP_PROPERTIES1 bitmap_properties = 
    D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
                            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
                            this->m_dpi,
                            this->m_dpi);

  ComPtr<IDXGISurface2> dxgi_back_buffer;
  DX::ThrowIfFailed(this->m_swap_chain->GetBuffer(0, IID_PPV_ARGS(&dxgi_back_buffer)));
  DX::ThrowIfFailed(this->m_d2d_context->CreateBitmapFromDxgiSurface(
    dxgi_back_buffer.Get(),
    &bitmap_properties,
    &(this->m_d2d_target_bitmap)));

  this->m_d2d_context->SetTarget(this->m_d2d_target_bitmap.Get());
  this->m_d2d_context->SetDpi(this->m_effective_dpi, this->m_effective_dpi);

  // Grayscale text anti-aliasing is recommended for all Windows Store apps.
  this->m_d2d_context->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
}


void DeviceResources::updateRenderTargetSize() {
  this->m_effective_dpi = this->m_dpi;
  this->m_effective_composition_scale_x = this->m_composition_scale_x;
  this->m_effective_composition_scale_y = this->m_composition_scale_y;

  // To improve battery life on high resolution devices, render to a 
  // smaller render target and allow the GPU to scale the output when it
  // is presented.
  if (!this->m_display_metrics.m_support_high_resolutions &&
      this->m_dpi > this->m_display_metrics.m_dpi_threshold) {
    float width  = DX::ConvertDipsToPixels(this->m_logical_size.Width,  this->m_dpi);
    float height = DX::ConvertDipsToPixels(this->m_logical_size.Height, this->m_dpi);

    // When the device is in portrait orientation, height > width. Compare the
    // larger dimension against the width threshold and the smaller dimension 
    // against the height threshold.
    if (max(width, height) > this->m_display_metrics.m_width_threshold &&
        max(width, height) > this->m_display_metrics.m_height_threshold) {
      // To scale the app we change the effective DPI. Logical size does 
      // not change
      this->m_effective_dpi /= 2.0f;
      this->m_effective_composition_scale_x /= 2.0f;
      this->m_effective_composition_scale_y /= 2.0f;
    }
  }

  // Calculate the necessary render target size in pixels
  this->m_output_size.Width = DX::ConvertDipsToPixels(this->m_logical_size.Width,
                                                      this->m_effective_dpi);
  this->m_output_size.Height = DX::ConvertDipsToPixels(this->m_logical_size.Height,
                                                       this->m_effective_dpi);

  this->m_output_size.Width  = max(this->m_output_size.Width,  1);
  this->m_output_size.Height = max(this->m_output_size.Height, 1);
}


// ---------------------------------------------------------------------------
// Set methods
// ---------------------------------------------------------------------------
void DeviceResources::setSwapChainPanel(SwapChainPanel^ panel) {
  DisplayInformation^ current_display_information = 
    DisplayInformation::GetForCurrentView();

  this->m_swap_chain_panel = panel;
  this->m_logical_size = Windows::Foundation::Size(static_cast<float>(panel->ActualWidth),
                                                   static_cast<float>(panel->ActualHeight));
  this->m_native_orientation  = current_display_information->NativeOrientation;
  this->m_current_orientation = current_display_information->CurrentOrientation;

  this->m_composition_scale_x = panel->CompositionScaleX;
  this->m_composition_scale_y = panel->CompositionScaleY;
  this->m_dpi = current_display_information->LogicalDpi;

  this->m_d2d_context->SetDpi(this->m_dpi, this->m_dpi);

  this->createWindowsResources();
}


void DeviceResources::setLogicalSize(Windows::Foundation::Size logical_size) {
  if (this->m_logical_size != logical_size) {
    this->m_logical_size = logical_size;
    this->createWindowsResources();
  }
}


void DeviceResources::setDPI(float dpi) {
  if (this->m_dpi != dpi) {
    this->m_dpi = dpi;
    this->m_d2d_context->SetDpi(this->m_dpi, this->m_dpi);
    this->createWindowsResources();
  }
}


void DeviceResources::setCurrentOrientation(DisplayOrientations current_orientation) {
  if (this->m_current_orientation != current_orientation) {
    this->m_current_orientation = current_orientation;
    this->createWindowsResources();
  }
}


void DeviceResources::setCompositionScale(float composition_scale_x,
                                          float composition_scale_y) {
  if (this->m_composition_scale_x != composition_scale_x ||
      this->m_composition_scale_y != composition_scale_y) {
    this->m_composition_scale_x = composition_scale_x;
    this->m_composition_scale_y = composition_scale_y;
    this->createWindowsResources();
  }
}


// ---------------------------------------------------------------------------
// misc. methods
// ---------------------------------------------------------------------------
void DeviceResources::validateDevice() {
	// The D3D Device is no longer valid if the default adapter changed since 
  // the device was created or if the device has been removed.
	// First, get the information for the default adapter from when the device 
  // was created.

  ComPtr<IDXGIDevice3> dxgi_device;
  DX::ThrowIfFailed(this->m_d3d_device.As(&dxgi_device));

  ComPtr<IDXGIAdapter> device_adapter;
  DX::ThrowIfFailed(dxgi_device->GetAdapter(&device_adapter));

  ComPtr<IDXGIFactory2> device_factory;
  DX::ThrowIfFailed(device_adapter->GetParent(IID_PPV_ARGS(&device_factory)));

  ComPtr<IDXGIAdapter1> previous_default_adapter;
  DX::ThrowIfFailed(device_factory->EnumAdapters1(0, &previous_default_adapter));

  DXGI_ADAPTER_DESC1 previous_desc;
  DX::ThrowIfFailed(previous_default_adapter->GetDesc1(&previous_desc));

  // Next, get the information for the current default adapter.

  ComPtr<IDXGIFactory4> current_factory;
  DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&current_factory)));

  ComPtr<IDXGIAdapter1> current_default_adapter;
  DX::ThrowIfFailed(current_factory->EnumAdapters1(0, &current_default_adapter));

  DXGI_ADAPTER_DESC1 current_desc;
  DX::ThrowIfFailed(current_default_adapter->GetDesc1(&current_desc));

  // If the adapter LUIDs do not match, or if the device reports that it has
  // been removed a new D3D device must be created
  if (previous_desc.AdapterLuid.LowPart != current_desc.AdapterLuid.LowPart   ||
      previous_desc.AdapterLuid.HighPart != current_desc.AdapterLuid.HighPart ||
      FAILED(this->m_d3d_device->GetDeviceRemovedReason())) {
    // release references to resources related to the old device
    dxgi_device = nullptr;
    device_adapter = nullptr;
    device_factory = nullptr;
    previous_default_adapter = nullptr;

    // Create a new device and swap chain
    this->handleDeviceLost();
  }
}


// Recreate all device resources and set them back to the current state
void DeviceResources::handleDeviceLost() {
  this->m_swap_chain = nullptr;

  if (this->m_device_notify != nullptr) this->m_device_notify->onDeviceLost();

  this->create3DResources();
  this->m_d2d_context->SetDpi(this->m_dpi, this->m_dpi);
  this->createWindowsResources();

  if (this->m_device_notify != nullptr) this->m_device_notify->onDeviceRestored();
}


// Register our DeviceNotify to be informed on device lost and creation
void DeviceResources::registerDeviceNotify(IDeviceNotify* device_notify) {
  this->m_device_notify = device_notify;
}


// Call this method when the app suspends. It provides a hint to the driver 
// that the app is entering an idle state and that temporary buffers can be 
// reclaimed for use by other apps.
void DeviceResources::trim() {
  ComPtr<IDXGIDevice3> dxgi_device;
  this->m_d3d_device.As(&dxgi_device);

  dxgi_device->Trim();
}


void DeviceResources::present() {
  // The first argument instructs DXGI to block until VSYync, putting the
  // application to sleep until the next VSync. This ensures we do not waste
  // any cycles rendering frames that will never be displayed to the screen.
  DXGI_PRESENT_PARAMETERS parameters = { 0 };
  HRESULT hr = this->m_swap_chain->Present1(1, 0, &parameters);

  // Discard the contents of the render target and depth stencil. 
  // This is a valid operation only when the existing contents will be entirely
  // overwritten. If dirty or scroll rects are used this call should be modified.
  this->m_d3d_context->DiscardView1(this->m_d3d_render_target_view.Get(),
                                    nullptr, 
                                    0);
  this->m_d3d_context->DiscardView1(this->m_d3d_depth_stencil_view.Get(),
                                    nullptr, 
                                    0);

  // If the device was removed either by a disconnection or a driver upgrade,
  // we must recreate all device resources
  if (hr == DXGI_ERROR_DEVICE_REMOVED ||
      hr == DXGI_ERROR_DEVICE_RESET) {
    this->handleDeviceLost();
  }
  else {
    DX::ThrowIfFailed(hr);
  }
}


DXGI_MODE_ROTATION DeviceResources::computeDisplayRotation() {
  DXGI_MODE_ROTATION rotation = DXGI_MODE_ROTATION_UNSPECIFIED;

  // Note: NativeORientation can only be Landscaped or Portrait
  // even though the DisplayOrientations enum has other values.
  switch (this->m_native_orientation) {
    // -------------------------------------------
    case DisplayOrientations::Landscape:
      switch (this->m_current_orientation) {
        case DisplayOrientations::Landscape:
          rotation = DXGI_MODE_ROTATION_IDENTITY;
          break;

        case DisplayOrientations::Portrait:
          rotation = DXGI_MODE_ROTATION_ROTATE270;
          break;

        case DisplayOrientations::LandscapeFlipped:
          rotation = DXGI_MODE_ROTATION_ROTATE180;
          break;

        case DisplayOrientations::PortraitFlipped:
          rotation = DXGI_MODE_ROTATION_ROTATE90;
          break;
      } 
      break;

    // -------------------------------------------
    case DisplayOrientations::Portrait:
      switch (this->m_current_orientation) {
        case DisplayOrientations::Landscape:
          rotation = DXGI_MODE_ROTATION_ROTATE90;
          break;

        case DisplayOrientations::Portrait:
          rotation = DXGI_MODE_ROTATION_IDENTITY;
          break;

        case DisplayOrientations::LandscapeFlipped:
          rotation = DXGI_MODE_ROTATION_ROTATE270;
          break;

        case DisplayOrientations::PortraitFlipped:
          rotation = DXGI_MODE_ROTATION_ROTATE180;
          break;
      } 
      break;
  }
  return rotation;
}

} // VixCommon