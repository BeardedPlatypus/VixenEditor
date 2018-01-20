#pragma once

#include "pch.h"

namespace VixLib {
namespace graphics {

struct CameraCB {
  DirectX::XMFLOAT4X4 model_view;
  DirectX::XMFLOAT4X4 inv_transpose_model_view;
  DirectX::XMFLOAT4X4 projection;
};


struct VertexPositionNormalColor {
  DirectX::XMFLOAT3 pos;
  DirectX::XMFLOAT3 normal;
  DirectX::XMFLOAT3 color;
};


struct LightingCB {
  DirectX::XMFLOAT4 pos;
};

} // graphics
} // VixLib