#pragma once

namespace {
  enum class State {
    Idle = 0,
    Pressed,
    Released
  };
}

using KeyState    = State;
using ButtonState = State;