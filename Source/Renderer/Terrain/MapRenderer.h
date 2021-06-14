#pragma once

#include "Core/Window.h"

#include "Game/Map.h"

class MapRenderer {
public:
	MapRenderer(Ref<Map>& map);

private:
	Ref<Map> map;
};