#include "Random.hh"

using namespace std;

namespace Random {

static variate_generator<mt19937, uniform_real<> >* sRand;

std::variate_generator<mt19937, uniform_real<> >&
GetGenerator()
{
  if (sRand == NULL) {
    sRand = new variate_generator<mt19937, uniform_real<> >
      (mt19937(time(NULL)), uniform_real<>(-1, 1));
  }

  return *sRand;
}

} // namespace
