#ifndef COLLIDABLE_HH
#define COLLIDABLE_HH

#include "Object.hh"
#include <exception>

template<class F>
class Collidable:
  virtual public Object<F>
{
public:
  Collidable(F springConstant)
    : mSpringConstant(springConstant)
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
	ZenMatrix<F, 3, 1> force = normal * (-overlap * springConstant);
	ApplyForce(force, timeslice);
	other->ApplyForce(-force, timeslice);
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
};

#endif // COLLIDABLE_HH
