#pragma once

#include "VixLib\include\camera\CameraData.h"


namespace VixLib {
namespace camera {
namespace transform {

class IRotate {
public:
  virtual ~IRotate() { }

  virtual void rotate(CameraData& data, float dx, float dy) const = 0;
};


class TurnTableRotate : public IRotate {
public:
  TurnTableRotate(float factor);

  virtual void rotate(CameraData& data, float dx, float dy) const override;

private:
  float m_factor;
};


} // transform
} // camera
} // VixLib
