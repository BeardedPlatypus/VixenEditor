#include "pch.h"
#include "include\renderer\Renderer.h"

#include "include\renderer\shaders\ShaderStructures.h"

using namespace DirectX;
using namespace Windows::Foundation;

namespace VixLib {
namespace graphics {

bool matrixEquals(XMFLOAT4X4 a, XMFLOAT4X4 b) {
  return (a(0, 0) == b(0, 0) && a(0, 1) == b(0, 1) && a(0, 2) == b(0, 2) && a(0, 3) == b(0, 3) &&
          a(1, 0) == b(1, 0) && a(1, 1) == b(1, 1) && a(1, 2) == b(1, 2) && a(1, 3) == b(1, 3) &&
          a(2, 0) == b(2, 0) && a(2, 1) == b(2, 1) && a(2, 2) == b(2, 2) && a(2, 3) == b(2, 3) &&
          a(3, 0) == b(3, 0) && a(3, 1) == b(3, 1) && a(3, 2) == b(3, 2) && a(3, 3) == b(3, 3));
}


Renderer::Renderer(const std::shared_ptr<VixCommon::DeviceResources>& device_resources,
                   const camera::CameraData& camera_data) :
    m_loading_complete(false),
    m_camera_data(camera_data),
    m_camera_cb_data(),
    m_device_resources(device_resources) {
  this->createDeviceDependentResources();

  // TODO fix this with object setting
  // Set model matrix up
  XMStoreFloat4x4(&(this->m_camera_cb_data.model),
                  XMMatrixIdentity());

  this->m_camera_cb_data.view = this->m_camera_data.look_at_matrix; //look_at;
  this->m_camera_cb_data.projection = this->m_camera_data.projection_matrix;
}


void Renderer::createDeviceDependentResources() {
  // Load shaders asynchronously
	auto loadVSTask = DX::ReadDataAsync(L"VixLib\\VertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"VixLib\\PixelShader.cso");
  
	// After the vertex shader file is loaded, create the shader, input layout and buffers.
	auto createVSTask = loadVSTask.then([this](const std::vector<byte>& file_data) {
		DX::ThrowIfFailed(this->m_device_resources->getD3DDevice()->CreateVertexShader(
      &file_data[0],
      file_data.size(),
      nullptr,
      &this->m_vertex_shader));

    // Specify the layout of the input element
		static const D3D11_INPUT_ELEMENT_DESC vertex_desc [] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

    // Set up the layout
		DX::ThrowIfFailed(this->m_device_resources->getD3DDevice()->CreateInputLayout(
      vertex_desc,
      ARRAYSIZE(vertex_desc),
      &file_data[0],
      file_data.size(),
      &(this->m_input_layout)));

	});

	auto createPSTask = loadPSTask.then([this](const std::vector<byte>& file_data) {
		DX::ThrowIfFailed(
			this->m_device_resources->getD3DDevice()->CreatePixelShader(
				&file_data[0],
				file_data.size(),
				nullptr,
				&(this->m_pixel_shader)));

    // Create the relevant buffers
		CD3D11_BUFFER_DESC camera_cb_desc(sizeof(CameraCB) , D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
      this->m_device_resources->getD3DDevice()->CreateBuffer(
        &camera_cb_desc,
        nullptr,
        &(this->m_camera_cb)));

	});

	// Once both shaders are loaded, create the mesh.
	auto createCubeTask = (createPSTask && createVSTask).then([this] () {
		// Load mesh vertices. Each vertex has a position and a color.
		static const VertexPositionColor cubeVertices[] = {
			{XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
			{XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
			{XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
			{XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f)},
			{XMFLOAT3( 0.5f, -0.5f,  0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
			{XMFLOAT3( 0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f)},
			{XMFLOAT3( 0.5f,  0.5f,  0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f)},
			{XMFLOAT3( 0.5f,  0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f)},
		};

		D3D11_SUBRESOURCE_DATA vertex_buffer_data = {0};
		vertex_buffer_data.pSysMem = cubeVertices;
		vertex_buffer_data.SysMemPitch = 0;
		vertex_buffer_data.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertex_buffer_desc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER);
		DX::ThrowIfFailed(
			this->m_device_resources->getD3DDevice()->CreateBuffer(
				&vertex_buffer_desc,
				&vertex_buffer_data,
				&(this->m_vertex_buffer)));

		// Load mesh indices. Each trio of indices represents
		// a triangle to be rendered on the screen.
		// For example: 0,2,1 means that the vertices with indexes
		// 0, 2 and 1 from the vertex buffer compose the 
		// first triangle of this mesh.
		static const unsigned short cubeIndices [] = {
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		this->m_index_count = ARRAYSIZE(cubeIndices);

		D3D11_SUBRESOURCE_DATA index_buffer_data = {0};
		index_buffer_data.pSysMem = cubeIndices;
		index_buffer_data.SysMemPitch = 0;
		index_buffer_data.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC index_buffer_desc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(this->m_device_resources->getD3DDevice()->CreateBuffer(
      &index_buffer_desc,
      &index_buffer_data,
			&this->m_index_buffer));
	});

	// Once the cube is loaded, the object is ready to be rendered.
	createCubeTask.then([this] () { this->m_loading_complete = true; });
}


void Renderer::render() {
	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!this->m_loading_complete) return;

	auto context = this->m_device_resources->getD3DDeviceContext();

  // Update Camera Model if necessary
  //bool needs_update = false;

  /*
  //if (!matrixEquals(this->m_camera_data.look_at_matrix, 
  //                  this->m_camera_cb_data.view)) {
    needs_update = true;
    this->m_camera_cb_data.view = this->m_camera_data.look_at_matrix;
  //}
    XMFLOAT4X4 model;
    XMStoreFloat4x4(&model,
                    XMMatrixIdentity());
   
    XMFLOAT4X4 model2;
    XMStoreFloat4x4(&model2,
                    XMMatrixIdentity());

    float radiansPerSecond = XMConvertToRadians(45);
    double totalRotation = 1 * radiansPerSecond;
    float radians = static_cast<float>(fmod(totalRotation, DirectX::XM_2PI));

    XMStoreFloat4x4(&model2, XMMatrixTranspose(XMMatrixRotationY(radians)));

    XMFLOAT4X4 view = this->m_camera_data.look_at_matrix;
    XMFLOAT4X4 projection = this->m_camera_data.projection_matrix;

  //if (!matrixEquals(this->m_camera_data.projection_matrix, 
  //                  this->m_camera_cb_data.projection)) {
    needs_update = true;
    this->m_camera_cb_data.projection = this->m_camera_data.projection_matrix;
  //}

    XMFLOAT4X4 look_at;

    XMStoreFloat4x4(&look_at, 
                    XMMatrixTranspose(XMMatrixLookAtRH(this->m_camera_data.eye, 
                                                       this->m_camera_data.center, 
                                                       this->m_camera_data.up)));
    //static const XMVECTORF32 eye = { 0.0f, 0.7f, 1.5f, 0.0f };
    //static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
    //static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

    //XMStoreFloat4x4(&look_at, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));

    CameraCB cb = { model, view, projection };
    */

  //XMFLOAT4X4 model;
  //XMStoreFloat4x4(&model,
  //                XMMatrixIdentity());

   // XMFLOAT4X4 look_at;

    //XMStoreFloat4x4(&look_at, 
    //                XMMatrixTranspose(XMMatrixLookAtRH(this->m_camera_data.eye, 
    //                                                   this->m_camera_data.center, 
    //                                                   this->m_camera_data.up)));

    //static const XMVECTORF32 eye = { 0.0f, 0.7f, 1.5f, 0.0f };
    //static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
    //static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

    //XMStoreFloat4x4(&look_at, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
  //CameraCB cb = { this->m_camera_cb_data.model,
  //                look_at, //this->m_camera_cb_data.view, 
  //                this->m_camera_data.projection_matrix };
  //DirectX::XMStoreFloat4x4(&this->m_camera_cb_data.view, DirectX::XMMatrixTranspose(DirectX::XMLoadFLoat4x4(this->m_camera_data.look_at_matrix)));
  XMStoreFloat4x4(&this->m_camera_cb_data.view,       XMMatrixTranspose(XMLoadFloat4x4(&this->m_camera_data.look_at_matrix)));
  XMStoreFloat4x4(&this->m_camera_cb_data.projection, XMMatrixTranspose(XMLoadFloat4x4(&this->m_camera_data.projection_matrix)));

  //this->m_camera_cb_data.view = this->m_camera_data.look_at_matrix; //look_at;
  //this->m_camera_cb_data.projection = this->m_camera_data.projection_matrix;

  context->UpdateSubresource1(this->m_camera_cb.Get(),
                              0,
                              NULL,
                              //&(cb),
                              &(this->m_camera_cb_data),
                              0,
                              0,
                              0);

	// Each vertex is one instance of the VertexPositionColor struct.
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	context->IASetVertexBuffers(0,
                              1,
                              this->m_vertex_buffer.GetAddressOf(),
                              &stride,
                              &offset);

	context->IASetIndexBuffer(this->m_index_buffer.Get(),
                            DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
                            0);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->IASetInputLayout(this->m_input_layout.Get());

	// Attach our vertex shader.
	context->VSSetShader(this->m_vertex_shader.Get(),
                       nullptr,
                       0);

	// Send the constant buffer to the graphics device.
  //if (needs_update) {
  context->VSSetConstantBuffers1(0,
                                 1,
                                 this->m_camera_cb.GetAddressOf(),
                                 nullptr,
                                 nullptr);
  //}

	// Attach our pixel shader.
	context->PSSetShader(this->m_pixel_shader.Get(),
                       nullptr,
                       0);

	// Draw the objects.
	context->DrawIndexed(this->m_index_count,
                       0,
                       0);
}


void Renderer::releaseDeviceDependentResources() {
	this->m_loading_complete = false;
	this->m_vertex_shader.Reset();
	this->m_input_layout.Reset();
	this->m_pixel_shader.Reset();
	this->m_camera_cb.Reset();
	this->m_vertex_buffer.Reset();
	this->m_index_buffer.Reset();
}


}
}