#ifndef EVS_BENCHMARK_HPP
#define EVS_BENCHMARK_HPP

#include "lib/eviltwin/observer_safe.hpp"

#include "chrono_timer.hpp"
#include "benchmark.hpp"

#include <forward_list>
#include <vector>

namespace Benchmark
{

class Evs
{
    NOINLINE(void handler(Rng_t& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    using Subject = Nano::Safe::Subject<void(Rng_t&)>;
    using Foo = Benchmark::Evs;

    static chrono_timer timer;

    public:

//------------------------------------------------------------------------------

    NOINLINE(static double construction(std::size_t N))
    {
        std::size_t count = 0;
        return 1.0;
    }

//------------------------------------------------------------------------------

    NOINLINE(static double destruction(std::size_t N))
    {
        std::size_t count = 0;
        return 1.0;
    }

//------------------------------------------------------------------------------

    NOINLINE(static double connection(std::size_t N))
    {
        std::size_t count = 0;
        return 1.0;
    }

//------------------------------------------------------------------------------

    NOINLINE(static double emission(std::size_t N))
    {
        std::size_t count = 0;
        return 1.0;
    }

//------------------------------------------------------------------------------

    NOINLINE(static double combined(std::size_t N, std::size_t ratio))
    {
        Rng_t rng;
        Eng_t eng(0, 100);
        std::size_t count = 0;
        timer.reset();

        using std::placeholders::_1;

        for (; g_limit > timer.count<Timer_u>(); ++count)
        {
            Subject subject;
            std::vector<Foo> foo_array(N);

            std::forward_list<Nano::Safe::UniversalPtr> reg;

            for (auto& foo : foo_array)
            {
                if (eng(rng) > ratio)
                {
                    reg.emplace_front(subject.connect
                        (std::bind(&Foo::handler, &foo, _1)));
                }
                else
                {
                    subject(rng);
                }
            }
        }
        return N / std::chrono::duration_cast<Delta_u>
            (Timer_u(g_limit / count)).count();
    }
};

chrono_timer Evs::timer;

} // namespace Benchmark -------------------------------------------------------

#endif // EVS_BENCHMARK_HPP