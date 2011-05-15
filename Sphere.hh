#ifndef SPHERE_HH
#define SPHERE_HH

#include "Collidable.hh"

template<class F>
class Sphere:
  public Collidable<F>
{
public:
  Sphere(F springConstant, F dampeningConstant, F frictionMultiplier)
    : Collidable<F>(springConstant, dampeningConstant, frictionMultiplier)
  {
  }

protected:
  bool
  Collides(Collidable<F>* other, ZenMatrix<F, 3, 1>& location, ZenMatrix<F, 3, 1>& normal, F& overlap)
  {
    Sphere<F>* otherSphere = dynamic_cast<Sphere<F>*>(other);
    if (otherSphere != NULL) {
      ZenMatrix<F, 3, 1> r = other->mLocation - Object<F>::mLocation;
      F distance = Length(r);
      F overlapLocal = Object<F>::mRadius + other->mRadius - distance;
      if (overlapLocal > 0) {
	overlap = overlapLocal;
	normal = Normalize(r);
	location = (other->mLocation + Object<F>::mLocation) / 2;
	return true;
      } else {
	return false;
      }
    } else {
      throw std::exception();
    }
  }
};

#endif // SPHERE_HH
