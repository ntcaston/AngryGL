#ifndef _SD_ANG_ENEMY_SPAWNER_H_
#define _SD_ANG_ENEMY_SPAWNER_H_

#include <vector>

#include "angrygl/enemy.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class EnemySpawner {
 public:
   EnemySpawner(float _monsterY, std::vector<Enemy>* _enemies);

   void update(const glm::vec3& playerPos, float deltaTimeSeconds);

 private:
   void spawnEnemy(const glm::vec3& playerPos);

   // not owned
   std::vector<Enemy>* enemies;
   float countdown;
   const float monsterY;
};

#endif  // _SD_ANG_ENEMY_SPAWNER_H_
