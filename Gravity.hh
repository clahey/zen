#ifndef GRAVITY_HH
#define GRAVITY_HH

#include "Object.hh"
#include <exception>
#include <algorithm>

template<class F>
class Gravity:
  virtual public Object<F>
{
public:
  Gravity(F gravitationalConstant)
    : mGravitationalConstant(gravitationalConstant)
  {
    Object<F>::mImmobile = true;
  }

protected:
  void
  InteractGravitationally(Object<F>* other, F timeslice)
  {
    if (other->mMass == 0) {
      return;
    }
    other->ApplyAcceleration(Object<F>::GetOrientation() * mGravitationalConstant, timeslice);
  }

private:
  F mGravitationalConstant;
};

#endif // GRAVITY_HH
