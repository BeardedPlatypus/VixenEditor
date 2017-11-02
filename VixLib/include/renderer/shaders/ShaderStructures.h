#pragma once

#include "pch.h"

namespace VixLib {
namespace graphics {

struct CameraCB {
  DirectX::XMFLOAT4X4 model;
  DirectX::XMFLOAT4X4 view;
  DirectX::XMFLOAT4X4 projection;
};


struct VertexPositionColor {
  DirectX::XMFLOAT3 pos;
  DirectX::XMFLOAT3 color;
};

} // graphics
} // VixLib