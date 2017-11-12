#pragma once

#include "pch.h"

namespace VixLib {
namespace graphics {

struct CameraCB {
  DirectX::XMFLOAT4X4 model_view;
  DirectX::XMFLOAT4X4 inv_transpose_model_view;
  DirectX::XMFLOAT4X4 projection;
};


struct VertexPositionColor {
  DirectX::XMFLOAT3 pos;
  DirectX::XMFLOAT3 normal;
};


struct LightingCB {
  DirectX::XMFLOAT4 pos;
};

} // graphics
} // VixLib