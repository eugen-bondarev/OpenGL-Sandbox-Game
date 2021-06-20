#pragma once

#include "../Maths/Maths.h"

#include "Map.h"
#include "Control/Camera.h"

#include "Core/Time.h"

#define LINOW_USE_GLM
#include "Linow/Linow.h"

#include "Physics/Collider.h"

class Character {
public:
  Character() {
    collider = CreateRef<Collider>(ColliderRect { { 8.0f, 8.0f }, { 2.0f, 2.0f } }, Vec2 { 16 * 3, 16 * 4 });
  }

  inline Vec2 GetPosition() const {
    return position;
  }

  template <typename... Args>
  inline void SetPosition(Args&&... args) {
    position = Vec2(std::forward<Args>(args)...);
    CalculateModelMatrix();
  }

  inline void SetPositionY(float y) {
    position.y = y;
    CalculateModelMatrix();
  }

  inline void SetPositionX(float x) {
    position.x = x;
    CalculateModelMatrix();
  }

  template <typename... Args>
  inline void AddPosition(Args... args) {
    Vec2 delta = Vec2(std::forward<Args>(args)...);
    SetPosition(position + delta);
  }

  inline const Mat4& GetModelMatrix() const {
    return modelMatrix;
  }

  struct Block {
    Vec2 index;
    Vec2 worldPosition;
  };

  inline Block GetBlock(Vec2 pos, Vec2 fix, Vec2 offset = Vec2(0, 0)) {
    Block block;
    block.worldPosition = ToInt((pos + fix + offset * 16.0f) / 16.0f) * 16.0f - 8.0f;
    block.index = (block.worldPosition + 8.0f) / 16.0f;
    return block;    
  }

  inline static float factor = 150.0f;

  void CheckCollisions(const Ref<Map>& map, const Ref<Camera>& camera) {
    auto& blocks = map->GetBlocks();
    onGround = false;

    collider->SetPosition(position);
    Linow::AddLine(collider->GetStart(), collider->GetStart() + Vec2(1.0f, 0.0f) * map->GetBlockSize() * 2.0f);

    for (int i = 2; i < 4; i++) {
      Block block = GetBlock(position, Vec2(-10.0f, 10.0f), Vec2(i, 0));
      Linow::AddQuad(block.worldPosition, block.worldPosition + 16.0f);

      if (blocks[block.index.x][block.index.y] != BlockType::Empty) {
        SetPositionY(block.worldPosition.y + map->GetBlockSize() - 4.0f);

        for (int j = 1; j < 5; j++) {
          if (blocks[block.index.x][block.index.y + j] != BlockType::Empty) {
            SetPositionY(block.worldPosition.y + map->GetBlockSize() * j + map->GetBlockSize() - 4.0f);
            break;
          }
        }

        onGround = true;
        break;
      }
    }

    ceiling = false;
    for (int i = 2; i < 4; i++) {
      Block block = GetBlock(position, Vec2(-10.0f, -10.0f), Vec2(i, 6));
      Linow::AddQuad(block.worldPosition, block.worldPosition + 16.0f);

      if (blocks[block.index.x][block.index.y - 1] != BlockType::Empty) {
        SetPositionY(block.worldPosition.y - 5 * 16.0f);
        ceiling = true;
        break;
      }
    }

    canMoveLeft = true;
    for (int i = 1; i < 5; i++) {
      Block block = GetBlock(position, Vec2(-2.0f, 10.0f), Vec2(1, i));
      Linow::AddQuad(block.worldPosition, block.worldPosition + 16.0f);

      if (blocks[block.index.x][block.index.y] != BlockType::Empty) {
        canMoveLeft = false;
        break;
      }
    }

    canMoveRight = true;
    for (int i = 1; i < 5; i++) {
      Block block = GetBlock(position, Vec2(-2.0f, 10.0f), Vec2(3, i));
      Linow::AddQuad(block.worldPosition, block.worldPosition + 16.0f);

      if (blocks[block.index.x][block.index.y] != BlockType::Empty) {
        canMoveRight = false;
        break;
      }
    }
  }

  void Update(float deltaTime, const Ref<Map>& map, const Ref<Camera>& camera) {
    if (!onGround) {
      SetPosition(position + velocity * Vec2(0, 1) * Time::delta);
      velocity += -Physics::g * Time::delta * factor;

      if (ceiling) {
        velocity.y = 0.0f;
        ceiling = false;
      }
    } else {
      velocity.y = 0.0f;
    }

    CheckCollisions(map, camera);
  }

  inline void Jump() {
    // Without vSync:
    SetPositionY(position.y + 16.0f - 4.0f - 10.0f);
    velocity = Vec2(0.0f, 4.0f * factor);
    onGround = false;
  }

  Vec2 GetBlockPosUnderPlayer(Ref<Map>& map, Ref<Camera>& camera) {
		Vec2 block = map->WindowCoordsToBlockCoords(camera->GetPositionOnScreen(GetPosition()), Window::GetSpace(), camera->GetViewMatrix());
    block = ToInt(block);
		auto charBlock = (block - map->GetChunkSize() / 2.0f) * map->GetBlockSize();
    Vec2 blockPos = charBlock + Vec2(2.0f, 0.0f) * map->GetBlockSize();

    return blockPos;
  }

  inline void Ceiling(bool value) { ceiling = value; }
  inline bool Ceiling() { return ceiling; }

  inline void OnGround(bool value) { onGround = value; }
  inline bool OnGround() const { return onGround; }

  inline void CanMoveLeft(bool value) { canMoveLeft = value; }
  inline void CanMoveRight(bool value) { canMoveRight = value; }
  inline bool CanMoveLeft() const { return canMoveLeft; }
  inline bool CanMoveRight() const { return canMoveRight; }

  bool intersection { false };

private:
  bool ceiling      { false };
  bool onGround     { false };
  bool canMoveLeft  { true };
  bool canMoveRight { true };

  Vec2 velocity { 0 };

  Vec2 position;
  Mat4 modelMatrix;
  
  Ref<Collider> collider;

  inline void CalculateModelMatrix() {
    modelMatrix = Math::Translate(Mat4(1), Vec3(position, 0.0));
  }

  Character(const Character&) = delete;
  Character& operator=(const Character&) = delete;
};