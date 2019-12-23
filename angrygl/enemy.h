#ifndef _SD_ANG_ENEMY_H_
#define _SD_ANG_ENEMY_H_

#include "glm/glm.hpp"
#include "angrygl/capsule.h"

const Capsule ENEMY_COLLIDER(0.4f, 0.08f);

struct Enemy {
  glm::vec3 position;
  glm::vec3 dir;

  Enemy(glm::vec3 _position, glm::vec3 _dir)
    : position(std::move(_position)), dir(std::move(_dir)) {}
};

#endif  // _SD_ANG_ENEMY_H_
