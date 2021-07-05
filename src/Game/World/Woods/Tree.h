#pragma once

#include "Maths/Maths.h"

class Tree {
public:
  Tree(Vec2 position);

  Vec2 GetPosition() const;

private:
  Vec2 position;
};