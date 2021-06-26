#pragma once

#include "Character.h"
#include "Map.h"

#include "Renderer/MapRenderer.h"
#include "Renderer/Characters/CharacterRenderer.h"

class Game {
public:
  Game();

  void Play();

private:
  void Logic();
  void Render();

	std::vector<Ref<Character>> characters;

	Ref<Map> map;
	Ref<MapRenderer> mapRenderer;

	Ref<Character> character;
	Ref<CharacterRenderer> characterRenderer;
	
	Ref<Camera> camera;

  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;
};