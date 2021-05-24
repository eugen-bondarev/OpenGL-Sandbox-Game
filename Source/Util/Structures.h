#pragma once

struct Size {
    float width;
    float height;
};

template <typename T = int>
struct Period {
    T start;
    T end;
};