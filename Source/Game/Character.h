// #pragma once

// #include "../Math/Math.h"

// #include "Renderer/DebugRenderer.h"
// #include "Map.h"
// #include "Control/Camera.h"

// class Character {
// public:
//   Character() = default;

//   inline Vec2 GetPosition() const {
//     return position;
//   }

//   template <typename... Args>
//   inline void SetPosition(Args&&... args) {
//     position = Vec2(std::forward<Args>(args)...);
//     CalculateModelMatrix();
//   }

//   inline void SetPositionY(float y) {
//     position.y = y;
//     CalculateModelMatrix();
//   }

//   template <typename... Args>
//   inline void AddPosition(Args... args) {
//     Vec2 delta = Vec2(std::forward<Args>(args)...);
//     SetPosition(position + delta);
//   }

//   inline const Mat4& GetModelMatrix() const {
//     return modelMatrix;
//   }

//   void Update(float deltaTime) {
//     if (!onGround) {
//       SetPosition(position + Vec2(0.0f, deltaTime) * acceleration);
//       acceleration += -Physics::g;

//       if (ceiling) {
//         acceleration = 0;
//         ceiling = false;
//       }
//     } else {
//       acceleration = 0;
//     }
//   }

//   inline void Jump() {
//     acceleration = 320.0f;
//     onGround = false;
//   }

//   void CheckCollisions(
// 		Ref<Map>& map,
// 		Ref<Camera>& camera
// 		// ,Ref<DebugRenderer>& debugRenderer = Ref<DebugRenderer>()
//   ) {
// 		CanMoveLeft(CheckCollision(
// 			map,
// 			camera,
// 			Vec2(0, 1), 
// 			Vec2(0, 1),
// 			Vec2(0),
// 			Vec2(0.0f, 0.0f),
// 			4,
// 			8.0f,
// 			true
// 			// ,			debugRenderer
// 		));
		
// 		CanMoveRight(CheckCollision(
// 			map,
// 			camera,
// 			Vec2(0, 1), 
// 			Vec2(3, 1),
// 			Vec2(3, 0),
// 			Vec2(0.0f, 0.0f),
// 			4,
// 			8.0f,
// 			false
// 			// ,			debugRenderer
// 		));

// 		OnGround(!CheckCollision( 
// 			map,
// 			camera,
// 			Vec2(1, 0), 
// 			Vec2(1, 0),
// 			Vec2(0),
// 			Vec2(0.0f, -4.0f),
// 			2,
// 			0.0f,
// 			true
// 			// ,			debugRenderer
// 		));

// 		Ceiling(!CheckCollision( 
// 			map,
// 			camera,
// 			Vec2(1, 0), 
// 			Vec2(1, 4),
// 			Vec2(0, 3),
// 			Vec2(0.0f, -4.0f),
// 			2,
// 			0.0f,
// 			true
// 			// ,			debugRenderer
// 		));
//   }

// 	bool CheckCollision(
// 		Ref<Map>& map,
// 		Ref<Camera>& camera,

// 		Vec2 affect,
// 		Vec2 check,
// 		Vec2 characterSize,
// 		Vec2 correction,

// 		int times,
// 		float leaveSpace,
// 		bool reverse,

//     Ref<DebugRenderer>& debugRenderer = Ref<DebugRenderer>()
// 	) {		
// 		Vec2 block = map->WindowCoordsToBlockCoords(camera->GetPositionOnScreen(GetPosition()), Window::GetSpace(), camera->GetViewMatrix());
// 		block.x = static_cast<int>(block.x);
// 		block.y = static_cast<int>(block.y);

// 		characterSize *= map->GetBlockSize();

// 		auto charBlock = (block - map->GetChunkSize() / 2.0f) * map->GetBlockSize();

// 		for (int i = 0; i < times; ++i) {
// 			auto currentBlock = charBlock + Vec2(check.x, check.y) * map->GetBlockSize() + affect * static_cast<float>(i) * map->GetBlockSize();

// 			if (debugRenderer) {
// 				debugRenderer->AddQuad(currentBlock, currentBlock + map->GetBlockSize());
// 			}

// 			const auto& blocks = map->GetBlocks();

// 			bool positionalCondition;
// 			if (affect.x) {
// 				positionalCondition = reverse ? (GetPosition().y + leaveSpace + characterSize.y < currentBlock.y + map->GetBlockSize()) : (GetPosition().y + leaveSpace + characterSize.y > currentBlock.y + map->GetBlockSize());
// 			} else if (affect.y) {
// 				positionalCondition = reverse ? (GetPosition().x + leaveSpace + characterSize.x < currentBlock.x + map->GetBlockSize()) : (GetPosition().x + leaveSpace + characterSize.x > currentBlock.x + map->GetBlockSize());
// 			}

// 			if (positionalCondition && blocks[block.x + check.x + affect.x * i][block.y + check.y + affect.y * i] != BlockType::Empty) {
// 				if (correction.x) {
// 					SetPosition((currentBlock + map->GetBlockSize() - leaveSpace + correction.x - characterSize.x).x, GetPosition().y);
// 				} else if (correction.y) {
// 					if (!characterSize.y) {
// 						SetPosition(GetPosition().x, (currentBlock + map->GetBlockSize() - leaveSpace + correction.y).y);
// 					} else {
// 						SetPosition(GetPosition().x, (currentBlock - map->GetBlockSize() - leaveSpace + correction.y - characterSize.y).y);
// 					}
// 				}
// 				return false;
// 			}
// 		}

// 		return true;
// 	}

//   inline void Ceiling(bool value) { ceiling = value; }

//   inline void OnGround(bool value) { onGround = value; }
//   inline bool OnGround() const { return onGround; }

//   inline void CanMoveLeft(bool value) { canMoveLeft = value; }
//   inline void CanMoveRight(bool value) { canMoveRight = value; }
//   inline bool CanMoveLeft() const { return canMoveLeft; }
//   inline bool CanMoveRight() const { return canMoveRight; }

// private:
//   bool ceiling      { false };
//   bool onGround     { true };
//   bool canMoveLeft  { true };
//   bool canMoveRight { true };

//   float acceleration { 0 };

//   Vec2 position;
//   Mat4 modelMatrix;

//   inline void CalculateModelMatrix() {
//     modelMatrix = Math::Translate(Mat4(1), Vec3(position, 0.0));
//   }

//   Character(const Character&) = delete;
//   Character& operator=(const Character&) = delete;
// };