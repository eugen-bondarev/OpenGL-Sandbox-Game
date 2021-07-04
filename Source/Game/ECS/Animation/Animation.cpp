#include "Animation.h"

namespace Animation {

int Clip::GetFrameIndex() const {
  return static_cast<int>(truncf(time)) % keyFrames.size();
}

void Clip::ApplyTo(Mat4& transform) const {
  keyFrames[GetFrameIndex()].ApplyTo(transform);
}

bool Clip::InUse(float state) const { 
  return true; 
}

}