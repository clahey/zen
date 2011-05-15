#ifndef COLLIDABLE_HH
#define COLLIDABLE_HH

#include "Object.hh"
#include <exception>
#include <algorithm>

template<class F>
class Collidable:
  virtual public Object<F>
{
public:
  Collidable(F springConstant, F dampeningConstant, F coefficientOfFriction)
    : mSpringConstant(springConstant),
      mDampeningConstant(dampeningConstant),
      mCoefficientOfFriction(coefficientOfFriction)
  {
  }

protected:
  void
  InteractPhysically(Object<F>* other, F timeslice)
  {
    Collidable<F>* otherCollidable = dynamic_cast<Collidable<F>*>(other);
    if (otherCollidable != NULL) {
      ZenMatrix<F, 3, 1> location;
      ZenMatrix<F, 3, 1> normal;
      F overlap;
      bool collides;
      try {
	collides = Collides(otherCollidable, location, normal, overlap);
      } catch(...) {
	collides = otherCollidable->Collides(this, location, normal, overlap);
	normal = -normal;
      }
      if (collides) {
	F springConstant = (mSpringConstant + otherCollidable->mSpringConstant) / 2;
	F dampeningConstant = (mDampeningConstant + otherCollidable->mDampeningConstant) / 2;
	F coefficientOfFriction = (mCoefficientOfFriction + otherCollidable->mCoefficientOfFriction) / 2;
	ZenMatrix<F, 3, 1> force;
	ZenMatrix<F, 3, 1> relVel = other->mVelocity - Object<F>::mVelocity;
	ZenMatrix<F, 3, 1> normalVel = normal * Dot(normal, relVel);
	ZenMatrix<F, 3, 1> tangentVel = relVel - normalVel;
	tangentVel += Cross(other->mAngularVelocity, normal * -other->mRadius);
	tangentVel -= Cross(Object<F>::mAngularVelocity, normal * Object<F>::mRadius);
	F springStrength = -overlap * springConstant;
	F frictionStrength = coefficientOfFriction * -springStrength;
	F stoppingStrength = Length(tangentVel) / timeslice / 2;
	ZenMatrix<F, 3, 1> tangentForce = Normalize(tangentVel) * min(frictionStrength, stoppingStrength);
	
	force = normal * springStrength + normalVel * dampeningConstant + tangentForce;
	ApplyForce(force, timeslice);
	other->ApplyForce(-force, timeslice);
	
	ApplyTorque(Cross(location - Object<F>::mLocation, tangentForce), timeslice);
	other->ApplyTorque(Cross(location - other->mLocation, -tangentForce), timeslice);
      }
    }
  }

  virtual bool
  Collides(Collidable<F>* other, ZenMatrix<F, 3, 1>& location, ZenMatrix<F, 3, 1>& normal, F& overlap)
  {
    throw std::exception();
  }

private:
  F mSpringConstant;
  F mDampeningConstant;
  F mCoefficientOfFriction;
};

#endif // COLLIDABLE_HH
