#pragma once

#include "Entities/Character.h"
#include "Map.h"

#include "Renderer/MapRenderer.h"
#include "Renderer/Characters/CharacterRenderer.h"

#include "World.h"
#include "Renderer/WorldRenderer.h"

class Game {
public:
  Game(int seed = 669);

  void Play(bool& resetGame);

private:
  void Logic(bool& resetGame);
  void Render();

	std::vector<Ref<Character>> characters;

  Ref<World> world;
  Ref<WorldRenderer> worldRenderer;

	Ref<Character> character;
	Ref<CharacterRenderer> characterRenderer;	

  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;
};