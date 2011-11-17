#ifndef OBJECT_HH
#define OBJECT_HH

#include <exception>
#include "ZenMatrix.hh"
#include "Quaternion.hh"

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
    : mRotation(1),
      mMass(1),
      mMomentOfInertia(1),
      mRadius(1),
      mImmobile(false),
      mOrientationValid(false)
  {
  }

  void SetSize(F mass, F radius)
  {
    mMass = mass;
    mRadius = radius;
    mMomentOfInertia = mMass * mRadius * mRadius * 2.0 / 5.0;
  }

  virtual void ApplyAngularVelocity(F timeslice)
  {
    if (mImmobile) {
      return;
    }
    F length = Length(mAngularVelocity);
    mRotation = Quaternion<F>(mAngularVelocity / length, length * timeslice) * mRotation;
    mOrientationValid = false;
  }

  void ApplyVelocity(F timeslice)
  {
    if (mImmobile) {
      return;
    }
    mLocation += mVelocity * timeslice;
  }

  void ApplyForce(ZenMatrix<F, 3, 1> force, F timeslice)
  {
    if (mImmobile) {
      return;
    }
    mVelocity += force * (timeslice / mMass);
  }

  void ApplyAcceleration(ZenMatrix<F, 3, 1> acceleration, F timeslice)
  {
    if (mImmobile) {
      return;
    }
    mVelocity += acceleration * timeslice;
  }

  void ApplyTorque(ZenMatrix<F, 3, 1> torque, F timeslice)
  {
    if (mImmobile) {
      return;
    }
    mAngularVelocity += torque * (timeslice / mMomentOfInertia);
  }

  void DampenAngularVelocity(F timeslice)
  {
    if (mImmobile) {
      return;
    }
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
    if (mImmobile) {
      return;
    }
    mAngularVelocity += ZenMatrix<F, 3, 1>::GetRandom(RANDOM * timeslice);
  }

  void Interact(Object<F>* other, F timeslice)
  {
    InteractMagnetically(other, timeslice);
    InteractPhysically(other, timeslice);
    try {
      InteractGravitationally(other, timeslice);
    } catch(...) {
      try {
	other->InteractGravitationally(this, timeslice);
      } catch(...) {}
    }
  }

  ZenMatrix<F, 3, 1> mLocation;
  ZenMatrix<F, 3, 1> mVelocity;
  Quaternion<F> mRotation;
  ZenMatrix<F, 3, 1> mAngularVelocity;
  F mMass;
  F mMomentOfInertia;
  F mRadius;

protected:
  virtual void InteractMagnetically(Object<F>* other, F timeslice) {}
  virtual void InteractPhysically(Object<F>* other, F timeslice) {}
  virtual void InteractGravitationally(Object<F>* other, F timeslice)
  {
    throw std::exception();
  }

  const ZenMatrix<F, 3, 1>& GetOrientation() {
    if (!mOrientationValid) {
      ZenMatrix<F, 3, 1> x;
      x(0, 0) = 1;
      mOrientation = mRotation.Rotate(x);
      mOrientationValid = true;
    }
    return mOrientation;
  }

  bool mImmobile;

private:
  ZenMatrix<F, 3, 1> mOrientation;
  bool mOrientationValid;
};

template<class F>
std::ostream& operator<<(std::ostream& out, const Object<F>& object)
{
  return out << "( " << object.mLocation << " " << object.mRotation << " )";
}

#endif // OBJECT_HH
