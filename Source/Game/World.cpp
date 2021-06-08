#include "World.h"

World::World() {
	map = CreateRef<Map>(Size(40, 40), Size(25, 25), 16.0f);
}