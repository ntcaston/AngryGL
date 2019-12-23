#ifndef _SD_AABB_H_
#define _SD_AABB_H_

#include <cfloat>

#include "glm/glm.hpp"

// Axis-aligned bounding box
struct AABB {
  float xMin = FLT_MAX;
  float xMax = FLT_MIN;
  float yMin = FLT_MAX;
  float yMax = FLT_MIN;
  float zMin = FLT_MAX;
  float zMax = FLT_MIN;

  void expandToInclude(const glm::vec3 v) {
    xMin = std::min(xMin, v.x);
    xMax = std::max(xMax, v.x);
    yMin = std::min(yMin, v.y);
    yMax = std::max(yMax, v.y);
    zMin = std::min(zMin, v.z);
    zMax = std::max(zMax, v.z);
    isInitialised = true;
  }

  void expandBy(const float f) {
    if (!isInitialised) return;
    xMin -= f;
    xMax += f;
    yMin -= f;
    yMax += f;
    zMin -= f;
    zMax += f;
  }

  bool containsPoint(const glm::vec3 point) const {
    return point.x >= xMin
      && point.x <= xMax
      && point.y >= yMin
      && point.y <= yMax
      && point.z >= zMin
      && point.z <= zMax;
  }

 private:
  bool isInitialised = false;
};

bool AABBsIntersect(const AABB& a, const AABB& b) {
  return a.containsPoint(glm::vec3(b.xMin, b.yMin, b.zMin))
    || a.containsPoint(glm::vec3(b.xMin, b.yMin, b.zMax))
    || a.containsPoint(glm::vec3(b.xMin, b.yMax, b.zMin))
    || a.containsPoint(glm::vec3(b.xMin, b.yMax, b.zMax))
    || a.containsPoint(glm::vec3(b.xMax, b.yMin, b.zMin))
    || a.containsPoint(glm::vec3(b.xMax, b.yMin, b.zMax))
    || a.containsPoint(glm::vec3(b.xMax, b.yMax, b.zMin))
    || a.containsPoint(glm::vec3(b.xMax, b.yMax, b.zMax));
}

#endif  // _SD_AABB_H_
