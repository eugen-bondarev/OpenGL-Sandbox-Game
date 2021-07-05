#pragma once

#include "maths/maths.h"

class Tree
{
public:
	Tree(Vec2 position);

	Vec2 GetPosition() const;

private:
	Vec2 position;
};