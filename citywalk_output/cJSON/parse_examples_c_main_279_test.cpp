// This C++11 test harness is designed to exercise the focal C/C file
// that provides the main() function for Unity-based tests (parse_examples.c).
// Since the focal main is intended to be run as a standalone executable inside
// a Unity framework, we load the focal main symbol from a compiled shared library
// at runtime (to avoid symbol conflicts with this harness' own main). We try
// multiple candidate symbol names in case the project is built with a macro
// that renames main (e.g., -Dmain=foo).
//
// How to build and run:
// - Build parse_examples.c into a shared library (e.g., libparse_examples.so on Linux)
//   with one of the following approaches in your build system:
//     -U main                               (to keep the original name)
//     -Dmain=foo (rename main to foo)       (preferred for this harness)
//     -Dmain=parse_main                    (alternative rename)
// - Compile this harness (test_parse_examples_suite.cpp) with C++11 support.
// - Run the harness; it will load the shared library, locate the focal main symbol,
//   invoke it, and verify the return value (expected 0 for success in Unity tests).
//
// Note: This harness uses a light-weight, framework-agnostic approach (no GTest).

#include <unity/src/unity.h>
#include <cerrno>
#include <memory>
#include <unity/examples/unity_config.h>
#include <iostream>
#include <functional>
#include <vector>
#include <common.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <stdio.h>


#if defined(_WIN32) || defined(_WIN64)
  #include <windows.h>
#else
  #include <dlfcn.h>
#endif

// Candidate symbol names for the focal main after potential macro renaming.
static const char* kCandidateMainNames[] = {
#if defined(_WIN32) || defined(_WIN64)
    "main", // If not renamed on Windows
    "foo",  // Possible rename
    "parse_main"
#else
    "main",
    "foo",
    "parse_main"
#endif
};

// Typedef for the focal function signature.
// We assume C linkage (Unity.h uses C). We use a plain function pointer type.
using FocalMainFunc = int (*)();

// Holder for the loaded focal main symbol.
struct FocalMainLoader {
    void* library_handle = nullptr;
    FocalMainFunc focal_main = nullptr;

    ~FocalMainLoader() {
#if defined(_WIN32) || defined(_WIN64)
        if (library_handle) {
            FreeLibrary((HMODULE)library_handle);
        }
#else
        if (library_handle) {
            dlclose(library_handle);
        }
#endif
    }

    bool load(const std::string& library_path) {
        // Attempt to load the shared library that contains the focal main function.
#if defined(_WIN32) || defined(_WIN64)
        library_handle = (void*)::LoadLibraryA(library_path.c_str());
        if (!library_handle) {
            std::cerr << "Failed to LoadLibrary: " << library_path
                      << " Error code: " << GetLastError() << std::endl;
            return false;
        }

        // Try candidate symbol names
        for (const char* name : kCandidateMainNames) {
            FARPROC sym = GetProcAddress((HMODULE)library_handle, name);
            if (sym) {
                focal_main = reinterpret_cast<FocalMainFunc>(sym);
                return true;
            }
        }
#else
        library_handle = dlopen(library_path.c_str(), RTLD_LAZY);
        if (!library_handle) {
            std::cerr << "dlopen failed: " << dlerror() << std::endl;
            return false;
        }

        // Try candidate symbol names
        for (const char* name : kCandidateMainNames) {
            // Clear any existing error
            dlerror();
            void* sym = dlsym(library_handle, name);
            char* err = dlerror();
            if (sym && !err) {
                focal_main = reinterpret_cast<FocalMainFunc>(sym);
                return true;
            }
        }
#endif
        std::cerr << "Could not locate focal main symbol in library: " << library_path << std::endl;
        return false;
    }
};

// Helper to load and invoke the focal main.
static int call_focal_main(const std::string& library_path) {
    static FocalMainLoader loader;
    // If not yet loaded, attempt to load.
    if (!loader.focal_main) {
        if (!loader.load(library_path)) {
            return -1; // Indicate failure to load focal main
        }
    }
    // Call the focal main function. Unity will run its tests and return a status code.
    return loader.focal_main();
}

// Simple test wrapper to guarantee deterministic behavior across tests.
// Each test will try to invoke the focal main and assert a successful return.
struct TestCase {
    std::string name;
    std::string comment; // mapping/description for the test case
    std::function<bool()> run;
    bool passed;
};

// Entry point for test harness
int main() {
    // Path to the compiled Unity-based test library (parse_examples).
    // Adjust as needed for your build system. If you place the library in the
    // same directory as this executable, "./parse_examples.so" (Linux/macOS)
    // or "./parse_examples.dll" (Windows) should work after building.
#if defined(_WIN32) || defined(_WIN64)
    std::string lib_path = "./parse_examples.dll";
#else
    std::string lib_path = "./libparse_examples.so";
    // Also support a direct executable library name if your build outputs a .so with a different name.
#endif

    std::vector<TestCase> tests;

    // We create 15 test cases corresponding to the RUN_TEST entries in the focal main.
    // Each test re-invokes the focal main to ensure end-to-end Unity test execution occurs.
    // Note: All tests exercise the same focal main; differences are represented via comments.
    // In practice, these tests map to:
    // 1-5: file_test1...file_test5_should_be_parsed_and_printed
    // 6: file_test6_should_not_be_parsed
    // 7-11: file_test7...file_test11_should_be_parsed_and_printed
    // 12: test12_should_not_be_parsed
    // 13: test13_should_be_parsed_without_null_termination
    // 14-15: test14_should_not_be_parsed, test15_should_not_heap_buffer_overflow

    // Helper to create a test case
    auto make_case = [&](const std::string& name, const std::string& comment) {
        TestCase tc;
        tc.name = name;
        tc.comment = comment;
        // Each test reuses the same runner
        tc.run = [&lib_path]() -> bool {
            int ret = call_focal_main(lib_path);
            // Unity's typical convention is 0 for all tests passing.
            // If the library failed to load or return non-zero, mark as failure.
            return ret == 0;
        };
        tc.passed = false;
        return tc;
    };

    // Create test cases with descriptive comments
    tests.emplace_back(make_case("test1_should_run_and_return_zero",
        "Mapping: file_test1_should_be_parsed_and_printed - Ensure the Unity test suite starts and returns success."));
    tests.emplace_back(make_case("test2_should_run_and_return_zero",
        "Mapping: file_test2_should_be_parsed_and_printed - Ensure the Unity test suite runs and reports success."));
    tests.emplace_back(make_case("test3_should_run_and_return_zero",
        "Mapping: file_test3_should_be_parsed_and_printed - Ensure the Unity test suite runs and reports success."));
    tests.emplace_back(make_case("test4_should_run_and_return_zero",
        "Mapping: file_test4_should_be_parsed_and_printed - Ensure the Unity test suite runs and reports success."));
    tests.emplace_back(make_case("test5_should_run_and_return_zero",
        "Mapping: file_test5_should_be_parsed_and_printed - Ensure the Unity test suite runs and reports success."));
    tests.emplace_back(make_case("test6_should_run_and_return_zero",
        "Mapping: file_test6_should_not_be_parsed - Ensure the Unity test suite runs and reports success (negative test embedded in Unity)."));
    tests.emplace_back(make_case("test7_should_run_and_return_zero",
        "Mapping: file_test7_should_be_parsed_and_printed - Ensure the Unity test suite runs and reports success."));
    tests.emplace_back(make_case("test8_should_run_and_return_zero",
        "Mapping: file_test8_should_be_parsed_and_printed - Ensure the Unity test suite runs and reports success."));
    tests.emplace_back(make_case("test9_should_run_and_return_zero",
        "Mapping: file_test9_should_be_parsed_and_printed - Ensure the Unity test suite runs and reports success."));
    tests.emplace_back(make_case("test10_should_run_and_return_zero",
        "Mapping: file_test10_should_be_parsed_and_printed - Ensure the Unity test suite runs and reports success."));
    tests.emplace_back(make_case("test11_should_run_and_return_zero",
        "Mapping: file_test11_should_be_parsed_and_printed - Ensure the Unity test suite runs and reports success."));
    tests.emplace_back(make_case("test12_should_not_be_parsed",
        "Mapping: test12_should_not_be_parsed - Unity handles negative parsing scenario."));
    tests.emplace_back(make_case("test13_should_be_parsed_without_null_termination",
        "Mapping: test13_should_be_parsed_without_null_termination - Ensure Unity handles edge-case parsing."));
    tests.emplace_back(make_case("test14_should_not_be_parsed",
        "Mapping: test14_should_not_be_parsed - Ensure Unity handles invalid input gracefully."));
    tests.emplace_back(make_case("test15_should_not_heap_buffer_overflow",
        "Mapping: test15_should_not_heap_buffer_overflow - Ensure Unity doesn't overflow buffers in edge cases."));

    // Run all tests
    int total = static_cast<int>(tests.size());
    int passed = 0;

    std::cout << "Starting focal main tests via dynamic loading: " << lib_path << std::endl;

    for (auto &tc : tests) {
        bool ok = tc.run();
        tc.passed = ok;
        std::cout << "[TEST] " << tc.name << " - "
                  << (ok ? "PASS" : "FAIL") << " ("
                  << tc.comment << ")" << std::endl;
        if (ok) ++passed;
    }

    std::cout << "\nTest results: " << passed << "/" << total << " passed." << std::endl;
    if (passed == total) {
        return 0;
    } else {
        return 1;
    }
}