#pragma once

namespace VixenEditor {

// Constant buffer used to send MVP matrices to the vertex shader.
struct ModelViewProjectionConstantBuffer {
  DirectX::XMFLOAT4X4 _model;
};

// Used to send per-vertex data to the vertex shader.
struct VertexPositionColor {
  DirectX::XMFLOAT3 pos;
  DirectX::XMFLOAT3 color;
};

} // VixenEditor