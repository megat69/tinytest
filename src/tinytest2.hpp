#include <iostream>

#define tinytest_print(text) std::cout << text << std::endl

class Test {
private:
    /// @brief The head of the linked list of tests
    static Test* head;

public:
    /// @brief Pointer to the function executing the test
    void (*callback)();

    /// @brief The name of the test
    const char* name;

    /// @brief The next element in the linked list of tests
    Test* nextTest;

    /**
     * @brief Constructs a new test.
     * @param callback A function pointer towards the function containing the test.
     * @param name The name of the test.
     */
    Test(void (*callback_)(), const char* name_) : callback{ callback_ }, name{ name_ }, nextTest{ nullptr } {
        registerTest(this);
    }

    /**
     * @brief Registers a new test to the list of tests.
     * @param test A new test.
     * @internal Pushes the new test at the top of the tests stack.
     */
    static void registerTest(Test* test) {
        test->nextTest = head;
        head = test;
    }

    /**
     * @brief Runs every test registered in the linked list of tests and provides information about which passed and failed.
     */
    static void runAllTests() {
        int testsPassed = 0;
        int testsFailed = 0;
        int totalTests = 0;

        // Fetches each test, one by one
        for (Test* test = head; test != nullptr; test = test->nextTest) {
            tinytest_print("[ RUNNING ] " << test->name);
            totalTests++;

            try {
                test->callback();  // Runs the test
                tinytest_print("[   PASS  ] " << test->name);
                testsPassed++;
            } catch (const char* error) {
                tinytest_print("[  FAIL   ] " << test->name << " - " << error);
                testsFailed++;
            }
        }

        // Summary of test results
        tinytest_print("\nSummary: " << testsPassed << "/" << totalTests << " passed, " << testsFailed << "/" << totalTests << " failed.");
    }
};

// Initializes the test pointer
Test* Test::head = nullptr;

/**
 * @brief Creates a test with the given name. The call to this macro should be followed by a function body.
 * @param name The name of the test. Should be a valid non-existent variable name, preferrably written in PascalCase.
 */
#define TEST(name) \
    void name(); \
    static Test test_##name(name, #name); \
    void name()

// --------------------------------------------------------- ASSERTIONS ---------------------------------------------------------
#define ASSERT(message, assertion)          tinytest_print(message); assertion
#define ASSERT_TRUE(x)                      if (!(x))        throw "Assertion failed: " #x " is not true."
#define ASSERT_FALSE(x)                     if ((x))         throw "Assertion failed: " #x " is not false."
#define ASSERT_EQUAL(a, b)                  if ((a) != (b))  throw "Assertion EQUAL failed: " #a " != " #b
#define ASSERT_NOT_EQUAL(a, b)              if ((a) == (b))  throw "Assertion NOT EQUAL failed: " #a " == " #b
#define ASSERT_LESS_THAN(a, b)              if ((a) >= (b))  throw "Assertion LESS THAN failed: " #a " >= " #b
#define ASSERT_LESS_THAN_OR_EQUAL(a, b)     if ((a) > (b))   throw "Assertion LESS THAN OR EQUAL failed: " #a " > " #b
#define ASSERT_GREATER_THAN(a, b)           if ((a) <= (b))  throw "Assertion GREATER THAN failed: " #a " <= " #b
#define ASSERT_GREATER_THAN_OR_EQUAL(a, b)  if ((a) < (b))   throw "Assertion GREATER THAN OR EQUAL failed: " #a " < " #b
#define ASSERT_THROWS(x)                    bool fails = true; try { x; } catch (auto e) { fails = false; }; if (fails) \
                                                throw "Assertion THROWS failed: " #x " did not throw an exception."
