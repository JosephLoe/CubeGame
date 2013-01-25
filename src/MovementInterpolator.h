#include <vectormath/scalar/cpp/vectormath_aos.h>

#include "IInterpolator.h"

using namespace Vectormath::Aos;

#ifndef MOVEMENTINTERPOLATOR_H_
#define MOVEMENTINTERPOLATOR_H_

class MovementInterpolator : public IInterpolator {
 public:
  MovementInterpolator(Vector3 moveVec);
  Vector3 update();
 private:
  Vector3 * movement;
};

#endif
