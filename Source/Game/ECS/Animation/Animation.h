#pragma once

#include "KeyFrame.h"

namespace Animation {

struct Clip {
  std::vector<KeyFrame> keyFrames;
  float time { 0 };

  int GetFrameIndex() const;
  void ApplyTo(Mat4& transform) const;
  virtual bool InUse(float state) const;
};

}