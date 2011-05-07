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
  negcenter(1, 0) = -sqrt(3.0L) / 2 * 0.005;
  Scene<double> scene;
  for (int i = 0; i < 6; i++) {
    MyMagnet* m = new MyMagnet(0.07, .0005, 0.005);
    ZenMatrix<double, 3, 1> z;
    z(2, 0) = 1;
    m->mLocation = Rotate(negcenter, z, Degrees2Radians(60 * i + 30)) - negcenter;
    m->mOrientation = ZenMatrix<double, 3, 1>::GetRandom(1);
    m->mOrientation = Normalize(m->mOrientation);
//    // Intentionally not quite pi/2.
//    m->mOrientation = Rotate(m->mOrientation, z, 1.57);
    scene.AddObject(m);
    //    location += 0.005;
  }
  //  MyMagnet* m = new MyMagnet(1);
  //  m->mLocation(1, 0) = 0.005;
  //  scene.AddObject(m);

  //  cout << scene << endl;
  for (int i = 0; i < 10001; i++) {
    scene.Step(0.00001);
    if (! (i % OUTPUT_SKIP)) {
      char filename[30];
      sprintf(filename, "magnets-%04d.pov", i / OUTPUT_SKIP);
      ofstream out(filename);
      out << "#include \"scene.inc\"" << endl;
      scene.Render(out);
    }
  }
}
