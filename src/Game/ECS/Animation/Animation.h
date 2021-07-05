#pragma once

#include "KeyFrame.h"

namespace Animation {

class Clip {
public:
  int GetFrameIndex() const;
  void ApplyTo(Mat4& transform) const;
  virtual bool InUse(float state) const;

  float GetTime() const;
  void SetTime(float time);

protected:
  std::vector<KeyFrame> keyFrames;

private:
  float time { 0 };
};

}