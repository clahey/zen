#ifndef QUATERNION_HH
#define QUATERNION_HH

#include <assert.h>
#include "ZenMatrix.hh"

template<class F> class Quaternion;
template<class F> std::ostream& operator<<(std::ostream& out, const Quaternion<F>& q);

template<class F>
class Quaternion
{
public:
  Quaternion() {
    mReal = 0;
    for (int i = 0; i < 3; i++) {
      mImaginary[i] = 0;
    }
  }

  Quaternion(F real, F i, F j, F k) {
    mReal = real;
    mImaginary[0] = i;
    mImaginary[1] = j;
    mImaginary[2] = k;
  }

  Quaternion(ZenMatrix<F, 3, 1> axis, F angle) {
    assert(abs(Length(axis) - 1) < 0.0001);
    mReal = cos(angle / 2);
    F sinangle = sin(angle / 2);
    mImaginary[0] = axis(0, 0) * sinangle;
    mImaginary[1] = axis(1, 0) * sinangle;
    mImaginary[2] = axis(2, 0) * sinangle;
    assert(abs(Norm() - 1 < 0.0001));
  }

  // Represent a vector as a quaternion.
  Quaternion(ZenMatrix<F, 3, 1> vector) {
    mReal = 0;
    mImaginary[0] = vector(0, 0);
    mImaginary[1] = vector(1, 0);
    mImaginary[2] = vector(2, 0);
  }

  Quaternion(F real) {
    mReal = real;
    for (int i = 0; i < 3; i++) {
      mImaginary[i] = 0;
    }
  }

  const static Quaternion<F> i;
  const static Quaternion<F> j;
  const static Quaternion<F> k;

  bool operator==(const Quaternion<F>& other)
    const
  {
    for (int i = 0; i < 3; i++) {
      if (mImaginary[i] != other.mImaginary[i]) {
	return false;
      }
    }
    return mReal == other.mReal;
  }

  bool operator!=(const Quaternion<F>& other)
    const
  {
    for (int i = 0; i < 3; i++) {
      if (mImaginary[i] != other.mImaginary[i]) {
	return true;
      }
    }
    return mReal != other.mReal;
  }

  bool operator<(F scalar)
    const
  {
    return Norm() < scalar;
  }

  bool operator<(const Quaternion<F> other)
    const
  {
    return Norm() < other.Norm();
  }

  bool operator>(F scalar)
    const
  {
    return Norm() > scalar;
  }

  bool operator>(const Quaternion<F> other)
    const
  {
    return Norm() > other.Norm();
  }

  bool operator>=(const Quaternion<F> other)
    const
  {
    return operator==(other) || operator>(other);
  }

  bool operator<=(const Quaternion<F> other)
    const
  {
    return operator==(other) || operator<(other);
  }

  Quaternion<F> operator+(const Quaternion<F>& other)
    const
  {
    Quaternion<F> retval;
    retval.mReal = mReal + other.mReal;
    for (int i = 0; i < 3; i++) {
      retval.mImaginary[i] = mImaginary[i] + other.mImaginary[i];
    }
    return retval;
  }

  Quaternion<F> operator-(const Quaternion<F>& other)
    const
  {
    Quaternion<F> retval;
    retval.mReal = mReal - other.mReal;
    for (int i = 0; i < 3; i++) {
      retval.mImaginary[i] = mImaginary[i] - other.mImaginary[i];
    }
    return retval;
  }

  Quaternion<F> operator-()
    const
  {
    Quaternion<F> retval;
    retval.mReal = -mReal;
    for (int i = 0; i < 3; i++) {
      retval.mImaginary[i] = -mImaginary[i];
    }
    return retval;
  }

  Quaternion<F> operator*(const Quaternion<F>& other)
    const
  {
    Quaternion<F> retval;
    retval.mReal = mReal * other.mReal;
    for (int i = 0; i < 3; i++) {
      retval.mReal -= mImaginary[i] * other.mImaginary[i];
      retval.mImaginary[i]
	= mReal * other.mImaginary[i]
	+ mImaginary[i] * other.mReal
	+ mImaginary[(i + 1) % 3] * other.mImaginary[(i + 2) % 3]
	- mImaginary[(i + 2) % 3] * other.mImaginary[(i + 1) % 3];
    }
    return retval;
  }

  Quaternion<F> operator*(F scalar)
    const
  {
    Quaternion<F> retval;
    retval.mReal = mReal * scalar;
    retval.mImaginary[0] = mImaginary[0] * scalar;
    retval.mImaginary[1] = mImaginary[1] * scalar;
    retval.mImaginary[2] = mImaginary[2] * scalar;
    return retval;
  }

  /* Be very careful.  This only works if you multiply on the right. */
  Quaternion<F> operator/(const Quaternion<F>& other)
    const
  {
    F norm = other.Norm();
    return operator*(other.Conj()) / norm / norm;
  }

  Quaternion<F> operator/(F scalar)
    const
  {
    Quaternion<F> retval;
    retval.mReal = mReal / scalar;
    retval.mImaginary[0] = mImaginary[0] / scalar;
    retval.mImaginary[1] = mImaginary[1] / scalar;
    retval.mImaginary[2] = mImaginary[2] / scalar;
    return retval;
  }

  Quaternion<F> operator+(F scalar)
    const
  {
    Quaternion<F> retval;
    retval.mReal = mReal + scalar;
    return retval;
  }

  Quaternion<F> operator-(F scalar)
    const
  {
    Quaternion<F> retval;
    retval.mReal = mReal - scalar;
    return retval;
  }

  F Norm()
    const
  {
    F retval = mReal * mReal;
    for (int i = 0; i < 3; i++) {
      retval += mImaginary[i] * mImaginary[i];
    }
    return sqrt(retval);
  }

  Quaternion<F> Normalize()
    const
  {
    return *this / Norm();
  }

  ZenMatrix<F, 3, 3> GetRotationMatrix()
    const
  {
    ZenMatrix<F, 3, 3> retval;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
	if (i == j) {
	  retval(i, i) = -1 + 2 * mImaginary[i] * mImaginary[i] + 2 * mReal * mReal;
	} else {
	  // (i + j) * 2 % 3 != i or j.
	  int k = (i + j) * 2 % 3;
	  // (j - i + 4) % 3 - 1  == 1 if i + 1 % 3 == j and -1 if j + 1 % 3 == i
	  // It's + 4 instead of + 1 since -1 % 3 == -1, not 2.
	  retval(i, j) = 2 * (mImaginary[i] * mImaginary[j] - ((j - i + 4) % 3 - 1) * mReal * mImaginary[k]);
	}
      }
    }
    return retval;
  }

  ZenMatrix<F, 3, 1> Rotate(const ZenMatrix<F, 3, 1>& vector)
    const
  {
    Quaternion<F> result = *this * vector / *this;
    ZenMatrix<F, 3, 1> retval;
    retval(0, 0) = result.mImaginary[0];
    retval(1, 0) = result.mImaginary[1];
    retval(2, 0) = result.mImaginary[2];
    return retval;
  }

  Quaternion<F> Conj()
    const
  {
    Quaternion<F> retval;
    retval.mReal = mReal;
    retval.mImaginary[0] = -mImaginary[0];
    retval.mImaginary[1] = -mImaginary[1];
    retval.mImaginary[2] = -mImaginary[2];
    return retval;
  }

 private:
  friend std::ostream& operator<< <>(std::ostream& out, const Quaternion<F>& q);
  F mReal;
  F mImaginary[3];
};

template<class F>
Quaternion<F> operator*(F scalar, const Quaternion<F>& value)
{
  return value * scalar;
}

template<class F>
Quaternion<F> operator-(F scalar, const Quaternion<F>& value)
{
  return -value + scalar;
}

template<class F>
Quaternion<F> operator+(F scalar, const Quaternion<F>& value)
{
  return value + scalar;
}

template<class F>
std::ostream& operator<<(std::ostream& out, const Quaternion<F>& q)
{
  out << "(" << q.mReal << ", <" << q.mImaginary[0] << ", " << q.mImaginary[1] << ", " << q.mImaginary[2] << ">)";

  return out;
}

template<class F>
const Quaternion<F> Quaternion<F>::i = Quaternion<F>(0, 1, 0, 0);

template<class F>
const Quaternion<F> Quaternion<F>::j = Quaternion<F>(0, 0, 1, 0);

template<class F>
const Quaternion<F> Quaternion<F>::k = Quaternion<F>(0, 0, 0, 1);

#endif
