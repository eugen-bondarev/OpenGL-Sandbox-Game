#pragma once

#include "game/ecs/animation/animation.h"

namespace Animation {
namespace Humanoid {

class Walking : public Clip
{
public:
	Walking()
	{
		keyFrames.emplace_back(Vec2{6, 18}, 0);
		keyFrames.emplace_back(Vec2{6, 18}, 0);
		keyFrames.emplace_back(Vec2{8, 18}, 0);
		keyFrames.emplace_back(Vec2{8, 16}, 0);
		keyFrames.emplace_back(Vec2{8, 16}, 0);
		keyFrames.emplace_back(Vec2{6, 16}, 0);
		keyFrames.emplace_back(Vec2{6, 16}, 0);
		keyFrames.emplace_back(Vec2{6, 18}, 0);
		keyFrames.emplace_back(Vec2{4, 18}, 0);
		keyFrames.emplace_back(Vec2{4, 18}, 0);
		keyFrames.emplace_back(Vec2{4, 16}, 0);
		keyFrames.emplace_back(Vec2{4, 16}, 0);
		keyFrames.emplace_back(Vec2{6, 16}, 0);
		keyFrames.emplace_back(Vec2{6, 16}, 0);
	}

	bool InUse(float state) const override
	{
		return state != 1.0f;
	}
		};

}
}