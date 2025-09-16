/**
 * @file TinyTest, a stupidly simple C++ testing framework, being made as a single header of only macros.
 */
#pragma once
#include <string.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <unordered_set>
#include <initializer_list>
#include <string>
#include <sstream>
#include <cmath>
#include <set>

/// @brief Current version of TinyTest. Follows [Semantic Versioning](https://semver.org/).
#define TINYTEST_VERSION "1.21.5"

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

#ifndef TINYTEST_FORCE_MICROSECOND_BENCHMARK_PRECISION
/// @brief If set to 1, benchmarks will output their value in microseconds. 
///    Otherwise, they will output the best suited unit for the job (default).
#define TINYTEST_FORCE_MICROSECOND_BENCHMARK_PRECISION 0
#endif

#ifndef TINYTEST_FORCE_MILLISECOND_BENCHMARK_PRECISION
/// @brief If set to 1, benchmarks will output their value in milliseconds, 
///     unless it is sub-millisecond, at which point the value will be outputted in microseconds.
///     Otherwise, they will output the best suited unit for the job (default).
#define TINYTEST_FORCE_MILLISECOND_BENCHMARK_PRECISION 0
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
/// @brief How many times flaky tests should be run by default
#define TINYTEST_FLAKY_TEST_ITERATIONS 10
#endif

#ifndef TINYTEST_SETUP_FUNCTION
/**
 * @brief A macro that will be run at the start of every test case, at the beginning of the test scope.
 *  This allows for a certain piece of code to set up every test case in a test unit.
 */
#define TINYTEST_SETUP_FUNCTION()
#endif

#ifndef TINYTEST_TEARDOWN_FUNCTION
/**
 * @brief A macro that will be run at the end of every test case, at the end of the test scope.
 *  This allows for a certain piece of code to tear down every test case in a test unit, 
 *  for example using delete on the correct components.
 */
#define TINYTEST_TEARDOWN_FUNCTION()
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
/// @brief Prints the given text if the verbose flag has been set, even if the important-only flag is set ON
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
#define tinytest_deprecated(old_function_name, new_function_name) [[deprecated]]; \
    test_warning_important(old_function_name << "() (line " << __LINE__ << ") is deprecated, in favor of " << old_function_name << "().")
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

/// @brief Creates a new test, with an expression that is supposed to throw an exception. Provides the ability to send an error message.
#define test_assert_throws_pro(title, expression, message_on_failure) test_assert_throws_pro_ex(title, expression, std::exception, message_on_failure << "\n")

/// @brief Creates a new test, with an expression that is supposed to throw an exception.
#define test_assert_throws(title, expression) test_assert_throws_pro(title, expression, "The assertion did not throw any exception.\n")

/** @cond PRIVATE */
#define _best_time_value(microseconds) \
    ((microseconds < 1'000 || TINYTEST_FORCE_MICROSECOND_BENCHMARK_PRECISION) ? microseconds : ((microseconds < 1'000'000 || TINYTEST_FORCE_MILLISECOND_BENCHMARK_PRECISION) ? (microseconds / 1'000) : (microseconds / 1'000'000)))
#define _best_time_unit(microseconds) \
    ((microseconds < 1'000 || TINYTEST_FORCE_MICROSECOND_BENCHMARK_PRECISION) ? "µs" : ((microseconds < 1'000'000 || TINYTEST_FORCE_MILLISECOND_BENCHMARK_PRECISION) ? "ms" : "s"))
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
    _best_time_unit(TINYTEST_TIMING_DURATION.count()) << COLOR_RESET); \
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

/** @cond PRIVATE */
#define _get_approx_complexity(input_size, call_count, approx_complexity) [&](){ \
    if (std::abs(approx_complexity) < 0.1 || call_count == 1) return "O(1)"; \
    if (std::abs(approx_complexity - 2) < 0.2 || std::abs(std::sqrt(call_count) - input_size) < 0.2) return "O(N^2)"; \
    if (std::abs(approx_complexity - 1) < 0.2) return "O(N)"; \
    if (std::abs(approx_complexity - 0.693) < 0.2) return "O(log N)"; \
    if (std::abs(approx_complexity - 1.5) < 0.2) return "O(N log N)"; \
    if (call_count > std::pow(2, input_size / 10)) return "O(2^N)";  /* Rough exponential check */ \
    return "unknown"; \
}()
/** @endcond */

/**
 * @brief Starts a VERY rough complexity check.
 * @param input_size The size of the input. In Big O notation, with O(n), n is the input size.
 * @warning The check is VERY rough, as it is done in a single pass. Results may or may not be accurate at all. Use at your own discretion.
 */
#define start_complexity_check(input_size) const unsigned int TINYTEST_COMPLEXITY_CHECK_INPUT_SIZE = input_size; unsigned int TINYTEST_COMPLEXITY_CHECK_CALL_COUNT = 0

/// @brief Call during a complexity check every time you want to incrase the complexity
#define tick_complexity_check() TINYTEST_COMPLEXITY_CHECK_CALL_COUNT++

/// @brief Stops a complexity check and outputs VERY rough results.
#define stop_complexity_check() double TINYTEST_COMPLEXITY_APPROX = std::log(TINYTEST_COMPLEXITY_CHECK_INPUT_SIZE) / std::log(TINYTEST_COMPLEXITY_CHECK_CALL_COUNT); \
    test_print(COLOR_GRAY << "Complexity check finished. Approximate complexity with input size " << TINYTEST_COMPLEXITY_CHECK_INPUT_SIZE \
        << " and call count " << TINYTEST_COMPLEXITY_CHECK_CALL_COUNT << " is " << COLOR_MAGENTA << _get_approx_complexity( \
        TINYTEST_COMPLEXITY_CHECK_INPUT_SIZE, TINYTEST_COMPLEXITY_CHECK_CALL_COUNT, TINYTEST_COMPLEXITY_APPROX \
    ) << COLOR_GRAY << "." << COLOR_RESET)

/** @cond PRIVATE */
#define _base_test_case(test_case_header, ...) [&](){ \
    __VA_ARGS__ \
    test_header(test_case_header); \
    int TINYTEST_ASSERTIONS_COUNT = 0; \
    int TINYTEST_TESTS_PASSED_COUNT = 0; \
    std::vector<std::chrono::_V2::system_clock::time_point> TINYTEST_BENCHMARK_VECTORS; \
    TINYTEST_SETUP_FUNCTION(); \
    benchmark_start()

#define should_run_test(tags, current_tag) [&]() { \
    bool returnVal = true; \
    if (tags.empty() && (current_tag != "" && current_tag.at(0) != '!')) returnVal = false; \
    if (!tags.empty() && current_tag.length() >= 1 && current_tag != "") { \
        if (TINYTEST_CURRENT_TAG.at(0) != '!' && !TINYTEST_TAGS.count(TINYTEST_CURRENT_TAG)) returnVal = false; \
        if (TINYTEST_CURRENT_TAG.at(0) == '!' && TINYTEST_TAGS.count(TINYTEST_CURRENT_TAG.substr(1))) returnVal = false; \
    } \
    return returnVal; \
}()
/** @endcond */

/**
 * @brief Opens a new test case in a new scope, with timer.
 * @param test_case_header The name of the test case.
 * @param tags An OPTIONAL list of tags to apply to the test case, separated by commas
 */
#define new_test_case(test_case_header, ...) _base_test_case(test_case_header, \
    static std::unordered_set<std::string> TINYTEST_TAGS = { __VA_ARGS__ } ; \
    if (!should_run_test(TINYTEST_TAGS, TINYTEST_CURRENT_TAG)) return TINYTEST_SKIP; \
)

/**
 * @brief Opens a new test case in a new scope, with timer. This also supports tags, and will only run if it has the correct tag
 * @param test_case_header The name of the test case.
 * @param tags A list of tags to apply to the test case, separated by commas
 * @deprecated Deprecated since version 1.14.1 ; use `new_test_case()` instead.
 */
#define new_tagged_test_case(test_case_header, ...) tinytest_deprecated("new_tagged_test_case", "new_test_case"); \
    new_test_case(test_case_header, __VA_ARGS__)

/**
 * @brief Closes a test case and the corresponding scope, and prints out the amount of tests passed, along with timing information.
 */
#define end_test_case() benchmark_stop(); \
    TINYTEST_TEARDOWN_FUNCTION(); \
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
 * @param tags A list of tags to apply to the flaky test case, separated by commas
 */
#define new_flaky_test_case_pro(test_case_header, test_case_iterations, ...) { \
    static std::unordered_set<std::string> TINYTEST_TAGS = { __VA_ARGS__ } ; \
    if (should_run_test(TINYTEST_TAGS, TINYTEST_CURRENT_TAG)) { \
        test_print_important(COLOR_GRAY << "\n\n-------------- NEW FLAKY TEST : " << test_case_header << " --------------" << COLOR_RESET); \
        static int TINYTEST_FLAKY_TEST_PASSES = 0; \
        static int TINYTEST_FLAKY_TEST_FAILS  = 0; \
        static int TINYTEST_FLAKY_TEST_SKIPS  = 0; \
        static int TINYTEST_FLAKY_TEST_TOTAL_ITERATIONS = test_case_iterations; \
        for (int TINYTEST_FLAKY_TEST_ITERATION = 0; TINYTEST_FLAKY_TEST_ITERATION < test_case_iterations; TINYTEST_FLAKY_TEST_ITERATION++) { \
            int TINYTEST_CURRENT_FLAKY_TEST_RESULT = new_test_case("Flaky Test Run " << TINYTEST_FLAKY_TEST_ITERATION + 1, __VA_ARGS__)

/**
 * @brief Creates a new flaky test case in a new scope ; basically, a test that will be run multiple times to check for race conditions and the likes
 * @param test_case_header The name of the test case.
 * @param tags A list of tags to apply to the flaky test case, separated by commas
 */
#define new_flaky_test_case(test_case_header, ...) new_flaky_test_case_pro(test_case_header, TINYTEST_FLAKY_TEST_ITERATIONS, __VA_ARGS__)

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
    COLOR_RESET); \
    }\
}

/**
 * @brief Whether a TinyTest flag is enabled.
 * @param flag_name The name of the flag, as a string (const char*).
 * @return A boolean indicating whether the flag is enabled.
 */
#define is_tinytest_flag_enabled(flag_name) (TINYTEST_ENABLED_USER_FLAGS.find(flag_name) != TINYTEST_ENABLED_USER_FLAGS.end())

/**
 * @brief Allows to specify which flags the test program can expect to be passed.
 * @param flags A variable number of flags, as strings (const char* or std::string).
 * Is used to generate the list displayed to the user when using the `show-flags` command line argument.
 */
#define tinytest_set_available_flags(...) TINYTEST_AVAILABLE_FLAGS = { __VA_ARGS__ }

/**
 * @brief Allows to specify which tags test cases in the program contain.
 * @param tags A variable number of tags, as strings (const char* or std::string).
 * Is used to generate the list displayed to the user when using the `show-tags` command line argument.
 */
#define tinytest_set_available_tags(...) TINYTEST_AVAILABLE_TAGS = { __VA_ARGS__ }

/// @brief Call after creating a new test. Allows the test framework to know whether to be verbose or not.
#define handle_command_line_args() \
    bool TINYTEST_FLAG_VERBOSE = true; \
    bool TINYTEST_FLAG_SHORTEN = false; \
    bool TINYTEST_FLAG_ERROR_ONLY = false; \
    bool TINYTEST_FLAG_IMPORTANT_ONLY = false; \
    std::string TINYTEST_CURRENT_TAG = ""; \
    std::unordered_set<std::string> TINYTEST_ENABLED_USER_FLAGS = {};\
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
        else if (strncmp(argv[i], "tag:", strlen("tag:")) == 0) { \
            TINYTEST_CURRENT_TAG = std::string(argv[i] + strlen("tag:")); \
            if (TINYTEST_CURRENT_TAG == "*") \
                TINYTEST_CURRENT_TAG = ""; \
        } \
        else if (strncmp(argv[i], "-t:", strlen("-t:")) == 0) { \
            TINYTEST_CURRENT_TAG = std::string(argv[i] + strlen("-t:")); \
            if (TINYTEST_CURRENT_TAG == "*") \
                TINYTEST_CURRENT_TAG = ""; \
        } \
        else if (strncmp(argv[i], "flags:", strlen("flags:")) == 0) { \
            std::stringstream tinytest_flags(argv[i] + strlen("flags:")); \
            std::string tinytest_current_flag; \
            while (std::getline(tinytest_flags, tinytest_current_flag, ',')) \
                TINYTEST_ENABLED_USER_FLAGS.insert(tinytest_current_flag); \
        } \
        else if (strncmp(argv[i], "-f:", strlen("-f:")) == 0) { \
            std::stringstream tinytest_flags(argv[i] + strlen("-f:")); \
            std::string tinytest_current_flag; \
            while (std::getline(tinytest_flags, tinytest_current_flag, ',')) \
                TINYTEST_ENABLED_USER_FLAGS.insert(tinytest_current_flag); \
        } \
        else if (strcmp(argv[i], "show-flags") == 0 || strcmp(argv[i], "available-flags") == 0 || strcmp(argv[i], "flags") == 0) { \
            if (!TINYTEST_AVAILABLE_FLAGS.empty()) { \
                std::cout << "Available flags :\n"; \
                for (const std::string& currentFlag : TINYTEST_AVAILABLE_FLAGS) \
                    std::cout << "- " << currentFlag << "\n"; \
            } \
            else std::cout << "This test program doesn't seem to implement any flags." << std::endl; \
            std::cout << std::flush; \
            return 0; \
        } \
        else if (strcmp(argv[i], "show-tags") == 0 || strcmp(argv[i], "available-tags") == 0 || strcmp(argv[i], "tags") == 0) { \
            if (!TINYTEST_AVAILABLE_TAGS.empty()) { \
                std::cout << "Available tags :\n"; \
                for (const std::string& currentTag : TINYTEST_AVAILABLE_TAGS) \
                    std::cout << "- " << currentTag << "\n"; \
            } \
            else std::cout << "This test program doesn't seem to implement any tags." << std::endl; \
            std::cout << std::flush; \
            return 0; \
        } \
        else if (strcmp(argv[i], "help") == 0 || strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) { \
            std::cout << "TinyTest CLI arguments :\n" \
            << "- help, -h :\n\tShows this message\n" \
            << "- version :\n\tShows the current TinyTest version\n" \
            << "- silent, quiet, -q :\n\tDoes not write anything to the standard output\n" \
            << "- verbose, -v :\n\tWrites to the standad output. Default behaviour.\n" \
            << "- summary, shorten, short, -s :\n\tRemoves the long details from failed asserts. Failed asserts will only show the 'FAILED' message.\n" \
            << "- errors, error-only, -e :\n\tONLY shows the long details from failed asserts.\n" \
            << "- important-only, important, -i :\n\tOnly shows test case names and statuses ; a.k.a the most important stuff. Helps summarize in case of long tests.\n" \
            << "- show-flags, available-flags, flags :\n\tShows which flags the test program can receive. Not every program will implement this.\n" \
            << "- show-tags, available-tags, tags :\n\tShows which flags the test program can receive. Not every program will implement this.\n" \
            << "- tag:<tag>, -t:<tag> :\n\tOnly runs test with the corresponding tag. <tag> should be a valid string.\n\tPrefix the <tag> by a '!' to make all tags run except this one.\n\tLeave empty or use '*' to run all tags.\n" \
            << "- flags:<flags>, -f:<flags> :\n\tEnables the given tags. These should be one word, separated by commas.\n" \
            << std::endl; \
            return 0; \
        } \
        else if (strcmp(argv[i], "version") == 0 || strcmp(argv[i], "--version") == 0) { \
            std::cout << "TinyTest version " << TINYTEST_VERSION << std::endl; \
            return 0; \
        } \
    } \
    test_print_important(COLOR_GRAY << "------------ TinyTest Results ------------" << COLOR_RESET)

/**
 * @brief Sarts a new test within the test framework. Needs a body.
 * @warning This is by all means a `main` function. Make sure there is no other main function in your program.
 */
#define new_test() static bool TINYTEST_ALL_TESTS_PASSED = true; \
    static std::set<std::string> TINYTEST_AVAILABLE_FLAGS = {}; \
    static std::set<std::string> TINYTEST_AVAILABLE_TAGS = {}; \
    int main(int argc, char** argv)

/// @brief To be called after every test has run. Terminates the testing process with code 0 if all tests passed, and code 1 if at least one test failed.
#define end_of_all_tests() return (TINYTEST_ALL_TESTS_PASSED) ? 0 : 1