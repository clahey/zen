#include "Scene.hh"
#include "Magnet.hh"
#include "Renderable.hh"
#include "Conversions.hh"
#include "Random.hh"
#include <iostream>
#include <fstream>
using namespace std;

const int OUTPUT_SKIP = 25;

class MyMagnet:
  public Magnet<double>,
  public Renderable<double>
{
public:
  MyMagnet()
    : Magnet<double>(0.07)
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
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 3; k++) {
	MyMagnet* m = new MyMagnet();
	m->mLocation(0, 0) = i * m->mRadius * 2;
	m->mLocation(1, 0) = j * m->mRadius * 2;
	m->mLocation(2, 0) = k * m->mRadius * 2;
	m->mRotation = Quaternion<double>(generator(), generator(), generator(), generator()).Normalize();
	scene.AddObject(m);
      }
    }
  }

  for (int i = 0; i < 10001; i++) {
    if (! (i % OUTPUT_SKIP)) {
      char filename[30];
      sprintf(filename, "magnets-%04d.pov", i / OUTPUT_SKIP);
      ofstream out(filename);
      out << "#include \"scene.inc\"" << endl;
      scene.Render(out);
    }
    scene.Step(0.00001);
  }
}
