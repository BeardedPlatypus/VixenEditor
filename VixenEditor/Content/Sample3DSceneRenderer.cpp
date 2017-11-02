#include "pch.h"
#include "Sample3DSceneRenderer.h"

#include "VixCommon\include\DirectXHelper.h"


using namespace DirectX;
using namespace Windows::Foundation;

namespace VixenEditor { 

// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Sample3DSceneRenderer::Sample3DSceneRenderer(const std::shared_ptr<VixCommon::DeviceResources>& device_resources) :
	m_loading_complete(false),
	m_degrees_per_second(45),
	m_index_count(0),
	m_tracking(false),
	m_device_resources(device_resources) {
	this->createDeviceDependentResources();
	this->createWindowSizeDependentResources();
}


// Initializes view parameters when the window size changes.
void Sample3DSceneRenderer::createWindowSizeDependentResources() {
	Size output_size = m_device_resources->getOutputSize();

	float aspect_ratio = output_size.Width / output_size.Height;
	float fov_angle_y = 70.0f * XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspect_ratio < 1.0f) fov_angle_y *= 2.0f;

	// Note that the OrientationTransform3D matrix is post-multiplied here
	// in order to correctly orient the scene to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain render target. For draw calls to other targets,
	// this transform should not be applied.

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	XMMATRIX perspective_matrix = XMMatrixPerspectiveFovRH(fov_angle_y,
                                                         aspect_ratio,
                                                         0.01f,
                                                         100.0f);

	XMFLOAT4X4 orientation = m_device_resources->getOrientationTransform3D();

	XMMATRIX orientation_matrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(&(this->m_constant_buffer_data.projection),
                  XMMatrixTranspose(perspective_matrix * orientation_matrix));

	// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
	static const XMVECTORF32 eye = { 0.0f,  0.7f, 1.5f, 0.0f };
	static const XMVECTORF32 at  = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up  = { 0.0f,  1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&(this->m_constant_buffer_data.view), 
                  XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
}


// Called once per frame, rotates the cube and calculates the model and view matrices.
void Sample3DSceneRenderer::update(DX::StepTimer const& timer) {
	if (!this->m_tracking) {
		// Convert degrees to radians, then convert seconds to rotation angle
		float radians_per_second = XMConvertToRadians(m_degrees_per_second);
		double totalRotation = timer.GetTotalSeconds() * radians_per_second;
		float radians = static_cast<float>(fmod(totalRotation, XM_2PI));

		this->rotate(radians);
	}
}


// Rotate the 3D cube model a set amount of radians.
void Sample3DSceneRenderer::rotate(float radians) {
	// Prepare to pass the updated model matrix to the shader
	XMStoreFloat4x4(&(this->m_constant_buffer_data.model), 
                  XMMatrixTranspose(XMMatrixRotationY(radians)));
}


void Sample3DSceneRenderer::startTracking() {
	this->m_tracking = true;
}


// When tracking, the 3D cube can be rotated around its Y axis by tracking pointer position relative to the output screen width.
void Sample3DSceneRenderer::trackingUpdate(float positionX) {
	if (this->m_tracking) {
		float radians = XM_2PI * 2.0f * positionX / this->m_device_resources->getOutputSize().Width;
		rotate(radians);
	}
}


void Sample3DSceneRenderer::stopTracking() {
	this->m_tracking = false;
}


// Renders one frame using the vertex and pixel shaders.
void Sample3DSceneRenderer::render() {
	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!this->m_loading_complete) return;

	auto context = this->m_device_resources->getD3DDeviceContext();

	// Prepare the constant buffer to send it to the graphics device.
	context->UpdateSubresource1(this->m_constant_buffer.Get(),
                              0,
                              NULL,
                              &(this->m_constant_buffer_data),
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
	context->VSSetConstantBuffers1(0,
                                 1,
                                 this->m_constant_buffer.GetAddressOf(),
                                 nullptr,
                                 nullptr);

	// Attach our pixel shader.
	context->PSSetShader(this->m_pixel_shader.Get(),
                       nullptr,
                       0);

	// Draw the objects.
	context->DrawIndexed(this->m_index_count,
                       0,
                       0);
}


void Sample3DSceneRenderer::createDeviceDependentResources() {
	// Load shaders asynchronously.
	auto loadVSTask = DX::ReadDataAsync(L"SampleVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"SamplePixelShader.cso");

	// After the vertex shader file is loaded, create the shader and input layout.
	auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
      this->m_device_resources->getD3DDevice()->CreateVertexShader(&fileData[0],
                                                                   fileData.size(),
                                                                   nullptr,
                                                                   &this->m_vertex_shader));

		static const D3D11_INPUT_ELEMENT_DESC vertex_desc [] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		DX::ThrowIfFailed(this->m_device_resources->getD3DDevice()->CreateInputLayout(
      vertex_desc,
      ARRAYSIZE(vertex_desc),
      &fileData[0],
      fileData.size(),
      &(this->m_input_layout)));
	});

	// After the pixel shader file is loaded, create the shader and constant buffer.
	auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			this->m_device_resources->getD3DDevice()->CreatePixelShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&(this->m_pixel_shader)));

		CD3D11_BUFFER_DESC constant_buffer_desc(sizeof(ModelViewProjectionConstantBuffer) , D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
      this->m_device_resources->getD3DDevice()->CreateBuffer(
        &constant_buffer_desc,
        nullptr,
        &(this->m_constant_buffer)));
	});

	// Once both shaders are loaded, create the mesh.
	auto createCubeTask = (createPSTask && createVSTask).then([this] () {

		// Load mesh vertices. Each vertex has a position and a color.
		static const VertexPositionColor cubeVertices[] = {
			{XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
			{XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
			{XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
			{XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f)},
			{XMFLOAT3( 0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
			{XMFLOAT3( 0.5f, -0.5f,  0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f)},
			{XMFLOAT3( 0.5f,  0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f)},
			{XMFLOAT3( 0.5f,  0.5f,  0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f)},
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


void Sample3DSceneRenderer::releaseDeviceDependentResources() {
	this->m_loading_complete = false;
	this->m_vertex_shader.Reset();
	this->m_input_layout.Reset();
	this->m_pixel_shader.Reset();
	this->m_constant_buffer.Reset();
	this->m_vertex_buffer.Reset();
	this->m_index_buffer.Reset();
}

}