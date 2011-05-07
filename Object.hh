#ifndef OBJECT_HH
#define OBJECT_HH

#include "ZenMatrix.hh"

#define RANDOM 0.01
#define DAMPEN 0.9
#define DAMPEN_SPEED 0.001
#define FRICTION 1.0
#define FRICTION_LIMIT 0.05

template<class F>
class Object
{
public:

  Object()
  {
    mMass = 1;
    mRadius = 1;
    mMomentOfInertia = 1;
    mOrientation(0, 0) = 1;
  }

  void SetSize(F mass, F radius)
  {
    mMass = mass;
    mRadius = radius;
    mMomentOfInertia = mMass * mRadius * mRadius * 2.0 / 5.0;
  }

  void ApplyAngularVelocity(F timeslice)
  {
    mOrientation = Rotate(mOrientation, mAngularVelocity * timeslice);
  }

  void ApplyVelocity(F timeslice)
  {
    mLocation += mVelocity * timeslice;
  }  

  void ApplyForce(ZenMatrix<F, 3, 1> force, F timeslice)
  {
    mVelocity += force * (timeslice / mMass);
  }

  void ApplyTorque(ZenMatrix<F, 3, 1> torque, F timeslice)
  {
    mAngularVelocity += torque * (timeslice / mMomentOfInertia);
  }

  void DampenAngularVelocity(F timeslice)
  {
    mAngularVelocity *= pow(DAMPEN, timeslice / DAMPEN_SPEED);
//    F angularSpeed = Length(mAngularVelocity);
//    if (angularSpeed > FRICTION_LIMIT) {
//      if (timeslice * FRICTION / angularSpeed > 1.0) {
//	mAngularVelocity *= 0;
//      } else {
//	mAngularVelocity *= (1.0 - timeslice * FRICTION / angularSpeed);
//      }
//    }
  }

  void RandomizeAngularVelocity(F timeslice)
  {
    mAngularVelocity += ZenMatrix<F, 3, 1>::GetRandom(RANDOM * timeslice);
  }

  void Interact(Object<F>* other, F timeslice)
  {
    InteractMagnetically(other, timeslice);
  }

  ZenMatrix<F, 3, 1> mLocation;
  ZenMatrix<F, 3, 1> mVelocity;
  ZenMatrix<F, 3, 1> mOrientation;
  ZenMatrix<F, 3, 1> mAngularVelocity;
  F mMass;
  F mMomentOfInertia;
  F mRadius;

protected:
  virtual void InteractMagnetically(Object<F>* other, F timeslice) {}
};

template<class F>
std::ostream& operator<<(std::ostream& out, const Object<F>& object)
{
  return out << "( " << object.mLocation << " " << object.mOrientation << " )";
}

#endif // OBJECT_HH