#ifndef RENDERABLE_HH
#define RENDERABLE_HH

template<class F>
class Renderable :
  virtual public Object<F>
{
public:
  virtual void Render(std::ostream& out) = 0;

protected:
  void OutputPosition(std::ostream& out)
  {
    if (mPI == 0) {
      mPI = 4*atan(1);
    }

    out << "scale " << Object<F>::mRadius << std::endl;

    F y = Object<F>::mOrientation(1, 0);
    F z = Object<F>::mOrientation(2, 0);
    if (z == 1) {
      out << "rotate y * -90" << std::endl;
    } else if (z == -1) {
      out << "rotate y * 90" << std::endl;
    } else {
      F yrotation = asin(-z);
      F zrotation = asin(-(y / sqrt((1 - z * z))));
      out << "rotate y * " << (yrotation * 360 / 2 / mPI) << std::endl;
      out << "rotate z * " << (zrotation * 360 / 2 / mPI) << std::endl;
    }
    out << "translate <" << Object<F>::mLocation(0, 0);
    out << ", " << Object<F>::mLocation(1, 0);
    out << ", " << -Object<F>::mLocation(2, 0);
    out << ">" << std::endl;
  }
private:
  F mPI;
};

#endif
