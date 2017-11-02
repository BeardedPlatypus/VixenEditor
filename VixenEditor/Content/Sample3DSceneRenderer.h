#pragma once

#include "VixCommon\include\DeviceResources.h"
#include "ShaderStructures.h"
#include "VixCommon\include\StepTimer.h"

namespace VixenEditor {
// This sample renderer instantiates a basic rendering pipeline.
class Sample3DSceneRenderer {
public:
  Sample3DSceneRenderer(const std::shared_ptr<VixCommon::DeviceResources>& device_resources);
  
  void createDeviceDependentResources();
  void createWindowSizeDependentResources();
  void releaseDeviceDependentResources();
  void update(DX::StepTimer const& timer);
  void render();
  void startTracking();
  void trackingUpdate(float positionX);
  void stopTracking();
  bool isTracking() { return this->m_tracking; }

private:
  void rotate(float radians);

  // Cached pointer to device resources.
  std::shared_ptr<VixCommon::DeviceResources> m_device_resources;

  // Direct3D resources for cube geometry.
  Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_input_layout;
  Microsoft::WRL::ComPtr<ID3D11Buffer>		m_vertex_buffer;
  Microsoft::WRL::ComPtr<ID3D11Buffer>		m_index_buffer;
  Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertex_shader;
  Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pixel_shader;
  Microsoft::WRL::ComPtr<ID3D11Buffer>		m_constant_buffer;

  // System resources for cube geometry.
  ModelViewProjectionConstantBuffer	m_constant_buffer_data;
  uint32	m_index_count;

  // Variables used with the rendering loop.
  bool	m_loading_complete;
  float	m_degrees_per_second;
  bool	m_tracking;
};

} // VixenEditor

