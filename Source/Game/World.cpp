#include "World.h"

World::World() {
	map = CreateRef<Map>(Size(16, 16), Size(25, 25), 16.0f);
}