#ifndef CONVERSIONS_HH
#define CONVERSIONS_HH

double
Degrees2Radians(double deg) {
  return deg * atan(1.0) / 45;
}

double
Radians2Degrees(double deg) {
  return deg * 45 / atan(1.0);
}

#endif // CONVERSIONS_HH
