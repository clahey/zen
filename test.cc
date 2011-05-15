#include "Sphere.hh"
#include "Plane.hh"
#include "Gravity.hh"
#include "Conversions.hh"
#include "Magnet.hh"
#include "Random.hh"
#include "Renderable.hh"
#include "Scene.hh"
#include <iostream>
#include <fstream>
using namespace std;

const int STEP_RATE = 100000;
const int SLOWDOWN = 10;
const int LENGTH = 6;
const int FRAME_RATE = 30;

const double STEP_LENGTH = 1.0L / STEP_RATE;
const double FRAME_LENGTH = 1.0L / FRAME_RATE / SLOWDOWN;

class MyMagnet:
  public Magnet<double>,
  public Renderable<double>,
  public Sphere<double>
{
public:
  MyMagnet()
    : Magnet<double>(0.07),
      Sphere<double>(100000, 1, .8, 0.0001)
  {
    SetSize(.0005, 0.0025);
  }

  void Render(std::ostream& out)
  {
    out << "object { Magnet" << endl;
    OutputPosition(out);
    out << "}" << endl;
  }
};

class MyPlane:
  public Renderable<double>,
  public Plane<double>
{
public:
  MyPlane()
    : Plane<double>(100000, 1, .8, .001)
  {
    Object<double>::mImmobile = true;
    SetSize(0, 0.01);
  }

  void Render(std::ostream& out)
  {
    out << "object { Plane" << endl;
    OutputPosition(out);
    out << "}" << endl;
  }
};

template<class F>
void output(Scene<F>& scene, int frame)
{
  char filename[30];
  sprintf(filename, "magnets-%05d.pov", frame);
  ofstream out(filename);
  out << "#include \"scene.inc\"" << endl;
  scene.Render(out);
}

int main(int argc, char* argv[])
{
  ZenMatrix<double, 3, 1> negcenter;
  negcenter(1, 0) = -0.005;
  Scene<double> scene;
  std::variate_generator<std::mt19937, std::uniform_real<> >& generator = Random::GetGenerator();

  Quaternion<double> rotations[2];
  rotations[0] = Quaternion<double>(-0.301424, 0.00471262, -0.00594442, -0.95346);
  rotations[1] = Quaternion<double>(-0.616212, -0.000475526, 0.00072065, 0.78758);
  // small cube
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 1; j++) {
      for (int k = 0; k < 1; k++) {
	MyMagnet* m = new MyMagnet();
	m->mLocation(0, 0) = (i * 2 - 1) * m->mRadius * 4;
	m->mLocation(1, 0) = j * m->mRadius * 2 + i * m->mRadius;
	m->mLocation(2, 0) = k * m->mRadius * 2;
	//	m->mRotation = Quaternion<double>(generator(), generator() / 1000, generator() / 1000, generator()).Normalize();
	//	cout << m->mRotation << endl;
	m->mVelocity(0, 0) = -(i * 2 - 1) * m->mRadius * 50;
//	ZenMatrix<double, 3, 1> rotation;
//	rotation(1, 0) = .1 * i;
//	rotation(2, 0) = 1;
//	rotation = Normalize(rotation);
//	m->mRotation = Quaternion<double>(rotation, asin(1) * 1.5);
	m->mRotation = rotations[i];
	scene.AddObject(m);
      }
    }
  }


//  MyPlane* p = new MyPlane();
//  ZenMatrix<double, 3, 1> z;
//  z(2, 0) = 1;
//  p->mRotation = Quaternion<double>(z, asin(1));
//  p->mLocation(1, 0) = -0.01;
//  scene.AddObject(p);
//
//  Gravity<double>* g = new Gravity<double>(10);
//  g->mRotation = Quaternion<double>(z, -asin(1));
//  scene.AddObject(g);

  double frameTime = 0;
  int frame = 0;
  output(scene, frame++);
  while (frame < LENGTH * FRAME_RATE) {
    if (frameTime > FRAME_LENGTH) {
      output(scene, frame++);
      frameTime = 0;
    }
    scene.Step(STEP_LENGTH);
    frameTime += STEP_LENGTH;
  }
}
