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
      F offset = Dot(orientation, Object<F>::mLocation);
      F sphereOffset = Dot(orientation, other->mLocation);
      F overlapLocal = offset - sphereOffset + otherSphere->mRadius;
      if (overlapLocal > 0) {
	overlap = overlapLocal;
	normal = orientation;
	location = other->mLocation - normal * (sphereOffset - offset);
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
