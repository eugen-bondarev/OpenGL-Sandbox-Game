#pragma once

#include "entities/character.h"
#include "renderer/characters/character_renderer.h"

#include "world/world.h"
#include "renderer/world/world_renderer.h"

#include "renderer/interface/interface_renderer.h"

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