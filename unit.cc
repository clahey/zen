#define BOOST_TEST_DYN_LINK 
#define BOOST_TEST_MODULE Quaternion
#include <boost/test/unit_test.hpp>
#include "Quaternion.hh"
#include "ZenMatrix.hh"


typedef Quaternion<double> Quat;
BOOST_AUTO_TEST_CASE(MyTestCase)
{
  std::variate_generator<std::mt19937, std::uniform_real<> >& generator = Random::GetGenerator();
  BOOST_CHECK_EQUAL(Quat::i * Quat::j, Quat::k);
  ZenMatrix<double, 3, 1> axis = Normalize(ZenMatrix<double, 3, 1>::GetRandom());
  double angle = generator();
  Quat rotation(axis, angle);
  ZenMatrix<double, 3, 1> vector = ZenMatrix<double, 3, 1>::GetRandom();
  
  BOOST_CHECK_CLOSE(rotation * rotation.Conj(), Quat(1), 0.0000001);
  BOOST_CHECK_CLOSE(Quat(rotation.Rotate(vector)), Quat(Rotate(vector, axis, angle)), 0.0000001);
  BOOST_CHECK_CLOSE(Quat(rotation.GetRotationMatrix() * vector), Quat(Rotate(vector, axis, angle)), 0.0000001);
}
