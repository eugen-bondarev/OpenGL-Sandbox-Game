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
    collider = CreateRef<Collider>(position, ColliderRect { { 8.0f, 8.0f }, { 2.0f, 2.0f } });
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

  inline static float factor = 150.0f;
  void Update(float deltaTime, const Ref<Map>& map, const Ref<Camera>& camera) {
    Linow::AddQuad(
      Vec3(collider->GetStart(), 0.0f),
      Vec3(collider->GetEnd(), 0.0f)
    );

    if (!onGround) {
      SetPosition(position + velocity * Vec2(0, 1) * deltaTime);
      // velocity += -Physics::g * deltaTime * factor;

      if (ceiling) {
        velocity = Vec2(0.0f);
        ceiling = false;
      }
    } else {
      velocity = Vec2(0.0f);
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