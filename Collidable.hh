#ifndef COLLIDABLE_HH
#define COLLIDABLE_HH

#include "Object.hh"
#include <exception>

template<class F>
class Collidable:
  virtual public Object<F>
{
public:
  Collidable(F springConstant, F dampeningConstant, F frictionMultiplier)
    : mSpringConstant(springConstant),
      mDampeningConstant(dampeningConstant),
      mFrictionMultiplier(frictionMultiplier)
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
	F frictionMultiplier = (mFrictionMultiplier + otherCollidable->mFrictionMultiplier) / 2;
	ZenMatrix<F, 3, 1> force;
	ZenMatrix<F, 3, 1> relVel = other->mVelocity - Object<F>::mVelocity;
	ZenMatrix<F, 3, 1> normalVel = normal * Dot(normal, relVel);
	ZenMatrix<F, 3, 1> tangentVel = relVel - normalVel;
	tangentVel += Cross(other->mAngularVelocity, normal * -other->mRadius);
	tangentVel -= Cross(Object<F>::mAngularVelocity, normal * Object<F>::mRadius);
	ZenMatrix<F, 3, 1> tangentForce = tangentVel * frictionMultiplier;
	force = normal * (-overlap * springConstant) + normalVel * dampeningConstant + tangentForce;
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
  F mFrictionMultiplier;
};

#endif // COLLIDABLE_HH
