# TinyTest
**TinyTest** is a stupidly simple C++ testing framework, being made as a single header of only macros.

## How to use
### Install
Simply put `tinytest.hpp` in your include directory, and you're good to go !

### Use
#### Initialization
Creates a new C++ file. This file will be your new application entry point for testing.  
Modify any of the preprocessor constants defined by **TinyTest**, if you wish to do so.  
Include `tinytest.hpp` in your file.  

Create a `new_test()`. By all intents and purposes, **this is your main function**.  
It can only be defined once, and requires a function body.  

Your file should now look something like this :  
```cpp
// Your tinytest preprocessor definitions...
#include <tinytest.hpp>

// Other imports...

new_test() {

}
```

Add a call to the `handle_command_line_args()` function at the top of your `new_test()`.  
Your code should now look like this :  
```cpp
// Your tinytest preprocessor definitions...
#include <tinytest.hpp>

// Other imports...

new_test() {
    handle_command_line_args();
}
```

Finally, leave a call to the `end_of_all_tests()` function at the very end of your `new_test()` scope.  
This function should be the last ever "thing" executed by your test program.  

Your code should now look like this :  
```cpp
// Your tinytest preprocessor definitions...
#include <tinytest.hpp>

// Other imports...

new_test() {
    handle_command_line_args();

    // Many test cases...

    end_of_all_tests();
}
```

#### Creating tests
Create a new test case using the `new_test_case(name)` macro.  
This will effectively create a new empty scope for you to make your tests.  

Close the scope using the `end_test_case()` macro.

Your code should now look like this :  
```cpp
// Your tinytest preprocessor definitions...
#include <tinytest.hpp>

// Other imports...

new_test() {
    handle_command_line_args();

    new_test_case("My new test case");
        // Your tests here
    end_test_case();

    end_of_all_tests();
}
```

#### Using the assertions
Within test cases, you can create assertions to check if your code is behaving right.  
You can mainly use 3 macros for this purpose :
- `test_assert(title, assertion)` : Verifies any given assertion. The title is the name of the assertion check, as a string.
- `test_assert_var(title, variable, operator, value)` : Tests a variable against a value. Provides better error messages on failure.
  - `variable` corresponds to an existing variable ;
  - `operator` corresponds to a C++ operator, such as `==`, `>`, etc. ;
  - `value` corresponds to the value the variable is tested against.
- `test_assert_pro(title, assertion, additional_message_on_failure)` : Same as `test_assert()`, but lets you send additional info to the standard error stream on failure.
- `test_assert_throws(title, expression)` and variants : Checks whether the given expression throws an exception ; if so, the test passes, otherwise, the test fails.

**Example :**  
```cpp
// Your tinytest preprocessor definitions...
#include <tinytest.hpp>

// Other imports...

new_test() {
    handle_command_line_args();

    new_test_case("My new test case");
        int a = 0;
        test_assert("Checking operator+ on int variables", a + 2 == 2);

        a++;
        test_assert_var("Checking value of a", a, !=, 0);

        a++;
        test_assert_pro("Checking evenness of a", a % 2 == 0, "Variable a is not even, since a = " << a);

        test_assert_throws("Checking zero division error", a / 0);
    end_test_case();

    end_of_all_tests();
}
```
