// RUN: %gtest_amp %s -o %t && %t
#include <stdlib.h>
#include <hc.hpp>
#include <gtest/gtest.h>
// the functor to test
class baz {
 public:
  void operator()(hc::index<1> idx) [[hc]] {}
  int foo;
  float bar;
};

TEST(GPUCodeGen, TrampolineName) {
  // Inject the trampoline declaration
  void* bar = reinterpret_cast<void*>(&baz::__cxxamp_trampoline);
  // An injected member function __cxxamp_trampoline_name
  // should return the mangled name of the trampoline
  // hardcoded for now..
  EXPECT_EQ(std::string("_ZN3baz19__cxxamp_trampolineEif"),
    std::string(baz::__cxxamp_trampoline_name()));
}
