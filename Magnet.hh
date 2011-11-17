#ifndef MAGNET_HH
#define MAGNET_HH

#include "Object.hh"
#include <math.h>

#define muNaughtOverPi (4e-7)

template<class F>
class Magnet :
  virtual public Object<F>
{
public:
  Magnet(F strength)
    : mStrength(strength)
  {
  }

  void ApplyMagneticField(const ZenMatrix<F, 3, 1>& field, F timeslice)
  {
    //    ApplyTorque(Cross(Object<F>::mOrientation * mStrength, field), timeslice );
    // Optimize
    ApplyTorque(Cross(Object<F>::GetOrientation(), field), timeslice * mStrength);
  }

  ZenMatrix<F, 3, 1> GetMagneticField(const ZenMatrix<F, 3, 1>& location)
  {
    ZenMatrix<F, 3, 1> r = location - Object<F>::mLocation;
    F distance = Length(r);
    return (r * (3 * Dot(Object<F>::GetOrientation(), r) / pow(distance, 5.0)) - Object<F>::GetOrientation() / pow(distance, 3.0))
      * mStrength * (muNaughtOverPi / 4.0);
    // Optimize
//    ZenMatrix<F, 3, 1> retval;
//    retval = r;
//    retval *= (3 * Dot(GetOrientation(), r) / pow(distance, 2.0));
//    retval -= GetOrientation();
//    retval /= pow(distance, 3.0);
//    return retval;
  }

protected:
  virtual void InteractMagnetically(Object<F>* other, F timeslice)
  {
    Magnet<F>* otherMagnet = dynamic_cast< Magnet<F>*>(other);
    if (otherMagnet != NULL) {
      ApplyMagneticField(otherMagnet->GetMagneticField(Object<F>::mLocation), timeslice);
      otherMagnet->ApplyMagneticField(GetMagneticField(otherMagnet->Object<F>::mLocation), timeslice);
      ZenMatrix<F, 3, 1> r = other->mLocation - Object<F>::mLocation;
      F length = Length(r);
      const ZenMatrix<F, 3, 1>& orientation = Object<F>::GetOrientation();
      const ZenMatrix<F, 3, 1>& otherOrientation = otherMagnet->GetOrientation();
      
      ZenMatrix<F, 3, 1> force = (otherOrientation * Dot(orientation, r) +
				  orientation * Dot(otherOrientation, r) +
				  r * (Dot(orientation, otherOrientation) -
				       5 * Dot(orientation, r) * Dot(otherOrientation, r) / pow(length, 2))) *
	(3 * mStrength * otherMagnet->mStrength * muNaughtOverPi / 4 / pow(length, 5));
      ApplyForce(-force, timeslice);
      other->ApplyForce(force, timeslice);
    }
  }

private:
  F mStrength;
};

#endif // MAGNET_HH
