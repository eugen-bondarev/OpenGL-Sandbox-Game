#pragma once

#include <vector>

template <typename T>
inline static void JoinVectors(const std::vector<T>& a, const std::vector<T>& b, std::vector<T>& result) {
  for (int i = 0; i < a.size(); i++) {
    result.push_back(a[i]);
  }

  for (int i = 0; i < b.size(); i++) {
    result.push_back(b[i]);
  }
}