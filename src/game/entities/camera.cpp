#include "camera.h"

const Vec2& Camera::GetLastPosition() const {
  return lastPosition;
}

Vec2 Camera::GetPositionOnScreen(Vec2 worldPosition) const {    
  return Window::GetSize() / 2.0f - (position - worldPosition) * Vec2(1, -1);
}

void Camera::OnPositionChange(std::function<void()> Callback) {
  if (position != lastPosition) {
    lastPosition = position;
    Callback();
  }
}

void Camera::CalculateTransform() {
  transform = Math::Inverse(Math::Translate(Mat4(1), Vec3(position, 0.0f)));
}