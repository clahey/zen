#include "Sphere.hh"
#include "Conversions.hh"
#include "Magnet.hh"
#include "Random.hh"
#include "Renderable.hh"
#include "Scene.hh"
#include <iostream>
#include <fstream>
using namespace std;

const int STEP_RATE = 75000;
const int SLOWDOWN = 1;
const int LENGTH = 10;
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
      Sphere<double>(100000, 1, .8)
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

  // small cube
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 1; j++) {
      for (int k = 0; k < 1; k++) {
	MyMagnet* m = new MyMagnet();
	m->mLocation(0, 0) = ((i * 2) - 1) * m->mRadius * 4;
	m->mLocation(1, 0) = i * m->mRadius * .5;
	m->mLocation(2, 0) = 0;
	m->mVelocity(0, 0) = -((i * 2) - 1) * m->mRadius * 100;
	m->mRotation = Quaternion<double>(generator(), generator() / 1000, generator() / 1000, generator()).Normalize();
	//ZenMatrix<double, 3, 1> rotation;
	//rotation(2, 0) = 1;
	//m->mRotation = Quaternion<double>(rotation, asin(1));
	scene.AddObject(m);
      }
    }
  }

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
