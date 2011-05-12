#include "Sphere.hh"
#include "Conversions.hh"
#include "Magnet.hh"
#include "Random.hh"
#include "Renderable.hh"
#include "Scene.hh"
#include <iostream>
#include <fstream>
using namespace std;

const int OUTPUT_SKIP = 25;
const int SLOWDOWN = 400;

class MyMagnet:
  public Magnet<double>,
  public Renderable<double>,
  public Sphere<double>
{
public:
  MyMagnet()
    : Magnet<double>(0.07),
      Sphere<double>(100000, 5)
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
	m->mLocation(1, 0) = i * m->mRadius * 3;
	m->mLocation(2, 0) = 0;
	m->mVelocity(0, 0) = -((i * 2) - 1) * m->mRadius * 1000;
	m->mRotation = Quaternion<double>(generator(), generator(), generator(), generator()).Normalize();
	scene.AddObject(m);
      }
    }
  }

  for (int i = 0; i < 400 * OUTPUT_SKIP + 1; i++) {
    if (! (i % OUTPUT_SKIP)) {
      char filename[30];
      sprintf(filename, "magnets-%04d.pov", i / OUTPUT_SKIP);
      ofstream out(filename);
      out << "#include \"scene.inc\"" << endl;
      scene.Render(out);
    }
    scene.Step(0.1 / SLOWDOWN / OUTPUT_SKIP);
  }
}
