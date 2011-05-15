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
  Collidable(F springConstant, F dampeningConstant, F coefficientOfFriction, F rollingResistance)
    : mSpringConstant(springConstant),
      mDampeningConstant(dampeningConstant),
      mCoefficientOfFriction(coefficientOfFriction),
      mRollingResistance(rollingResistance)
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
	F coefficientOfFriction = sqrt(mCoefficientOfFriction * otherCollidable->mCoefficientOfFriction);
	//	F rollingResistance = sqrt(mRollingResistance * otherCollidable->mRollingResistance);
	ZenMatrix<F, 3, 1> normalVel = normal * Dot(normal, Object<F>::mVelocity);
	ZenMatrix<F, 3, 1> otherNormalVel = normal * Dot(normal, other->mVelocity);
	ZenMatrix<F, 3, 1> relNormalVel = otherNormalVel - normalVel;
	ZenMatrix<F, 3, 1> rotationalVel = Cross(Object<F>::mAngularVelocity, normal * Object<F>::mRadius);
	ZenMatrix<F, 3, 1> otherRotationalVel = Cross(other->mAngularVelocity, normal * -other->mRadius);
	ZenMatrix<F, 3, 1> tangentVel = Object<F>::mVelocity - normalVel + rotationalVel;
	ZenMatrix<F, 3, 1> otherTangentVel = other->mVelocity - otherNormalVel + otherRotationalVel;
	ZenMatrix<F, 3, 1> relTangentVel = otherTangentVel - tangentVel;
	F springStrength = overlap * springConstant;
	F frictionStrength = coefficientOfFriction * springStrength;
	F relStoppingStrength = Length(relTangentVel) / timeslice / 2;
	ZenMatrix<F, 3, 1> tangentForce = Normalize(relTangentVel) * min(frictionStrength, relStoppingStrength);
	
	ZenMatrix<F, 3, 1> force = normal * -springStrength + relNormalVel * dampeningConstant + tangentForce;
	ApplyForce(force, timeslice);
	other->ApplyForce(-force, timeslice);

//	F rollingStrength = rollingResistance * springStrength / Object<F>::mRadius;
//	F otherRollingStrength = rollingResistance * springStrength / other->mRadius;
//	F stoppingStrength = Length(tangentVel) / timeslice;
//	F otherStoppingStrength = Length(otherTangentVel) / timeslice;
//	ZenMatrix<F, 3, 1> rollingForce = Normalize(tangentVel) * -min(rollingStrength, stoppingStrength);
//	ZenMatrix<F, 3, 1> otherRollingForce = Normalize(otherTangentVel) * -min(otherRollingStrength, otherStoppingStrength);
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
  F mRollingResistance;
};

#endif // COLLIDABLE_HH
