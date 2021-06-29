#pragma once

#include "Entities/Character.h"
#include "Renderer/Characters/CharacterRenderer.h"

#include "World.h"
#include "Renderer/World/WorldRenderer.h"

#include "Renderer/Interface/InterfaceRenderer.h"

class Game {
public:
  Game(int seed = 669);
  ~Game();

  void Play(bool& resetGame);

private:
  void Logic(bool& resetGame);
  void Render();
  
  Ref<ToolsTileMap> toolsTileMap;

	std::vector<Ref<Character>> characters;

  Ref<World> world;
  Ref<WorldRenderer> worldRenderer;

	Ref<Character> character;
	Ref<CharacterRenderer> characterRenderer;	

  Ref<InterfaceRenderer> interfaceRenderer;

  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;
};