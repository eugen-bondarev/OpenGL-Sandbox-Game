#include "tree.h"

Tree::Tree(Vec2 position) : position { position } {

}

Vec2 Tree::GetPosition() const {
  return position;
}