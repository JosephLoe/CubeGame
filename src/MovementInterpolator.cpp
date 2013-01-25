#include "MovementInterpolator.h"

MovementInterpolator::MovementInterpolator(Vector3 moveVec) {
  this->movement = new Vector3(moveVec);
  //this->G = new Vector3(0.0f, -9.8f, 0.0f);
  //this->G = new Vector3(0.0f, -9.8f, 0.0f);
}

Vector3 MovementInterpolator::update() {
  return (*movement);
  //return ((*launch)*time) + ((*G) * ((1.0f/2.0f) * pow(time, 2.0f)));
}
