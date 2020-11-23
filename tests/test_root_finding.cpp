#include <doctest/doctest.h>
#include "../include/isto/root_finding/root_finding.hpp"
    using namespace isto::root_finding;
#include <cmath>
    using std::cos, std::sin, std::pow;

TEST_CASE("root_finding.hpp")
{
        auto
    f1 = [](double x){ return cos (x) - pow (x, 3.0); };
        auto
    df1 = [](double x){ return -sin (x) - 3 * pow (x, 2.0); };
        auto const
    target1 = 0.8654740331016144466206859011862287477929;
     
        auto
    f2 = [](double x){ return cos (x) - x; };
        auto
    df2 = [](double x){ return -sin (x) - 1; };
        auto const
    target2 = 0.7390851332151606416553120876738734040134 ;

        auto
    cvg1 = [](double x){ return fabs (x) < 1e-12; };

    SUBCASE("newton")
    {
            auto
        r = newton (f1, df1, 1.0, cvg1);
        CHECK(r == doctest::Approx { target1 });

            auto
        s = newton (f2, df2, 1.0, cvg1); 
        CHECK(s == doctest::Approx { target2 });
    }
    SUBCASE("newton, throws if zero derivative")
    {
        CHECK_THROWS_AS(
              newton (f1, [](auto){ return 0.0; }, 1.0, cvg1)
            , zero_derivative_e
        );
    }
    SUBCASE("newton, with options")
    {
        CHECK_THROWS_AS(
              newton (f1, df1, 1.0, cvg1, { .max_iter = 1 })
            , no_convergence_e
        );
    }
    SUBCASE("newton, with info")
    {
            auto
        info = info_iterations_t {};
        newton (f1, df1, 1.0, cvg1, { /*default options*/ }, info);
        CHECK(info.iteration_count > 1);
    }


        auto
    cvg2 = [](
          double a
        , double b
        , double fa
        , double fb
    ){ 
            constexpr double
        tol = 1e-12;
        return 
               fa == 0.0
            || fb == 0.0
            || fabs (b - a) < tol
        ;
    };

    SUBCASE("zhang")
    {
            auto
        r = zhang (f1, 0.0, 10.0, cvg2);
        CHECK(r == doctest::Approx { target1 });

            auto
        s = zhang (f2, 0.0, 10.0, cvg2); 
        CHECK(s == doctest::Approx { target2 });
    }
    SUBCASE("zhang, throws if no single root between brackets")
    {
        CHECK_THROWS_AS(
              zhang (f1, 0.0, 0.1, cvg2);
            , no_single_root_between_brackets_e
        );
    }
    SUBCASE("zhang, with options")
    {
        CHECK_THROWS_AS(
              zhang (f1, 0.0, 10.0, cvg2, { .max_iter = 1 })
            , no_convergence_e
        );
    }
    SUBCASE("zhang, with info")
    {
            auto
        info = info_iterations_t {};
        zhang (f1, 0.0, 10.0, cvg2, { /*default options*/ }, info);
        CHECK(info.iteration_count > 1);
    }
}