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

  Vec2 nextPosition;
  Mat4 nextModelMatrix;

  inline static float factor = 150.0f;

  float stopAt = 0.0f;

  void Update(float deltaTime, const Ref<Map>& map, const Ref<Camera>& camera) {

    if (!onGround) {
      SetPosition(position + velocity * Vec2(0, 1) * Time::delta);
      velocity += -Physics::g * Time::delta * factor;

      Vec2 nextVelocity = velocity - Physics::g * Time::nextDelta * factor;
      nextPosition = position + nextVelocity * 4.0f * Vec2(0, 1) * Time::nextDelta;
      nextModelMatrix = Math::Translate(Mat4(1), Vec3(nextPosition + Vec2(64, 0), 0.0));

      if (ceiling) {
        velocity = Vec2(0.0f);
        ceiling = false;
      }
    } else {
      velocity = Vec2(0.0f);
      nextPosition.x = position.x;
    }

    auto& blocks = map->GetBlocks();
    onGround = false;

    collider->SetPosition(position);    

    Linow::AddLine(collider->GetStart(), collider->GetStart() + Vec2(1.0f, 0.0f) * 16.0f * 2.0f);

    for (int i = 2; i < 4; i++) {
      Block block = GetBlock(position + Vec2(0, 0), Vec2(-5.0f, 5.0f), Vec2(i, -1));
      Block nextBlock = GetBlock(nextPosition + Vec2(0, 0), Vec2(-5.0f, 0.0f), Vec2(i, 0));

      Collider blockCollider({ 0.0f, 0.0f, 0.0f, 0.0f }, { 16, 16 });
      blockCollider.SetPosition(block.worldPosition);
      Linow::AddQuad(blockCollider.GetStart(), blockCollider.GetEnd());

      Collider nextBlockCollider({ 0.0f, 0.0f, 0.0f, 0.0f }, { 16, 16 });
      nextBlockCollider.SetPosition(nextBlock.worldPosition);

      if (blocks[nextBlock.index.x][nextBlock.index.y + 1] != BlockType::Empty) {    
        for (int j = 1; j < 5; j++) {
          if (blocks[nextBlock.index.x][nextBlock.index.y + 1 + j] == BlockType::Empty) {
            stopAt = (nextBlock.index.y + j) * 16.0f + collider->GetRect().y.bottom;
            break;
          }
        }
      }

      if (blocks[block.index.x][block.index.y + 1] != BlockType::Empty) {
        if (stopAt) {
          SetPositionY(stopAt);
          stopAt = 0.0f;
        }
        onGround = true;
        return;
      }
    }
  }

  inline void Jump() {
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