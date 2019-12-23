#ifndef _SD_ANG_BULLET_STORE_H_
#define _SD_ANG_BULLET_STORE_H_

#include <vector>

#include "angrygl/spritesheet.h"
#include "angrygl/enemy.h"
#include "glm/glm.hpp"
#include "lib/ThreadPool.h"

class BulletStore {
public:
  static BulletStore initialiseBuffersAndCreate(ThreadPool* const threadPool);

  void createBullets(const glm::vec3& position, const glm::vec3& midDir, const int spreadAmount);

  void updateBullets(float deltaTimeSeconds, std::vector<Enemy>* enemies, std::vector<SpritesheetSprite>* enemyDeathSprites);

  void renderBulletSprites(bool isMeasuredFrame, unsigned int shaderId);
 private:
  std::vector<glm::vec3> allBulletPositions;
  std::vector<glm::quat> allQuats;
  std::vector<glm::vec3> allBulletDirs;

  struct BulletGroup {
    int startIndex;
    int groupSize;
    float TTL;

    BulletGroup(int _startIndex, int _groupSize, float lifetime)
    {
      TTL = lifetime;
      startIndex = _startIndex;
      groupSize = _groupSize;
    }
  };
  BulletStore(ThreadPool* const _threadPool, unsigned int _VAO, unsigned int _instanceVBO, unsigned int _offsetVBO)
    : threadPool(_threadPool), VAO(_VAO), instanceVBO(_instanceVBO), offsetVBO(_offsetVBO) {}

  ThreadPool* const threadPool;
  const unsigned int VAO;
  const unsigned int instanceVBO;
  const unsigned int offsetVBO;
  // Must be ordered in increasing TTL
  std::vector<BulletGroup> bulletGroups;
};

#endif // _SD_ANG_BULLET_STORE_H_
