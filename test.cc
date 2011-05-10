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
  MyMagnet(double strength, double mass, double radius)
    : Magnet<double>(strength)
  {
    SetSize(mass, radius);
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
  for (int i = 0; i < 6; i++) {
    MyMagnet* m = new MyMagnet(0.07, .0005, 0.0025);
    ZenMatrix<double, 3, 1> z;
    z(2, 0) = 1;
    m->mLocation = Rotate(negcenter, z, Degrees2Radians(60 * i + 30)) - negcenter;
    std::variate_generator<std::mt19937, std::uniform_real<> >& generator = Random::GetGenerator();
    //    ZenMatrix<double, 3, 1> axis = Normalize(ZenMatrix<double, 3, 1>::GetRandom());
    //    double angle = generator() * 4 * asin(1.0);
    m->mRotation = Quaternion<double>(generator(), generator(), generator(), generator()).Normalize();
    //    cout << m->mRotation << endl;
    scene.AddObject(m);
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
