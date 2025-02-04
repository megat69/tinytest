#define TINYTEST_ASSERTION_FAILED_STOPS_EXECUTION 0
#include <tinytest1.hpp>

#include "letters.hpp"


new_test() {
    set_verbose_status();

    new_test_case("Test lookup for letter, lowercase");
        short pointsFromA = get_points_from_letter('a');
        test_assert("Letter 'a' is worth 1 point ?", pointsFromA == 1);

        short pointsFromZ = get_points_from_letter('z');
        test_assert("Letter 'z' is worth 10 points ?", pointsFromZ == 10);
    end_test_case();

    new_test_case("Test lookup for letter, uppercase");
        short pointsFromA = get_points_from_letter('A');
        test_assert("Letter 'A' is worth 1 point ?", pointsFromA == 1);

        short pointsFromZ = get_points_from_letter('Z');
        test_assert("Letter 'Z' is worth 10 point ?", pointsFromZ == 10);
    end_test_case();

    new_test_case("Test lookup for non-existent letter");
        short pointsFromUnknown = get_points_from_letter(' ');
        test_assert_pro("Letter ' ' is worth -1 point ?", pointsFromUnknown == 0, "pointsFromUnknown = " << pointsFromUnknown);
    end_test_case();
}