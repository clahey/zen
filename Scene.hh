#ifndef SCENE_HH
#define SCENE_HH

#include "Object.hh"
#include "Renderable.hh"
#include <list>

template<class F> class Scene;
template<class F> std::ostream& operator<<(std::ostream& out, const Scene<F>& scene);

template<class F>
class Scene
{
public:
  typedef std::list<Object<F>*> ObjectList;

  void Step(F timeslice) {
    for (typename ObjectList::iterator i = mObjects.begin(); i != mObjects.end(); i++) {
      typename ObjectList::iterator j = i;
      j++;
      for (; j != mObjects.end(); j++) {
	(*i)->Interact(*j, timeslice);
      }
      //      (*i)->DampenAngularVelocity(timeslice);
      //      (*i)->RandomizeAngularVelocity(timeslice);
      (*i)->ApplyAngularVelocity(timeslice);
      (*i)->ApplyVelocity(timeslice);
    }
  }

  void AddObject(Object<F>* object) {
    mObjects.push_back(object);
  }

  void Render(std::ostream& out) {
    for (typename ObjectList::const_iterator i = mObjects.begin(); i != mObjects.end(); i++) {
      Object<F>* object = *i;
      Renderable<F>* renderable = dynamic_cast<Renderable<F>*>(object);
      if (renderable != NULL) {
	renderable->Render(out);
      }
    }
  }

private:
  friend std::ostream& operator<< <>(std::ostream& out, const Scene<F>& scene);
  ObjectList mObjects;
};

template<class F>
std::ostream& operator<<(std::ostream& out, const Scene<F>& scene)
{
  out << "(";
  for (typename Scene<F>::ObjectList::const_iterator i = scene.mObjects.begin(); i != scene.mObjects.end(); i++) {
    out << **i;
  }
  out << ")";
  return out;
}

#endif
