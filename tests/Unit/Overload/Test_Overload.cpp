// RUN: %cxxamp %s -o %t.out && %t.out
#include <hc/hc.hpp>

using namespace hc;

#define TEST_CPU
#define TEST_ELIDED
#define TEST_GPU
#define TEST_BOTH

int callee() [[hc]]
{
    return 1;
}

int callee()
{
    return 2;
}

#ifdef TEST_CPU
bool CPU_Func()
{
    // in CPU path, return 1; in GPU path, return 0

    if (callee() != 2)
    {
        return false;
    }

    return true;
}
#endif

#ifdef TEST_ELIDED
bool Elided_Func()
{
    if (callee() != 2)
    {
        return false;
    }

    return true;
}
#endif

#ifdef TEST_GPU
bool AMP_Func() [[hc]]
{
    if (callee() != 1)
    {
        return false;
    }

    return true;
}
#endif

#ifdef TEST_BOTH
bool BOTH_CPU_AND_AMP() [[cpu, hc]]
{
#if __HCC_ACCELERATOR__
    if (callee() != 1)
#else
    if (callee() != 2)
#endif
    {
        return false;
    }

    return true;
}
#endif

int main()
{
    int flag;
#ifdef TEST_CPU
    flag = CPU_Func()? 0 : 1;
    if(flag) { printf("CPU_Func Error! exit!\n"); exit(1);}
#endif
#ifdef TEST_ELIDED
    flag = Elided_Func()?0:1;
    if(flag) { printf("Elided_Func Error! exit!\n"); exit(1);}
#endif
#ifdef TEST_GPU
    // directly called is not allowed, we use pfe
    {
      int result;
      array_view<int> gpu_resultsv(1, &result);
      parallel_for_each(gpu_resultsv.get_extent(), [=](index<1> idx) [[hc]] {
        gpu_resultsv[idx] = AMP_Func();
      });

       if(gpu_resultsv[0] == 0) { printf("AMP_Func Error! exit!\n"); exit(1);}
     }
#endif

#ifdef TEST_BOTH
    {
      int result;
      array_view<int> gpu_resultsv(1, &result);
      parallel_for_each(gpu_resultsv.get_extent(), [=](index<1> idx) [[hc]]
      {
        gpu_resultsv[idx] = BOTH_CPU_AND_AMP();
      });

       if(gpu_resultsv[0] == 0) { printf("BOTH_CPU_AND_AMP Error! exit!\n"); exit(1);}
     }
#endif

    return 0;
}
