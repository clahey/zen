#ifndef RANDOM_HH
#define RANDOM_HH

#include <random>

namespace Random {

std::variate_generator<std::mt19937, std::uniform_real<> >& GetGenerator();

} // namespace

#endif

