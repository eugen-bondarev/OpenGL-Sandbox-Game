#include "World.h"

World::World() {
	map = CreateRef<Map>(Vec2(40, 40), Vec2(25, 25), 16.0f);
}