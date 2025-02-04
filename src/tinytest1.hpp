#include <string.h>
#include <iostream>
#include <chrono>

#ifndef TINYTEST_ASSERTION_FAILED_TO_STDERR
#define TINYTEST_ASSERTION_FAILED_TO_STDERR 1
#endif

#ifndef TINYTEST_ASSERTION_FAILED_STOPS_EXECUTION
#define TINYTEST_ASSERTION_FAILED_STOPS_EXECUTION 1
#endif

#define COLOR_RESET     "\033[1;0m"
#define COLOR_GRAY      "\033[1;90m"
#define COLOR_GREEN     "\033[0;32m"
#define COLOR_RED       "\033[0;91m"
#define COLOR_GREEN_B   "\033[0;92m"
#define COLOR_MAGENTA   "\033[1;95m"

/// @brief Prints the given text if the verbose flag has been set
#define test_print(text) if (verbose) std::cout << text << std::endl
/// @brief Prints that the test has passed
#define test_passed() test_print("\t" << COLOR_GREEN << "OK" << COLOR_RESET)
/// @brief Prints that the test has failed
#define test_failed() test_print("\t" << COLOR_RED << "FAILED" << COLOR_RESET)
/// @brief Prints the header for a test
#define test_header(text) test_print("\n" << COLOR_GRAY << "---- " << text << " ----" << COLOR_RESET)


/// @brief Defines what is done after an assertion fails. Internal use only.
#define _assert_condition_failed(condition) \
            test_failed(); \
            if (TINYTEST_ASSERTION_FAILED_TO_STDERR) \
                std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                        << " line " << __LINE__ << std::endl;
/// @brief Defines what is done after an assertion succeeds. Internal use only.
#define _assert_condition_passed(condition) \
            TINYTEST_TESTS_PASSED_COUNT++; \
            test_passed(); 

/**
 * @brief Implementation of the assertion function.
 * @param condition The condition of the assertion. Should evaluate to boolean.
 * @param callback What to do after the assertion fails.
 */
#define assert(condition) \
    { \
        if (!(condition)) { \
            _assert_condition_failed(condition) \
            if (TINYTEST_ASSERTION_FAILED_STOPS_EXECUTION) \
                std::terminate(); \
        } else { \
            _assert_condition_passed(condition) \
        } \
    }

/// @brief Creates a new test with an assertion and name.
#define test_assert(title, assertion) test_print(title); TINYTEST_ASSERTIONS_COUNT++; assert(assertion)

/**
 * @brief Opens a new test case in a new scope, with timer.
 * @param test_case_header The name of the test case.
 */
#define new_test_case(test_case_header) { \
    test_header(test_case_header); \
    int TINYTEST_ASSERTIONS_COUNT = 0; \
    int TINYTEST_TESTS_PASSED_COUNT = 0; \
    auto TINYTEST_START_TIMING = std::chrono::high_resolution_clock::now()
/**
 * @brief Closes a test case and the corresponding scope, and prints out the amount of tests passed, along with timing information.
 */
#define end_test_case() auto TINYTEST_STOP_TIMING = std::chrono::high_resolution_clock::now(); \
    auto TINYTEST_TIMING_DURATION = std::chrono::duration_cast<std::chrono::microseconds>(TINYTEST_STOP_TIMING - TINYTEST_START_TIMING); \
    test_print(COLOR_GRAY << "Test completed in " << COLOR_MAGENTA << TINYTEST_TIMING_DURATION.count() << COLOR_GRAY << "µs" << COLOR_RESET); \
    test_print(COLOR_GRAY << " -> " << \
        ((TINYTEST_TESTS_PASSED_COUNT == TINYTEST_ASSERTIONS_COUNT) ? COLOR_GREEN_B : COLOR_RED) << \
         TINYTEST_TESTS_PASSED_COUNT << "/" << TINYTEST_ASSERTIONS_COUNT << \
        COLOR_GRAY << " tests passed." << COLOR_RESET \
    ); \
    }

/// @brief Call after creating a new test. Allows the test framework to know whether to be verbose or not.
#define set_verbose_status() bool verbose = true; \
    if (argc > 1) { \
        if (strcmp(argv[1], "silent") || strcmp(argv[1], "quiet")) { \
            verbose = false; \
        } \
    } \
    test_print(COLOR_GRAY << "------------ TESTING FRAMEWORK ------------" << COLOR_RESET)

/// @brief Sarts a new test within the test framework. Needs a body.
#define new_test() int main(int argc, char** argv)
