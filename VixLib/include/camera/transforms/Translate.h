#pragma once

#include "VixLib\include\camera\CameraData.h"


namespace VixLib {
namespace camera {
namespace transform {

class ITranslate {
public:
  virtual ~ITranslate() { }

  virtual void translate(CameraData& data, float dx, float dy) const = 0;
};


class Translate : public ITranslate {
public:
  Translate(float factor);

  virtual void translate(CameraData& data, float dx, float dy) const override;

private:
  float m_factor;
};


} // transform
} // camera
} // VixLib
