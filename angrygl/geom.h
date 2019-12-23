#ifndef _SD_ANG_GEOM_H_
#define _SD_ANG_GEOM_H_

#include "glm/glm.hpp"

inline float distanceBetweenPointAndLineSegment(
    const glm::vec3& point,
    const glm::vec3& a,
    const glm::vec3& b) {
  const glm::vec3 ab = b - a;
  const glm::vec3 ap = point - a;
  if (glm::dot(ap, ab) <= 0.0f) {
    return glm::length(ap);
  }
  const glm::vec3 bp = point - b;
  if (glm::dot(bp, ab) >= 0.0f) {
    return glm::length(bp);
  }
  return glm::length(glm::cross(ab, ap)) / glm::length(ab);
}

inline float distanceBetweenLineSegments(
    const glm::vec3& a0,
    const glm::vec3& a1,
    const glm::vec3& b0,
    const glm::vec3& b1) {
  const float EPS = 0.001f;

  const glm::vec3 A = a1 - a0;
  const glm::vec3 B = b1 - b0;
  const float magA = glm::length(A);
  const float magB = glm::length(B);

  const glm::vec3 _A = A / magA;
  const glm::vec3 _B = B / magB;

  const glm::vec3 cross = glm::cross(_A, _B);
  const float cl = glm::length(cross);
  const float denom = cl * cl;


  // If lines are parallel (denom=0) test if lines overlap.
  // If they don't overlap then there is a closest point solution.
  // If they do overlap, there are infinite closest positions, but there is a closest distance
  if (denom < EPS) {
    const float d0 = glm::dot(_A,(b0-a0));
    const float d1 = glm::dot(_A,(b1-a0));

    // Is segment B before A?
    if (d0 <= 0.0f && 0.0f >= d1) {
      if (abs(d0) < abs(d1)) {
        return glm::length(a0-b0);
      }
      return glm::length(a0-b1);
    } else if (d0 >= magA && magA <= d1) {
      if (abs(d0) < abs(d1)) {
        return glm::length(a1-b0);
      }
      return glm::length(a1-b1);
    }


    // Segments overlap, return distance between parallel segments
    return glm::length(((d0*_A)+a0)-b0);
  }



  // Lines criss-cross: Calculate the projected closest points
  const glm::vec3 t = (b0 - a0);
  const float detA = glm::determinant(glm::mat3(t, _B, cross));
  const float detB = glm::determinant(glm::mat3(t, _A, cross));

  const float t0 = detA/denom;
  const float t1 = detB/denom;

  glm::vec3 pA = a0 + (_A * t0); // Projected closest point on segment A
  glm::vec3 pB = b0 + (_B * t1); // Projected closest point on segment B

  // Clamp projections
  if (t0 < 0.0f) {
    pA = a0;
  } else if (t0 > magA) {
    pA = a1;
  }

  if (t1 < 0) {
    pB = b0;
  } else if (t1 > magB) {
    pB = b1;
  }

  // Clamp projection A
  if (t0 < 0.0f || t0 > magA) {
    float dot = glm::dot(_B,(pA-b0));
    if (dot < 0.0f) {
      dot = 0;
    } else if (dot > magB) {
      dot = magB;
    }
    pB = b0 + (_B * dot);
  }

  // Clamp projection B
  if (t1 < 0.0f || t1 > magB) {
    float dot = glm::dot(_A,(pB-a0));
    if (dot < 0.0f) {
      dot = 0;
    } else if (dot > magA) {
      dot = magA;
    }
    pA = a0 + (_A * dot);
  }

  return glm::length(pA-pB);
}


#endif  // _SD_ANG_GEOM_H_
