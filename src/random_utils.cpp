#include <cstdlib>
#include <ctime>
#include "random_utils.h"

float random_utils::range(float low, float high) {
  return ((float)std::rand() / RAND_MAX) * (high - low) + low;
}

float random_utils::range(float high) { return range(0.0f, high); }
