#include "angrygl/enemy_spawner.h"

#include <cstdlib>

namespace {

const float enemySpawnInterval = 1.0f; // seconds
const int spawnsPerInterval = 1;
const float spawnRadius = 10.0f;  // from player

} // namespace

EnemySpawner::EnemySpawner(float _monsterY, std::vector<Enemy>* _enemies) : enemies(_enemies), countdown(spawnsPerInterval), monsterY(_monsterY) {}

void EnemySpawner::update(const glm::vec3& playerPos, float deltaTimeSeconds) {
  countdown -= deltaTimeSeconds;
  if (countdown <= 0.0f) {
    for (int i = 0; i < spawnsPerInterval; ++i) {
      spawnEnemy(playerPos);
    }
    countdown += enemySpawnInterval;
  }
}

void EnemySpawner::spawnEnemy(const glm::vec3& playerPos) {
  const float theta = glm::radians((float)(rand() % 360));
  const float x = playerPos.x + sin(theta) * spawnRadius;
  const float z = playerPos.z + cos(theta) * spawnRadius;
  enemies->emplace_back(glm::vec3(x, monsterY, z), glm::vec3(0.0f, 0.0f, 1.0f));
}
