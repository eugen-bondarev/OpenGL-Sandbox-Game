#include "Character.h"

Character::Character(const Ref<Map>& map) : map { map } {}

Blocks::Representations::IndexAndPosition Character::GetBlockNearby(Vec2 fix, Vec2 offset) const {
  Blocks::Representations::IndexAndPosition block;
  block.worldPosition = ToInt((position + fix + offset * map->GetBlockSize()) / map->GetBlockSize()) * map->GetBlockSize() - 8.0f;
  block.index = (block.worldPosition + 8.0f) / map->GetBlockSize();
  return block;    
}

void Character::CollectLights(std::vector<Vec2>& lights) const {
  FORGIO_PROFILER_SCOPE();
  
  const auto& blocks = map->GetBlocks();
  const auto& walls = map->GetWalls();

  for (int x = 0; x < 3; x++) {
    for (int y = 0; y < 3; y++) {
      auto& block = GetBlockNearby(Vec2(), Vec2(1 + x, 2 + y * 2));
      if (blocks[block.index.x][block.index.y] == BlockType::Empty && walls[block.index.x][block.index.y] == WallType::Empty) {
        lights.push_back(block.worldPosition);
      }
    }
  }
}

static constexpr bool RENDER_COLLIDERS = false;

void Character::CheckCollisions() {
  FORGIO_PROFILER_SCOPE();

  auto& blocks = map->GetBlocks();

  onGround = false;
  ceiling = false;
  canMoveLeft = true;
  canMoveRight = true;
  
  for (int i = 2; i < 4; i++) {
    Blocks::Representations::IndexAndPosition block = GetBlockNearby(Vec2(-10.0f, 10.0f), Vec2(i, 0));

    if (RENDER_COLLIDERS)
      Linow::AddQuad(block.worldPosition, block.worldPosition + map->GetBlockSize());

    if (blocks[block.index.x][block.index.y] != BlockType::Empty) {
      SetPositionY(block.worldPosition.y + map->GetBlockSize() - 4.0f);

      if (velocity.y < 0) {
        // We don't want to test if we fell through a couple of blocks, if velocity is low.
        int amountOfBlocksToTest = std::max<float>(sqrt(abs(velocity.y)) - 25.0f, 0.0f);
        for (int j = amountOfBlocksToTest; j >= 1; j--) {
          if (blocks[block.index.x][block.index.y + j] != BlockType::Empty) {
            SetPositionY(block.worldPosition.y + map->GetBlockSize() * j + map->GetBlockSize() - 4.0f);
            break;
          }
        }
      }

      onGround = true;
      break;
    }
  }

  for (int i = 2; i < 4; i++) {
    Blocks::Representations::IndexAndPosition block = GetBlockNearby(Vec2(-10.0f, -10.0f), Vec2(i, 6));

    if (RENDER_COLLIDERS)
      Linow::AddQuad(block.worldPosition, block.worldPosition + map->GetBlockSize());

    if (blocks[block.index.x][block.index.y - 1] != BlockType::Empty) {
      SetPositionY(block.worldPosition.y - 5 * map->GetBlockSize());
      ceiling = true;
      break;
    }
  }

  for (int i = 1; i < 5; i++) {
    Blocks::Representations::IndexAndPosition block = GetBlockNearby(Vec2(-2.0f, 10.0f), Vec2(1, i));

    if (RENDER_COLLIDERS)
      Linow::AddQuad(block.worldPosition, block.worldPosition + map->GetBlockSize());

    if (blocks[block.index.x][block.index.y] != BlockType::Empty) {
      canMoveLeft = false;
      break;
    }
  }

  for (int i = 1; i < 5; i++) {
    Blocks::Representations::IndexAndPosition block = GetBlockNearby(Vec2(-2.0f, 10.0f), Vec2(3, i));

    if (RENDER_COLLIDERS)
      Linow::AddQuad(block.worldPosition, block.worldPosition + map->GetBlockSize());

    if (blocks[block.index.x][block.index.y] != BlockType::Empty) {
      canMoveRight = false;
      break;
    }
  }
}

void Character::Update(float deltaTime) {
  FORGIO_PROFILER_SCOPE();

  if (!onGround) {
    SetPosition(position + velocity * Vec2(0, 1) * deltaTime);
    velocity += -Physics::g * deltaTime * velocityFactor;

    if (ceiling) {
      velocity.y = 0.0f;
      ceiling = false;
    }
  } else {
    velocity.y = 0.0f;
  }

  CheckCollisions();
}

void Character::Jump() {
  SetPositionY(position.y + map->GetBlockSize() - 4.0f - 10.0f);
  velocity = Vec2(0.0f, 4.0f * velocityFactor);
  onGround = false;
}

void Character::SetCeiling(bool value) { ceiling = value; }
void Character::SetOnGround(bool value) { onGround = value; }
void Character::CanMoveLeft(bool value) { canMoveLeft = value; }
void Character::CanMoveRight(bool value) { canMoveRight = value; }

bool Character::GetCeiling() const { return ceiling; }
bool Character::GetOnGround() const { return onGround; }
bool Character::CanMoveLeft() const { return canMoveLeft; }
bool Character::CanMoveRight() const { return canMoveRight; }