
// RUN: %cxxamp %s -o %t.out && %t.out
#include <hc/hc.hpp>
#include <stdlib.h>
#include <iostream>
#include <limits>
#include <hc/hc_math.hpp.hpp>

using namespace hc;

int main(void) {
  const int vecSize = 2;

  // Alloc & init input data
  extent<1> e(vecSize);
  array_view<float, 1> in(vecSize);
  array_view<int, 1> out(vecSize);

  in[0] = 1.0f;
  in[1] = 0.0f;

  parallel_for_each(
    e,
    [=](index<1> idx) [[hc]] {
    out[idx] = fast_math::isinf(6.5f/in[idx]);
  });

  //check accelerator results
  for (int i=0; i<vecSize; ++i) {
    if (std::isinf(6.5f/in[i]) != (out[i] ? true : false))
      return 1;
  }

  //check on cpu
  for (int i=0; i<vecSize; ++i) {
    if (std::isinf(in[i]) != (fast_math::isinf(in[i]) ? true : false))
      return 1;
  }


  return 0;
}
