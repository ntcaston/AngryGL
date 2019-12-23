#ifndef _SD_ANG_CAPSULE_H_
#define _SD_ANG_CAPSULE_H_

struct Capsule {
  float height;
  float radius;

  Capsule(float _height, float _radius)
    : height(_height), radius(_radius) {}
};

#endif  // _SD_ANG_CAPSULE_H_
