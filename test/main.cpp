#include "../src/hlbvh.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <vector>
#include <math.h>

/* Define a fixed array of points for testing
  -  The array should be large enought to test for kNN,
  -  and neighbour queries

  Start with a linearly spaced set of points in 3D space, only x is non-zero rest are zero
 */

constexpr int NUM_POINTS = 1000;
const std::vector<std::array<float, 3>> test_points = []() {
  std::vector<std::array<float, 3>> pts(NUM_POINTS);
  for (int i = 0; i < NUM_POINTS; ++i) {
    pts[i] = {static_cast<float>(i), 0.0f, 0.0f};  // Linearly spaced along x-axis
  }
  return pts;
}();

void main() {}