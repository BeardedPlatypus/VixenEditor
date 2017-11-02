#pragma once

#include "VixLib\include\camera\CameraData.h"

namespace VixLib {
namespace camera {
namespace transform {

class IZoom {
public:
  virtual ~IZoom() { }

  virtual void zoom(CameraData& data, float d) const = 0;
};


class Zoom : public IZoom {
public:
  Zoom(float factor);

  virtual void zoom(CameraData& data, float d) const override;

private:
  float m_factor;
};

} // transform
} // camera
} // VixLib
