#include "GameAsset.h"
#include "IInterpolator.h"

#ifndef CubeASSET_H_
#define CubeASSET_H_

class CubeAsset : public GameAsset {
 public:
  CubeAsset();
  CubeAsset(float x, float y, float z, float size, string shader);
  ~CubeAsset();
  //move(std::<shared_ptr> a, int b);

  //virtual void move(float mx, float my, float mz);

  virtual void update();
  virtual void draw();
  void setInterpolator(shared_ptr<IInterpolator> li);
 private:
  shared_ptr<IInterpolator> li;
  shared_ptr<Point3>        pos;
  shared_ptr<Vector3>		vecPos;
};

#endif // CubeASSET_H_
