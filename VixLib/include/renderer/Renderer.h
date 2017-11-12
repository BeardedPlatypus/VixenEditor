#pragma once

#include "VixCommon\include\DeviceResources.h"
#include "VixLib\include\camera\Camera.h"
#include "shaders\ShaderStructures.h"


namespace VixLib {
namespace graphics {

class Renderer {
public:
  Renderer(const std::shared_ptr<VixCommon::DeviceResources>& device_resources,
           const camera::CameraData& camera_data);

  void createDeviceDependentResources();
  void releaseDeviceDependentResources();

  void render();

private:
  std::shared_ptr<VixCommon::DeviceResources> m_device_resources;

  // Direct3D resources for cube geometry.
  Microsoft::WRL::ComPtr<ID3D11InputLayout>	  m_input_layout;
  Microsoft::WRL::ComPtr<ID3D11Buffer>		    m_vertex_buffer;
  Microsoft::WRL::ComPtr<ID3D11Buffer>		    m_index_buffer;
  Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertex_shader;
  Microsoft::WRL::ComPtr<ID3D11PixelShader>	  m_pixel_shader;
  
  // Constant buffers
  Microsoft::WRL::ComPtr<ID3D11Buffer>		    m_camera_cb;
  Microsoft::WRL::ComPtr<ID3D11Buffer>		    m_lighting_cb;

  uint32 m_index_count;

  CameraCB   m_camera_cb_data;
  LightingCB m_lighting_cb_data;

  const camera::CameraData& m_camera_data;

  bool m_loading_complete;
};


}
}