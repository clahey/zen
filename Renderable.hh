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
#if 0
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
#endif
    out << "matrix <";
    ZenMatrix<F, 3, 3> matrix = Object<F>::mRotation.GetRotationMatrix();
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
	//	if ((i == 2 || j == 2) && i != j) {
	//	  out << -(matrix(i, j) * Object<F>::mRadius) << ", ";
	//	} else {
	  out << (matrix(j, i) * Object<F>::mRadius) << ", "; 
	  //	}
      }
    }
    out << Object<F>::mLocation(0, 0) << ", ";
    out << Object<F>::mLocation(1, 0) << ", ";
    out << Object<F>::mLocation(2, 0) << ">" << std::endl;
  }
private:
  F mPI;
};

#endif
