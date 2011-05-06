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
  {}

  void ApplyMagneticField(const ZenMatrix<F, 3, 1>& field, F timeslice)
  {
    //    ApplyTorque(Cross(Object<F>::mOrientation * mStrength, field), timeslice );
    // Optimize
    ApplyTorque(Cross(Object<F>::mOrientation, field), timeslice * mStrength);
  }

  ZenMatrix<F, 3, 1> GetMagneticField(const ZenMatrix<F, 3, 1>& location)
  {
    ZenMatrix<F, 3, 1> r = location - Object<F>::mLocation;
    F distance = Length(r);
    return (r * (3 * Dot(Object<F>::mOrientation, r) / pow(distance, 5.0)) - Object<F>::mOrientation / pow(distance, 3.0))
      * mStrength * (muNaughtOverPi / 4.0);
    // Optimize
//    ZenMatrix<F, 3, 1> retval;
//    retval = r;
//    retval *= (3 * Dot(Object<F>::mOrientation, r) / pow(distance, 2.0));
//    retval -= Object<F>::mOrientation;
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
    }
  }

private:
  F mStrength;
};

#endif // MAGNET_HH
