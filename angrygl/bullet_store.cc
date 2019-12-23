#include "angrygl/bullet_store.h"

#include <chrono>
#include <algorithm>
#include <iostream>
#include <thread>

#include "angrygl/geom.h"
#include "angrygl/aabb.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "angrygl/capsule.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace {

const Capsule BULLET_COLLIDER(0.3f, 0.03f);

const float pi = (float)M_PI;
const float rotPerBullet = 3.0f * pi / 180.0f;

glm::quat partialHamiltonProduct2(const glm::quat& q1, const glm::vec3& q2 /*partial*/) {
  return glm::quat(
    - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
    q1.w * q2.x + q1.y * q2.z - q1.z * q2.y,
    q1.w * q2.y - q1.x * q2.z + q1.z * q2.x,
    q1.w * q2.z + q1.x * q2.y - q1.y * q2.x);
}

glm::vec3 partialHamiltonProduct(const glm::quat& q1, const glm::quat& q2) {
  return glm::vec3(
    q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
    q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
    q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w);
}

glm::vec3 rotateByQuat(const glm::vec3& v, const glm::quat& q) {
  const glm::quat qPrime = glm::quat(q.w, -q.x, -q.y, -q.z);
  return partialHamiltonProduct(partialHamiltonProduct2(q, v), qPrime);
}

void logTimeSince(const std::string& label, std::chrono::time_point<std::chrono::high_resolution_clock> start) {
  const auto x = std::chrono::high_resolution_clock::now();
  const auto y = std::chrono::duration_cast<std::chrono::milliseconds>(x - start).count();
  std::cout << label << y << "ms" << std::endl;
}

const float bulletScale = 0.3f;
const float bulletLifetime = 1.0f; // seconds
const glm::vec3 scaleVec(bulletScale, bulletScale, bulletScale);
const float bulletSpeed = 15.0f; // Game units per second
const glm::vec3 bulletNormal(0.0f, 1.0f, 0.0f);
const glm::vec3 canonicalDir(0.0f, 0.0f, 1.0f);

// TODO double sided?
const float bulletVertices[] = {
    // Positions                                            // Tex Coords
    bulletScale * (-0.243f), 0.0f, bulletScale * (-0.5f),   1.0f, 0.0f,
    bulletScale * (-0.243f), 0.0f, bulletScale * 0.5f,      0.0f, 0.0f,
    bulletScale * 0.243f,  0.0f, bulletScale * 0.5f,        0.0f, 1.0f,
    bulletScale * 0.243f, 0.0f, bulletScale * (-0.5f),      1.0f, 1.0f
};
const unsigned int bulletIndices[] = {
    0, 1, 2,
    0, 2, 3
};

const float bulletEnemyMaxCollisionDist =
    BULLET_COLLIDER.height / 2 + BULLET_COLLIDER.radius +
    ENEMY_COLLIDER.height / 2 + ENEMY_COLLIDER.radius;
const float bulletEnemyMaxCollisionDist2 = bulletEnemyMaxCollisionDist * bulletEnemyMaxCollisionDist;

bool bulletCollidesWithEnemy(const glm::vec3& bPos, const glm::vec3& bDir, const Enemy& e) {
  if (glm::distance2(bPos, e.position) > bulletEnemyMaxCollisionDist2) {
    return false;
  }
  const float closestDist =
    distanceBetweenLineSegments(
        bPos - bDir * (BULLET_COLLIDER.height / 2),
        bPos + bDir * (BULLET_COLLIDER.height / 2),
        e.position - e.dir * (ENEMY_COLLIDER.height / 2),
        e.position + e.dir * (ENEMY_COLLIDER.height / 2));
  return closestDist <= (BULLET_COLLIDER.radius + ENEMY_COLLIDER.radius);
}

}  // namespace

//static
BulletStore BulletStore::initialiseBuffersAndCreate(ThreadPool* const threadPool) {
  unsigned int bulletVAO;
  glGenVertexArrays(1, &bulletVAO);
  unsigned int bulletVBO;
  glGenBuffers(1, &bulletVBO);
  unsigned int bulletEBO;
  glGenBuffers(1, &bulletEBO);
  glBindVertexArray(bulletVAO);
  glBindBuffer(GL_ARRAY_BUFFER, bulletVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(bulletVertices), bulletVertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bulletEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bulletIndices), bulletIndices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  unsigned int instanceVBO;
  glGenBuffers(1, &instanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::quat), (void*)0);
  glVertexAttribDivisor(2, 1);

  unsigned int offsetVBO;
  glGenBuffers(1, &offsetVBO);
  glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
  glVertexAttribDivisor(3, 1);

  return std::move(BulletStore(threadPool, bulletVAO, instanceVBO, offsetVBO));
}

void BulletStore::createBullets(const glm::vec3& position, const glm::vec3& midDir, const int spreadAmount) {
  const glm::vec3 normalizedDir = glm::normalize(midDir);

  glm::quat midDirQuat(1.0f, 0.0f, 0.0f, 0.0f);
  // TODO there's probably a more efficient way to calculate this...
  {
    const glm::vec3 rotVec(0.0, 1.0f, 0.0f);
    const glm::vec3 x = glm::normalize(glm::vec3(canonicalDir.x, 0.0f, canonicalDir.z));
    const glm::vec3 y = glm::normalize(glm::vec3(normalizedDir.x, 0.0f, normalizedDir.z));
    const float theta = glm::orientedAngle(x, y, rotVec);
    midDirQuat = glm::rotate(midDirQuat, theta, rotVec);
  }

  const int startIndex = allBulletPositions.size();
  const int bulletGroupSize = spreadAmount * spreadAmount;
  BulletGroup g(startIndex, bulletGroupSize, bulletLifetime);
  allBulletPositions.resize(startIndex + bulletGroupSize);
  allQuats.resize(startIndex + bulletGroupSize);
  allBulletDirs.resize(startIndex + bulletGroupSize);
  const int parallelism = threadPool->numWorkers();
  const int workerGroupSize = spreadAmount / parallelism;
  std::vector<std::future<void>> futures;
  for (int p = 0; p < parallelism; ++p) {
    const int iStart = p * workerGroupSize;
    const int iEnd = p == (parallelism - 1) ? spreadAmount : iStart + workerGroupSize;
    futures.emplace_back(threadPool->enqueue([this, &position, &midDirQuat, spreadAmount, startIndex, &g, iStart, iEnd]() {
      for (int i = iStart; i < iEnd; ++i) {
        const glm::quat yQuat = glm::rotate(
            midDirQuat,
            rotPerBullet * (i - spreadAmount / 2),
            glm::vec3(0.0f, 1.0f, 0.0f));
        for (int j = 0; j < spreadAmount; ++j) {
          const glm::quat rotQuat = glm::rotate(
              yQuat,
              rotPerBullet * (j - spreadAmount / 2),
              glm::vec3(1.0f, 0.0f, 0.0f));
          const glm::vec3 dir = rotateByQuat(canonicalDir, rotQuat);
          const int pos = i * spreadAmount + j + startIndex;
          allBulletPositions[pos] = position;
          allBulletDirs[pos] = dir;
          allQuats[pos] = rotQuat;
        }
      }
    }));
  }
  for (auto& future : futures) {
    future.get();
  }
  bulletGroups.push_back(g);
}

void BulletStore::updateBullets(float deltaTimeSeconds, std::vector<Enemy>* enemies, std::vector<SpritesheetSprite>* enemyDeathSprites) {
  // Bullet groups are divided into subgroups, which are excluded en masse from
  // enemy collision detection.
  const int numSubGroups = 9;

  // TODO get enemy subgroups working
  const int numEnemySubGroups = std::min((size_t)1, enemies->size());
  const int enemiesPerGroup = numEnemySubGroups == 0 ? 0 : enemies->size() / numEnemySubGroups;
  std::vector<AABB> enemySubGroupAABBs(numEnemySubGroups);
  for (int a = 0; a < numEnemySubGroups; ++a) {
    for (int b = a * enemiesPerGroup; b < (enemiesPerGroup * a + enemiesPerGroup); ++b) {
      enemySubGroupAABBs[a].expandToInclude((*enemies)[b].position);
    }
  }

  const float deltaPos = deltaTimeSeconds * bulletSpeed;
  int firstLiveGroup = 0;
  int deadEnemiesStart = enemies->size();
  std::vector<bool> enemyDeathMarker(enemies->size());
  std::vector<std::future<void>> futures;
  for (BulletGroup& g : bulletGroups) {
    g.TTL -= deltaTimeSeconds;
    if (g.TTL <= 0.0f) {
      firstLiveGroup++;
    } else {
      futures.emplace_back(threadPool->enqueue([this, &enemySubGroupAABBs, enemiesPerGroup, numSubGroups, enemyDeathSprites, deltaPos, &enemies, &g, &enemyDeathMarker]() {
        const int bulletGroupStartIdx = g.startIndex;
        const int numBulletsInGroup = g.groupSize;
        const int subgroupSize = numBulletsInGroup / numSubGroups;
        const bool useAABB = enemies->size() > 0;
        for (int subgroup = 0; subgroup < numSubGroups; ++subgroup) {
          int bulletsStart = subgroupSize * subgroup;
          int bulletsEnd = (subgroup == (numSubGroups - 1)) ? numBulletsInGroup : (bulletsStart + subgroupSize);
          bulletsStart += bulletGroupStartIdx;
          bulletsEnd += bulletGroupStartIdx;

          // Bullet subgroup AABB
          AABB subgroupBoundingBox;
          for (int bulletIdx = bulletsStart; bulletIdx < bulletsEnd; ++bulletIdx) {
            allBulletPositions[bulletIdx] += allBulletDirs[bulletIdx] * deltaPos;
          }
          if (useAABB) {
            for (int bulletIdx = bulletsStart; bulletIdx < bulletsEnd; ++bulletIdx) {
              subgroupBoundingBox.expandToInclude(allBulletPositions[bulletIdx]);
            }
            subgroupBoundingBox.expandBy(bulletEnemyMaxCollisionDist);
          }

          for (int s = 0; s < enemySubGroupAABBs.size(); ++s) {
            if (!AABBsIntersect(enemySubGroupAABBs[s], subgroupBoundingBox)) {
              //continue;
            }
            for (int i = s * enemiesPerGroup; i < (s * enemiesPerGroup + enemiesPerGroup); ++i) {
              const Enemy& e = enemies->at(i);
              if (useAABB && !subgroupBoundingBox.containsPoint(e.position)) {
                continue;
              }
              for (int bulletIdx = bulletsStart; bulletIdx < bulletsEnd; ++bulletIdx) {
                if (bulletCollidesWithEnemy(allBulletPositions[bulletIdx], allBulletDirs[bulletIdx], e)) {
                  // TODO kill bullet too?
                  enemyDeathSprites->emplace_back(e.position);
                  enemyDeathMarker[i] = true;
                  break;
                }
              }
            }
          }
        }
      }));
    }
  }
  for (auto& future : futures) {
    future.get();
  }
  int firstLivingBullet = 0;
  if (firstLiveGroup != 0) {
    firstLivingBullet = bulletGroups[firstLiveGroup - 1].startIndex + bulletGroups[firstLiveGroup - 1].groupSize;
    bulletGroups.erase(bulletGroups.begin(), bulletGroups.begin() + firstLiveGroup);
  }
  if (firstLivingBullet != 0) {
    allBulletPositions.erase(allBulletPositions.begin(), allBulletPositions.begin() + firstLivingBullet);
    allQuats.erase(allQuats.begin(), allQuats.begin() + firstLivingBullet);
    allBulletDirs.erase(allBulletDirs.begin(), allBulletDirs.begin() + firstLivingBullet);
    for (BulletGroup& g : bulletGroups) {
      g.startIndex -= firstLivingBullet;
    }
  }
  for (int i = (enemies->size() - 1); i >= 0; --i) {
    if (enemyDeathMarker[i]) {
      enemies->erase(enemies->begin() + i);
    }
  }
}

void BulletStore::renderBulletSprites(bool isMeasuredFrame, unsigned int shaderId) {
  if (bulletGroups.size() == 0) {
    return;
  }
  const auto frameStart = std::chrono::high_resolution_clock::now();

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::quat) * allQuats.size(), &allQuats[0], GL_STREAM_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * allBulletPositions.size(), &allBulletPositions[0], GL_STREAM_DRAW);
  glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, allBulletPositions.size());

  if (isMeasuredFrame) {
    logTimeSince("    matrix draw call complete: ", frameStart);
  }
}
