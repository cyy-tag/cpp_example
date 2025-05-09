#pragma once

template<unsigned p, unsigned d>
struct DoIsPrime {
  static constexpr bool value = (p % d != 0) && DoIsPrime<p, d-1>::value;
};

template<unsigned p>
struct DoIsPrime<p, 2> {
  static constexpr bool value = (p % 2 != 0);
};

template<unsigned p>
struct IsPrime {
  static constexpr bool value = DoIsPrime<p, p/2>::value;
};

template<>
struct IsPrime<0> { static constexpr bool value = false; };
template<>
struct IsPrime<1> { static constexpr bool value = false; };
template<>
struct IsPrime<2> { static constexpr bool value = true; };
template<>
struct IsPrime<3> { static constexpr bool value = true; };

// C++ 14 constexpr function
constexpr bool isPrime(unsigned int p)
{
  for(unsigned int d = 2; d <= p / 2; d++) {
    if(p % d == 0) {
      return false;
    }
  }
  return p > 1;
}
