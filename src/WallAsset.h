#include "GameAsset.h"
#include "IInterpolator.h"

#ifndef WallASSET_H_
#define WallASSET_H_

class WallAsset : public GameAsset {
 public:
  WallAsset();
  WallAsset(float x, float y, float z, float wallX, float wallY, float wallZ, string(side),string(shader));
  ~WallAsset();
  //move(std::<shared_ptr> a, int b);

  //virtual void move(float mx, float my, float mz);

  virtual void update();
  virtual void draw();
  void setInterpolator(shared_ptr<IInterpolator> li);
 private:
  shared_ptr<IInterpolator> li;
  shared_ptr<Point3>        pos;
};

#endif // WallASSET_H_
