/**
 * @file TinyTest, a stupidly simple C++ testing framework, being made as a single header of only macros.
 */
#pragma once
#include <string.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>

/// @brief Current version of TinyTest. Follows [Semantic Versioning](https://semver.org/).
#define TINYTEST_VERSION "1.12.1"

#ifndef TINYTEST_ASSERTION_FAILED_TO_STDERR
/// @brief When an assertion fails, some output gets generated and sent to stderr. Setting this constant to 0 disables this behaviour.
#define TINYTEST_ASSERTION_FAILED_TO_STDERR 1
#endif

#ifndef TINYTEST_ASSERTION_FAILED_STOPS_EXECUTION
/// @brief As soon as an assertion fails, execution gets stopped. Setting this constant to 0 disables this behaviour.
#define TINYTEST_ASSERTION_FAILED_STOPS_EXECUTION 1
#endif

#ifndef TINYTEST_COLORIZE_STDERR
/// @brief Whether or not to colorize the stderr output.
#define TINYTEST_COLORIZE_STDERR 1
#endif

#ifndef TINYTEST_STANDARD_OUTPUT
/// @brief The stream that will be the standard output for TinyTest. Should be an std::ostream. Default is std::cout.
#define TINYTEST_STANDARD_OUTPUT std::cout
#endif

#ifndef TINYTEST_STANDARD_ERROR
/// @brief The stream that will be the standard error output for TinyTest. Should be an std::ostream. Default is std::cerr.
#define TINYTEST_STANDARD_ERROR std::cerr
#endif

#ifndef TINYTEST_FLAKY_TEST_ITERATIONS
/// @brief How many times flaky tests should be run
#define TINYTEST_FLAKY_TEST_ITERATIONS 10
#endif

#define COLOR_RESET     "\033[1;0m"
#define COLOR_GRAY      "\033[1;90m"
#define COLOR_GREEN     "\033[0;32m"
#define COLOR_GREEN_B   "\033[0;92m"
#define COLOR_RED       "\033[0;91m"
#define COLOR_YELLOW    "\033[1;33m"
#define COLOR_MAGENTA   "\033[1;95m"

#define TINYTEST_SKIP -1
#define TINYTEST_FAIL  0
#define TINYTEST_PASS  1

/// @brief Prints the given text if the verbose flag has been set
#define test_print(text) if (!TINYTEST_FLAG_IMPORTANT_ONLY && TINYTEST_FLAG_VERBOSE) TINYTEST_STANDARD_OUTPUT << text << std::endl
/// @brief Prints the given text if the verbose flag has been set
#define test_print_important(text) if (TINYTEST_FLAG_VERBOSE) TINYTEST_STANDARD_OUTPUT << text << std::endl
/// @brief Prints that the test has passed
#define test_passed() test_print("\t" << COLOR_GREEN << "OK" << COLOR_RESET)
/// @brief Prints that the test has failed
#define test_failed() test_print("\t" << COLOR_RED << "FAILED" << COLOR_RESET)
/// @brief Prints the header for a test
#define test_header(text) test_print_important("\n" << COLOR_GRAY << "---- " << text << " ----" << COLOR_RESET)
/// @brief Prints a warning to the user
#define test_warning(text) test_print(COLOR_YELLOW << "WARNING: " << text << COLOR_RESET)
/// @brief Prints a warning to the user, even if the important-only flag is set ON
#define test_warning_important(text) test_print_important(COLOR_YELLOW << "WARNING: " << text << COLOR_RESET)


/** @cond PRIVATE */
#define _stderr_color(tinytest_color) ((TINYTEST_COLORIZE_STDERR) ? tinytest_color : "")
#define _line() "--------------------------------------------------------------------------------------------------------"
#define _small_line() "--------------------------------------------------------------"
/// @brief Defines what is done after an assertion fails. Internal use only.
#define _assert_condition_failed(condition, additional_info) \
            TINYTEST_ALL_TESTS_PASSED = false; \
            test_failed(); \
            if ((TINYTEST_ASSERTION_FAILED_TO_STDERR && !TINYTEST_FLAG_SHORTEN && TINYTEST_FLAG_VERBOSE) || TINYTEST_FLAG_ERROR_ONLY) \
                TINYTEST_STANDARD_ERROR << _stderr_color(COLOR_RED) << _line() << "\nOn file: " << __FILE__ << " - Line " << _stderr_color(COLOR_MAGENTA) << __LINE__ << "\n" << \
                    _stderr_color(COLOR_RED) << "Assertion failed: `" << _stderr_color(COLOR_YELLOW) << #condition << _stderr_color(COLOR_RED) << "`\n" \
                    << additional_info << _line() << _stderr_color(COLOR_RESET) << std::endl;
/// @brief Defines what is done after an assertion succeeds. Internal use only.
#define _assert_condition_passed(condition) \
            TINYTEST_TESTS_PASSED_COUNT++; \
            test_passed(); 
/** @endcond */

/**
 * @brief Implementation of the assertion function.
 * @param condition The condition of the assertion. Should evaluate to boolean.
 * @param additional_message_on_failure What to do after the assertion fails.
 */
#define assert(condition, additional_message_on_failure) \
    { \
        if (!(condition)) { \
            _assert_condition_failed(condition, additional_message_on_failure) \
            if (TINYTEST_ASSERTION_FAILED_STOPS_EXECUTION) \
                std::terminate(); \
        } else { \
            _assert_condition_passed(condition) \
        } \
    }

/** @cond PRIVATE */
/// @brief Internal use only. Gets called by test_assert and test_assert_pro.
#define _base_test_assert(title) test_print(title); TINYTEST_ASSERTIONS_COUNT++
/** @endcond */
/// @brief Creates a new test with an assertion and name.
#define test_assert(title, assertion) _base_test_assert(title); assert(assertion, "")
/// @brief Creates a new test with an assertion and name, along with something that should be added to the stderr upon failure
#define test_assert_pro(title, assertion, additional_message_on_failure) _base_test_assert(title); assert(assertion, "Additional info:\n" << additional_message_on_failure << "\n")
/**
 * @brief Creates a new test for the value of a variable
 * @param title The title of this assertion
 * @param variable The variable whose value is to be tested
 * @param operation A C++ operator to apply on the variable, e.g. ==
 * @param value The value to test the variable against
 * test_assert_var("Tests that a is still equal to 1", a, ==, 1)
 */
#define test_assert_var(title, variable, operation, value) _base_test_assert(title); assert((variable operation value), "Additional info:\n" << #variable << " = " << variable << "\n")

/**
 * @brief Creates a new test, with an expression that is supposed to throw an exception. Allows to specify which exception, and provide an error message.
 * @param title The title of this assertion
 * @param expression An expression that is supposed to throw an exception
 * @param exception_type A subclass of std::exception (or itself). e.g. std::out_of_bounds
 * @param message_on_failure A message to be displayed if the assertion fails
*/
#define test_assert_throws_pro_ex(title, expression, exception_type, message_on_failure) _base_test_assert(title); \
    { \
        bool TINYTEST_ASSERT_THROWS_PASSED = false; \
        try { \
            expression; \
        } catch ( exception_type _ ) { \
            TINYTEST_ASSERT_THROWS_PASSED = true; \
        } \
        if (TINYTEST_ASSERT_THROWS_PASSED) { \
            _assert_condition_passed(expression); \
        } else { \
            _assert_condition_failed(expression, message_on_failure << "\n"); \
            if (TINYTEST_ASSERTION_FAILED_STOPS_EXECUTION) \
                std::terminate(); \
        } \
    }

/// @brief Creates a new test, with an expression that is supposed to throw an error. Provides the ability to send an error message
#define test_assert_throws_pro(title, expression, message_on_failure) test_assert_throws_pro_ex(title, expression, std::exception, message_on_failure << "\n")

/// @brief Creates a new test, with an expression that is supposed to throw an error
#define test_assert_throws(title, expression) test_assert_throws_pro(title, expression, "The assertion did not throw any exception.\n")

/** @cond PRIVATE */
#define _best_time_value(microseconds) \
    ((microseconds < 1'000) ? microseconds : ((microseconds < 1'000'000) ? (microseconds / 1'000) : (microseconds / 1'000'000)))
#define _best_time_unit(microseconds) \
    ((microseconds < 1'000) ? "µs" : ((microseconds < 1'000'000) ? "ms" : "s"))
/** @endcond */


/**
 * @brief Starts a high resolution timer to benchmark your code
 */
#define benchmark_start() \
    TINYTEST_BENCHMARK_VECTORS.push_back(std::chrono::high_resolution_clock::now()); \
    if (TINYTEST_BENCHMARK_VECTORS.size() != 1) \
        test_print_important(COLOR_GRAY << "Benchmark started with id #" << (TINYTEST_BENCHMARK_VECTORS.size() - 1) << COLOR_RESET)

/**
 * @brief Stops the current benchmark and displays the time it took to execute
 */
#define benchmark_stop() { \
    auto TINYTEST_STOP_TIMING = std::chrono::high_resolution_clock::now(); \
    int TINYTEST_CURRENT_BENCHMARK = TINYTEST_BENCHMARK_VECTORS.size() - 1; \
    auto TINYTEST_TIMING_DURATION = std::chrono::duration_cast<std::chrono::microseconds>(TINYTEST_STOP_TIMING - TINYTEST_BENCHMARK_VECTORS[TINYTEST_CURRENT_BENCHMARK]); \
    test_print_important(COLOR_GRAY << \
    ((TINYTEST_CURRENT_BENCHMARK == 0) ? "Test" : "Benchmark id #") << ((TINYTEST_CURRENT_BENCHMARK == 0) ? "" : std::to_string(TINYTEST_CURRENT_BENCHMARK).c_str()) << " completed in " \
     << COLOR_MAGENTA << _best_time_value(TINYTEST_TIMING_DURATION.count()) << \
    COLOR_GRAY << _best_time_unit(TINYTEST_TIMING_DURATION.count()) << COLOR_RESET); \
    TINYTEST_BENCHMARK_VECTORS.pop_back(); \
}

/**
 * @brief Starts a "long" benchmark with many iterations of a code block to give more accurate benchmarking results
 * @param iterations The amount of iterations to benchmark
 * @warning DO NOT PLACE A LONG BENCHMARK WITHIN ANOTHER LONG BENCHMARK ! This will create an infinite loop !
 */
#define benchmark_long_start(iterations) benchmark_start(); for (long long TINYTEST_BENCHMARK_I = 0; TINYTEST_BENCHMARK_I < iterations; TINYTEST_BENCHMARK_I++) {

/**
 * @brief Marks the end of a "long" benchmark created with the `benchmark_long_start()` macro
 */
#define benchmark_long_stop() } benchmark_stop()

/**
 * @brief Opens a new test case in a new scope, with timer.
 * @param test_case_header The name of the test case.
 */
#define new_test_case(test_case_header) [&](){ \
    test_header(test_case_header); \
    int TINYTEST_ASSERTIONS_COUNT = 0; \
    int TINYTEST_TESTS_PASSED_COUNT = 0; \
    std::vector<std::chrono::_V2::system_clock::time_point> TINYTEST_BENCHMARK_VECTORS; \
    benchmark_start()
/**
 * @brief Closes a test case and the corresponding scope, and prints out the amount of tests passed, along with timing information.
 */
#define end_test_case() benchmark_stop(); \
    test_print_important(COLOR_GRAY << " -> " << \
        ((TINYTEST_TESTS_PASSED_COUNT == TINYTEST_ASSERTIONS_COUNT) ? COLOR_GREEN_B : COLOR_RED) << \
         TINYTEST_TESTS_PASSED_COUNT << "/" << TINYTEST_ASSERTIONS_COUNT << \
        COLOR_GRAY << " tests passed." << COLOR_RESET \
    ); \
    return (TINYTEST_TESTS_PASSED_COUNT == TINYTEST_ASSERTIONS_COUNT) ? TINYTEST_PASS : TINYTEST_FAIL ; \
    }()
/**
 * @brief Skips the current test case.
 */
#define skip_test_case() test_print_important(COLOR_GRAY << "TEST CASE SKIPPED" << COLOR_RESET); return TINYTEST_SKIP

/**
 * @brief Creates a new flaky test case in a new scope ; basically, a test that will be run multiple times to check for race conditions and the likes
 * @param test_case_header The name of the test case.
 * @param test_case_iterations The amount of iterations that the flaky test should perform
 */
#define new_flaky_test_case_pro(test_case_header, test_case_iterations) { \
    test_print_important(COLOR_GRAY << "\n\n-------------- NEW FLAKY TEST : " << test_case_header << " --------------" << COLOR_RESET); \
    static int TINYTEST_FLAKY_TEST_PASSES = 0; \
    static int TINYTEST_FLAKY_TEST_FAILS  = 0; \
    static int TINYTEST_FLAKY_TEST_SKIPS  = 0; \
    static int TINYTEST_FLAKY_TEST_TOTAL_ITERATIONS = test_case_iterations; \
    for (int TINYTEST_FLAKY_TEST_ITERATION = 0; TINYTEST_FLAKY_TEST_ITERATION < test_case_iterations; TINYTEST_FLAKY_TEST_ITERATION++) { \
        int TINYTEST_CURRENT_FLAKY_TEST_RESULT = new_test_case("Flaky Test Run " << TINYTEST_FLAKY_TEST_ITERATION + 1)

/**
 * @brief Creates a new flaky test case in a new scope ; basically, a test that will be run multiple times to check for race conditions and the likes
 * @param test_case_header The name of the test case.
 */
#define new_flaky_test_case(test_case_header) new_flaky_test_case_pro(test_case_header, TINYTEST_FLAKY_TEST_ITERATIONS)

/**
 * @brief Closes a flaky test case, and prints out the amount of test cases passed, failed, and skipped
 */
#define end_flaky_test_case() end_test_case(); \
    if (TINYTEST_CURRENT_FLAKY_TEST_RESULT == TINYTEST_PASS) TINYTEST_FLAKY_TEST_PASSES++; \
    if (TINYTEST_CURRENT_FLAKY_TEST_RESULT == TINYTEST_FAIL) TINYTEST_FLAKY_TEST_FAILS++; \
    if (TINYTEST_CURRENT_FLAKY_TEST_RESULT == TINYTEST_SKIP) TINYTEST_FLAKY_TEST_SKIPS++; \
    } \
    test_print_important(COLOR_GRAY << "\n\n" << _small_line() << "\n\tPassed: " << COLOR_GREEN << TINYTEST_FLAKY_TEST_PASSES << "/" << TINYTEST_FLAKY_TEST_TOTAL_ITERATIONS << \
        COLOR_GRAY << ", Failed: " << COLOR_RED << TINYTEST_FLAKY_TEST_FAILS << "/" << TINYTEST_FLAKY_TEST_TOTAL_ITERATIONS << \
        COLOR_GRAY << ", Skipped: " << TINYTEST_FLAKY_TEST_SKIPS << "/" << TINYTEST_FLAKY_TEST_TOTAL_ITERATIONS << \
    COLOR_RESET); }

/// @brief Call after creating a new test. Allows the test framework to know whether to be verbose or not.
#define handle_command_line_args() \
    bool TINYTEST_FLAG_VERBOSE = true; \
    bool TINYTEST_FLAG_SHORTEN = false; \
    bool TINYTEST_FLAG_ERROR_ONLY = false; \
    bool TINYTEST_FLAG_IMPORTANT_ONLY = false; \
    for (int i = 1; i < argc; i++) { \
        if (strcmp(argv[i], "silent") == 0 || strcmp(argv[i], "quiet") == 0 || strcmp(argv[i], "-q") == 0) { \
            TINYTEST_FLAG_VERBOSE = false; \
        } \
        else if (strcmp(argv[i], "verbose") == 0 || strcmp(argv[i], "-v") == 0) { \
            TINYTEST_FLAG_VERBOSE = true; \
        } \
        else if (strcmp(argv[i], "summary") == 0 || strcmp(argv[i], "shorten") == 0 || strcmp(argv[i], "short") == 0 || strcmp(argv[i], "-s") == 0) { \
            TINYTEST_FLAG_SHORTEN = true; \
        } \
        else if (strcmp(argv[i], "errors") == 0 || strcmp(argv[i], "error-only") == 0 || strcmp(argv[i], "-e") == 0) { \
            TINYTEST_FLAG_VERBOSE = false; \
            TINYTEST_FLAG_ERROR_ONLY = true; \
        } \
        else if (strcmp(argv[i], "important-only") == 0 || strcmp(argv[i], "important") == 0 || strcmp(argv[i], "-i") == 0) { \
            TINYTEST_FLAG_IMPORTANT_ONLY = true; \
        } \
        else if (strcmp(argv[i], "help") == 0 || strcmp(argv[i], "-h") == 0) { \
            std::cout << "TinyTest CLI arguments :\n" \
            << "- help, -h :\n\tShows this message\n" \
            << "- version :\n\tShows the current TinyTest version\n" \
            << "- silent, quiet, -q :\n\tDoes not write anything to the standard output\n" \
            << "- verbose, -v :\n\tWrites to the standad output. Default behaviour.\n" \
            << "- summary, shorten, short, -s :\n\tRemoves the long details from failed asserts. Failed asserts will only show the 'FAILED' message.\n" \
            << "- errors, error-only, -e :\n\tONLY shows the long details from failed asserts.\n" \
            << "- important-only, important, -i :\n\tOnly shows test case names and statuses ; a.k.a the most important stuff. Helps summarize in case of long tests.\n" \
            << std::endl; \
            return 0; \
        } \
        else if (strcmp(argv[i], "version") == 0 || strcmp(argv[i], "--version") == 0) { \
            std::cout << "TinyTest version " << TINYTEST_VERSION << std::endl; \
            return 0; \
        } \
    } \
    test_print_important(COLOR_GRAY << "------------ TinyTest Results ------------" << COLOR_RESET)

/// @brief Sarts a new test within the test framework. Needs a body.
#define new_test() static bool TINYTEST_ALL_TESTS_PASSED = true; int main(int argc, char** argv)

/// @brief To be called after every test has run. Terminates the testing process with code 0 if all tests passed, and code 1 if at least one test failed.
#define end_of_all_tests() return (TINYTEST_ALL_TESTS_PASSED) ? 0 : 1