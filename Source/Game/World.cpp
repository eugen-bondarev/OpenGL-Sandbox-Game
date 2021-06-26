#include "World.h"

World::World() {
	map = CreateRef<Map>(669, Vec2(40, 40), Vec2(25, 25), 16.0f);
}