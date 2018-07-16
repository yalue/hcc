// RUN: %cxxamp %s -o %t.out && %t.out
#include <hc/hc.hpp>
int main()
{
    // This test outlines a subtle issue with how we obtain mangled kernel names
    // which is tracked in SWDEV-137849. fun is made static to work around it.
    int gpu_result;
	hc::array_view<int> gpu_resultsv(1, &gpu_result);
    gpu_resultsv.discard_data();
    static auto fun = [&]() [[cpu, hc]] { return 0; };
    hc::parallel_for_each(gpu_resultsv.get_extent(), [=] (hc::index<1> idx) [[hc]] { gpu_resultsv[idx] = fun(); });
}
