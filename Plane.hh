#ifndef PLANE_HH
#define PLANE_HH

#include "Collidable.hh"
#include "Sphere.hh"

template<class F>
class Plane:
  public Collidable<F>
{
public:
  Plane(F springConstant, F dampeningConstant, F frictionMultiplier, F rollingResistance)
    : Collidable<F>(springConstant, dampeningConstant, frictionMultiplier, rollingResistance)
  {
  }

protected:
  bool
  Collides(Collidable<F>* other, ZenMatrix<F, 3, 1>& location, ZenMatrix<F, 3, 1>& normal, F& overlap)
  {
    Plane<F>* otherPlane = dynamic_cast<Plane<F>*>(other);
    Sphere<F>* otherSphere = dynamic_cast<Sphere<F>*>(other);
    if (otherPlane != NULL) {
      return false;
    } else if (otherSphere != NULL) {
      ZenMatrix<F, 3, 1> orientation = Object<F>::GetOrientation();
      F offset = Dot(orientation, other->mLocation - Object<F>::mLocation);
      F overlapLocal = otherSphere->mRadius - offset;
      if (overlapLocal > 0) {
	overlap = overlapLocal;
	normal = orientation;
	location = other->mLocation - normal * offset;
	return true;
      } else {
	return false;
      }
    } else {
      throw std::exception();
    }
  }
};

#endif // PLANE_HH
