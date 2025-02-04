#include <tinytest2.hpp>

#include "letters.hpp"

TEST(LookupLettersLowercase) {
    short pointsFromA = get_points_from_letter('a');
    ASSERT("Letter 'a' is worth 1 point ?", ASSERT_EQUAL(pointsFromA, 1);)

    short pointsFromZ = get_points_from_letter('z');
    ASSERT_EQUAL(pointsFromZ, 10);
}

TEST(LookupLettersUppercase) {
    short pointsFromA = get_points_from_letter('A');
    ASSERT_EQUAL(pointsFromA, 1);

    short pointsFromZ = get_points_from_letter('Z');
    ASSERT_EQUAL(pointsFromZ, 10);
}

TEST(LookupLettersUnknown) {
    short pointsFromUnknown = get_points_from_letter(' ');
    ASSERT_EQUAL(pointsFromUnknown, -1);
}

int main() {
    Test::runAllTests();
}