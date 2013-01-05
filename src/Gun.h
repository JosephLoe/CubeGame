#include "GameAsset.h"
#include "IInterpolator.h"

#ifndef Gun_H_
#define Gun_H_

class Gun : public GameAsset {
 public:
  Gun();
  Gun(float x, float y, float z);
  ~Gun();
  //move(std::<shared_ptr> a, int b);

  virtual void update();
  virtual void draw();
  void setInterpolator(shared_ptr<IInterpolator> li);
 private:
  shared_ptr<IInterpolator> li;
  shared_ptr<Point3>        pos;
};

#endif // Gun_H_
