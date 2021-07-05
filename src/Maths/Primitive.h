#pragma once

#include "Renderer/Vertex.h"

#include <vector>
	
namespace Primitives {
namespace Block {

inline static std::vector<Vertex2D> Vertices(float tileSizeX, float tileSizeY, bool flipUVs = false) {
	return {
		{ {  tileSizeX / 2,  tileSizeY / 2 }, { 1, 0 } },
		{ { -tileSizeX / 2,  tileSizeY / 2 }, { 0, 0 } },
		{ {  tileSizeX / 2, -tileSizeY / 2 }, { 1, 1 } },
		{ { -tileSizeX / 2, -tileSizeY / 2 }, { 0, 1 } },
	};
}

inline static std::vector<int> indices = { 0, 1, 2, 2, 1, 3 };

}
namespace Char {

inline static std::vector<Vertex2D> Vertices(float tileSizeX, float tileSizeY, bool flipUVs = false) {
	return {
		{ { tileSizeX,  tileSizeY }, { 1, 0 } },
		{ { 0,  				tileSizeY }, { 0, 0 } },
		{ { tileSizeX, 	0 }, { 1, 1 } },
		{ { 0, 							0 }, { 0, 1 } },
	};
}

inline static std::vector<int> indices = { 0, 1, 2, 2, 1, 3 };

}

namespace Light {

inline static int tileSize = 16 * 16;

inline static std::vector<Vertex2D> vertices = {
	{ { -tileSize / 2, -tileSize / 2 }, { 1, 0 } },
	{ {  tileSize / 2, -tileSize / 2 }, { 0, 0 } },
	{ { -tileSize / 2,  tileSize / 2 }, { 1, 1 } },
	{ {  tileSize / 2,  tileSize / 2 }, { 0, 1 } },
};

inline static std::vector<int> indices = { 0, 1, 2, 2, 1, 3 };

}

namespace Canvas {

inline static std::vector<Vertex2D> vertices = {
	{ { -1.0f, -1.0f }, { 0, -1 } },
	{ {  1.0f, -1.0f }, { 1, -1 } },
	{ { -1.0f,  1.0f }, { 0,  0 } },
	{ {  1.0f,  1.0f }, { 1,  0 } },
};

inline static std::vector<int> indices = { 0, 1, 2, 2, 1, 3 };

}

namespace Pixel {

inline static std::vector<Vertex2D> vertices = {
	{ { -0.5f, -0.5f }, { 0, 1 } },
	{ {  0.5f, -0.5f }, { 1, 1 } },
	{ { -0.5f,  0.5f }, { 0, 0 } },
	{ {  0.5f,  0.5f }, { 1, 0 } },
};

inline static std::vector<int> indices = { 0, 1, 2, 2, 1, 3 };

}

}