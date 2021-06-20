#pragma once

#include "Maths/Maths.h"

#include "Core/Window.h"

#include "../ITransform.h"

class Camera : public ITransform {
public:
  Camera() = default;

  inline const Vec2& GetLastPosition() const {
    return lastPosition;
  }

  template <typename... Args>
  inline void SetLastPosition(Args... args) {
    lastPosition = Vec2(std::forward<Args>(args)...);
  }

  inline Vec2 GetPositionOnScreen(Vec2 worldPosition) const {    
		return Window::GetSize() / 2.0f - (position - worldPosition) * Vec2(1, -1);
  }

  inline void OnPositionChange(std::function<void()> callback) {
    if (position != lastPosition) {
      lastPosition = position;
      callback();
    }
  }

private:
  Vec2 lastPosition; 

  inline virtual void CalculateTransform() override {
	  transform = Math::Inverse(Math::Translate(Mat4(1), Vec3(position, 0.0f)));
  }
};