#pragma once

#include "Game/ECS/Animation/Animation.h"

namespace Animation {
namespace Humanoid {

class Attacking : public Clip {
public:
  Attacking() {
    keyFrames.emplace_back(Vec2 { 24, 36 },  30);
    keyFrames.emplace_back(Vec2 { 26, 34 },  15);
    keyFrames.emplace_back(Vec2 { 28, 32 },  0);
    keyFrames.emplace_back(Vec2 { 30, 30 }, -25);
    keyFrames.emplace_back(Vec2 { 28, 28 }, -45);
    keyFrames.emplace_back(Vec2 { 26, 26 }, -60);
    keyFrames.emplace_back(Vec2 { 24, 24 }, -75);    
  }

  bool InUse(float state) const override {
    return state == 1.0f;
  }
};

}
}