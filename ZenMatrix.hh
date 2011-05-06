#ifndef ZEN_MATRIX_HH
#define ZEN_MATRIX_HH

#include <math.h>
#include <iostream>

template<class F, int R, int C>
class ZenMatrix
{
public:
  inline F& operator()(int r, int c) { return values[r * C + c]; }
  inline const F& operator()(int r, int c) const { return values[r * C + c]; }

  ZenMatrix()
  {
    for (int r = 0; r < R; r++) {
      for (int c = 0; c < C; c++) {
	operator()(r, c) = 0;
      }
    }
  }

  template<int otherC>
  ZenMatrix<F, R, otherC> operator* (const ZenMatrix<F, C, otherC>& other) const {
    ZenMatrix<F, R, otherC> retval;
    for (int r = 0; r < R; r++) {
      for (int c = 0; c < otherC; c++) {
	for (int k = 0; k < C; k++) {
	  retval(r, c) += operator()(r, k) * other(k, c);
	}
      }
    }
    return retval;
  }

  ZenMatrix<F, R, C> operator+ (const ZenMatrix<F, R, C>& other) const {
    ZenMatrix<F, R, C> retval;
    for (int r = 0; r < R; r++) {
      for (int c = 0; c < C; c++) {
	retval(r, c) = operator()(r, c) + other(r, c);
      }
    }
    return retval;
  }

  ZenMatrix<F, R, C> operator- (const ZenMatrix<F, R, C>& other) const {
    ZenMatrix<F, R, C> retval;
    for (int r = 0; r < R; r++) {
      for (int c = 0; c < C; c++) {
	retval(r, c) = operator()(r, c) - other(r, c);
      }
    }
    return retval;
  }

  ZenMatrix<F, R, C> operator* (const F& s) const {
    ZenMatrix<F, R, C> retval;
    for (int r = 0; r < R; r++) {
      for (int c = 0; c < C; c++) {
	retval(r, c) = operator()(r, c) * s;
      }
    }
    return retval;
  }

  ZenMatrix<F, R, C> operator/ (const F& s)
    const
  {
    ZenMatrix<F, R, C> retval;
    for (int r = 0; r < R; r++) {
      for (int c = 0; c < C; c++) {
	retval(r, c) = operator()(r, c) / s;
      }
    }
    return retval;
  }

//  ZenMatrix<F, R, C>& operator+= (const ZenMatrix<F, R, C>& other) { *this = *this + other; return *this; }
//  ZenMatrix<F, R, C>& operator-= (const ZenMatrix<F, R, C>& other) { *this = *this - other; return *this; }
//  ZenMatrix<F, R, C>& operator*= (F s) { *this = *this * s; return *this; }
//  ZenMatrix<F, R, C>& operator/= (F s) { *this = *this / s; return *this; }

  ZenMatrix<F, R, C>& operator+= (const ZenMatrix<F, R, C>& other)
  {
    for (int r = 0; r < R; r++) {
      for (int c = 0; c < C; c++) {
	operator()(r, c) += other(r, c);
      }
    }
    return *this;
  }

  ZenMatrix<F, R, C>& operator-= (const ZenMatrix<F, R, C>& other)
  {
    for (int r = 0; r < R; r++) {
      for (int c = 0; c < C; c++) {
	operator()(r, c) -= other(r, c);
      }
    }
    return *this;
  }

  ZenMatrix<F, R, C>& operator*= (F s)
  {
    for (int r = 0; r < R; r++) {
      for (int c = 0; c < C; c++) {
	operator()(r, c) *= s;
      }
    }
    return *this;
  }

  ZenMatrix<F, R, C>& operator/= (F s)
  {
    for (int r = 0; r < R; r++) {
      for (int c = 0; c < C; c++) {
	operator()(r, c) /= s;
      }
    }
    return *this;
  }

  ZenMatrix<F, C, R> T()
    const
  {
    ZenMatrix<F, C, R> retval;
    for (int r = 0; r < R; r++) {
      for (int c = 0; c < C; c++) {
	retval(c, r) = operator()(r, c);
      }
    }
    return retval;
  }


private:
  F values[R * C];
};


template<class F>
ZenMatrix<F, 3, 1> Cross(const ZenMatrix<F, 3, 1>& a,
			 const ZenMatrix<F, 3, 1>& b)
{
  ZenMatrix<F, 3, 1> retval;
  retval(0, 0) = a(1, 0) * b(2, 0) - a(2, 0) * b(1, 0);
  retval(1, 0) = a(2, 0) * b(0, 0) - a(0, 0) * b(2, 0);
  retval(2, 0) = a(0, 0) * b(1, 0) - a(1, 0) * b(0, 0);
  return retval;
}

template<class F, int N>
F Dot(const ZenMatrix<F, N, 1>& a,
      const ZenMatrix<F, N, 1>& b)
{
	F retval = 0;
	for (int i = 0; i < N; i++) {
		retval += a(i, 0) * b(i, 0);
	}
	return retval;
}

template<class F, int N>
F Length(const ZenMatrix<F, N, 1>& a)
{
	return sqrt(Dot(a, a));
}

template<class F, int N>
ZenMatrix<F, N, 1> Normalize(const ZenMatrix<F, N, 1>& a)
{
	return a / Length(a);
}

/*
 * Rotate
 * v: vector to rotate
 * axis: Axis of rotation (must be normalized)
 * angle: Angle of rotation (in radians, right hand rule)
 */
template<class F, int N>
ZenMatrix<F, N, 1> Rotate(const ZenMatrix<F, N, 1>& v,
			  const ZenMatrix<F, N, 1>& axis,
			  F angle)
{
  F cosangle = cos(angle);
  return v * cosangle + Cross(axis, v) * sin(angle) + axis * Dot(axis, v) * (1 - cosangle);
}

/*
 * Rotate
 * v: vector to rotate
 * moment: axis and angle of rotation.
 */
template<class F, int N>
ZenMatrix<F, N, 1> Rotate(const ZenMatrix<F, N, 1>& v,
			  const ZenMatrix<F, N, 1>& moment)
{
  F length = Length(moment);
  if (length == 0) {
    return v;
  }
  return Rotate (v, moment / length, length);
}

/*
 * Rotate
 * v: vector to rotate
 * moment: angular velocity
 * time: time slice.
 */
template<class F, int N>
ZenMatrix<F, N, 1> ApplyAngularVelocity(const ZenMatrix<F, N, 1>& v,
					const ZenMatrix<F, N, 1>& moment,
					F time)
{
  F length = Length(moment);
  return Rotate (v, moment * (time / length), length);
}

template<class F, int R, int C>
std::ostream& operator<<(std::ostream& out, const ZenMatrix<F, R, C>& matrix)
{
  out << "( ";
  for (int r = 0; r < R; r++) {
    if (r != 0) {
      out << ", ";
    }
    out << "(";
    for (int c = 0; c < C; c++) {
      if (c != 0) {
	out << ", ";
      }
      out << matrix(r, c);
    }
    out << ")";
  }
  out << " )";
  return out;
}

template<class F, int N>
std::ostream& operator<<(std::ostream& out, const ZenMatrix<F, N, 1>& matrix)
{
  out << "(";
  for (int r = 0; r < N; r++) {
    if (r != 0) {
      out << " ";
    }
    out << matrix(r, 0);
  }
  out << ")";

  return out;
}


#endif // ZEN_MATRIX_HH
