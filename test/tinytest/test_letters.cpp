#define TINYTEST_ASSERTION_FAILED_STOPS_EXECUTION 0
#define TINYTEST_FLAKY_TEST_ITERATIONS 3
#include <tinytest.hpp>

#include "letters.hpp"
#include <thread>


new_test() {
    handle_command_line_args();

    new_test_case("Test lookup for letter, lowercase");
        short pointsFromA = get_points_from_letter('a');
        test_assert("Letter 'a' is worth 1 point ?", pointsFromA == 1);

        short pointsFromZ = get_points_from_letter('z');
        test_assert("Letter 'z' is worth 10 points ?", pointsFromZ == 10);
    end_test_case();

    new_tagged_test_case("Test lookup for letter, uppercase", "Failing");
        short pointsFromA = get_points_from_letter('A');
        test_assert_var("Letter 'A' is worth 1 point ?", pointsFromA, ==, 1);

        test_warning("This test has been artificially lengthened to test the benchmarking system.");
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        short pointsFromZ = get_points_from_letter('Z');
        test_assert_var("Letter 'Z' is worth 10 point ?", pointsFromZ, ==, 11);
    end_test_case();

    new_tagged_test_case("Test lookup for non-existent letter", "Failing");
        short pointsFromUnknown = get_points_from_letter(' ');
        test_assert_pro("Letter ' ' is worth -1 point ?", pointsFromUnknown == 0, "pointsFromUnknown = " << pointsFromUnknown);
    end_test_case();

    new_test_case("Test that will skip");
        benchmark_start();
        benchmark_start();
        test_print("This won't skip");
        if (false) {
            test_assert_pro("This will not be run.", false, "HOW DID THIS RUN ??? Apparently false != false ???");
            skip_test_case();
        }
        test_passed();
        benchmark_stop();
        benchmark_start();
        benchmark_stop();
        benchmark_stop();

        test_print("This will now skip");
        skip_test_case();

        test_print("If this run, the program will terminate, because WHAT IN THE ACTUAL FUCK");
        std::terminate();
    end_test_case();

    new_test_case("Assertions that an exception will occur");
        test_assert_throws("Test that throws", throw std::exception());
    end_test_case();

    new_test_case("Long benchmark");
        constexpr long long ITERATIONS = 4'000'000;
        test_print("Benchmarks how long it takes to call the `get_points_from_letter()` function " << ITERATIONS << " times.");
        benchmark_long_start(ITERATIONS);
            get_points_from_letter(ITERATIONS % 26);
        benchmark_long_stop();
    end_test_case();

    srand(time(NULL));
    new_flaky_test_case("Flaky tests !");
        int randomChosenInt = rand() % 3;
        switch (randomChosenInt)
        {
        case 0:
            test_assert("", true);
            break;
        case 1:
            test_assert("", false);
            break;
        case 2:
        default:
            skip_test_case();
            break;
        }
    end_flaky_test_case();

    new_tagged_test_case("Tagged test case 1", "Tag1", "AllTags");
        test_print("Tag1 runs !");
    end_test_case();

    new_tagged_test_case("Tagged test case 2", "Tag2", "AllTags");
        test_print("Tag2 runs !");
    end_test_case();

    end_of_all_tests();
}