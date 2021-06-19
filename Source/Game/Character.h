#pragma once

#include "../Maths/Maths.h"

#include "Map.h"
#include "Control/Camera.h"

#include "Core/Time.h"

#define LINOW_USE_GLM
#include "Linow/Linow.h"

class Character {
public:
  Character() = default;

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
    if (!onGround) {
      SetPosition(position + velocity * Vec2(0, 1) * deltaTime);
      velocity += -Physics::g * deltaTime * factor;

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

  void CheckCollisions(
		Ref<Map>& map,
		Ref<Camera>& camera
  ) {
    const auto& blocks = map->GetBlocks();
    Vec2 blockPos = GetBlockPosUnderPlayer(map, camera);

		for (int i = -1; i < 1; i++) {
			Linow::AddQuad(
				Vec3(blockPos + Vec2(16.0f * i, 0.0f), 0.0f), 
				Vec3(blockPos + Vec2(16.0f * i, 0.0f) + map->GetBlockSize(), 0.0f)
			);

      if (blocks[(blockPos.x / 16.0f) + (i + 1)][(blockPos.y / 16.0f) + 1] != BlockType::Empty) {
        if (!onGround) {
          onGround = true;
        }
        break;
      } else {
        onGround = false;
      }
		}

		static float offset = 9.0f;
    static float offsetY = 4.0f;

    static float lineWidth = 1.0f;
		Vec2 colliderTopLeft(GetPosition() + Vec2(offset - lineWidth, 4 * 16 + offsetY - lineWidth));
		Vec2 colliderBotRight(GetPosition() + Vec2(3 * 16 - (offset - lineWidth), offsetY - lineWidth));

		Linow::AddQuad(
			Vec3(colliderTopLeft, 0),
			Vec3(colliderBotRight, 0)
		);
  }

	bool CheckCollision(
		Ref<Map>& map,
		Ref<Camera>& camera
	) {		
		Vec2 block = map->WindowCoordsToBlockCoords(camera->GetPositionOnScreen(GetPosition()), Window::GetSpace(), camera->GetViewMatrix());
		block.x = static_cast<int>(block.x);
		block.y = static_cast<int>(block.y);
		auto charBlock = (block - map->GetChunkSize() / 2.0f) * map->GetBlockSize();
    auto current = charBlock + Vec2(1, 0) * map->GetBlockSize();

    const auto& blocks = map->GetBlocks();
    const auto& coord = current / 16.0f + Vec2(1, 0);

		bool blocksDown = true;
    
		for (int i = 0; i < 2; i++) {
			Vec2 blockPos = coord * 16.0f;
			Linow::AddQuad(
				Vec3((blockPos) + Vec2(16.0f * (i - 1), 0.0f), 0.0f), 
				Vec3((blockPos) + Vec2(16.0f * (i - 1), 0.0f) + map->GetBlockSize(), 0.0f)
			);

			if (blocks[coord.x + i][coord.y + 1] != BlockType::Empty) {
				blocksDown = false;
				break;
			}
		}

    return blocksDown;
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

  inline void CalculateModelMatrix() {
    modelMatrix = Math::Translate(Mat4(1), Vec3(position, 0.0));
  }

  Character(const Character&) = delete;
  Character& operator=(const Character&) = delete;
};