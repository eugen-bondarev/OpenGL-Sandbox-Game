#pragma once

#include "ColorFBO.h"

#include "Werwel/Texture.h"
#include "Werwel/Shader.h"
#include "Werwel/VAO.h"

#include "Game/Control/Camera.h"

// struct BlockData {
//   struct {
//     float x { 0 }, y { 0 };
//   } pos;

//   struct {
//     float x { 0 }, y { 0 };
//   } tile;

//   BlockData() = default;
//   BlockData(float posX, float posY, float tileX, float tileY) {
//     pos.x = posX;
//     pos.y = posY;
//     tile.x = tileX;
//     tile.y = tileY;
//   }
// };

class ColorPass {
public:
  ColorPass(int amountOfBlocks);

  void Perform(const Ref<Camera>& camera, int amountOfBlocks);

  inline const Ref<Werwel::VBO>& GetVBO() const {
    return vbo;
  }

  inline const Ref<ColorFBO>& GetFBO() const {
    return fbo;
  }

private:
  Ref<ColorFBO> fbo;
  Ref<Werwel::Shader> shader;
  Ref<Werwel::VAO> vao;
  Ref<Werwel::VBO> vbo;
  Ref<Werwel::Texture> tileMap;
};