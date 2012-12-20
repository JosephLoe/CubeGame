#include "GameAsset.h"
#include "IInterpolator.h"

#ifndef CubeASSET_H_
#define CubeASSET_H_

class CubeAsset : public GameAsset {
 public:
  CubeAsset();
  CubeAsset(float x, float y, float z);
  ~CubeAsset();
  //move(std::<shared_ptr> a, int b);

  virtual void update();
  virtual void draw();
  void setInterpolator(shared_ptr<IInterpolator> li);
 private:
  shared_ptr<IInterpolator> li;
  shared_ptr<Point3>        pos;
};

#endif // CubeASSET_H_
