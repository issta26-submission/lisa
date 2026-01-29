// Simple C++11 test suite for the C function sqlite3_normalize
// The tests are designed to be run in a project where SQLite's core
// library (including sqlite3_normalize and memory helpers) is available.
// This test avoids GoogleTest/GMock and uses a minimal, self-contained
// test harness. It exercises a few representative inputs to validate
// core normalization behavior.
//
// Build notes (example):
//   g++ -std=c++11 -I/path/to/sqlite/include test_normalize.cpp -L/path/to/sqlite/lib -lsqlite3 -o test_normalize
//
// The tests rely on sqlite3_free being available; ensure the SQLite build provides it.
//
// Author note: This test focuses on functional outputs of sqlite3_normalize
// and does not attempt to mock internal static helpers. It uses the
// public API only.

#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <cassert>


// Declaration of the C function under test
extern "C" {
    // Normalized SQL is allocated with SQLite's allocator and must be freed with sqlite3_free
    char* sqlite3_normalize(const char* zSql);
    void sqlite3_free(void*);
}

// Simple test harness
static int g_tests_run = 0;
static int g_tests_passed = 0;

static void runTest(const std::string& testName,
                    const std::string& input,
                    const std::string& expected)
{
    ++g_tests_run;
    char* z = sqlite3_normalize(input.c_str());
    if (z == nullptr) {
        std::cerr << "[FAIL] " << testName << " - sqlite3_normalize returned NULL for input: " << input << "\n";
        return;
    }
    std::string actual(z);
    sqlite3_free(z);

    if (actual == expected) {
        ++g_tests_passed;
        std::cout << "[PASS] " << testName << "\n";
    } else {
        std::cerr << "[FAIL] " << testName << "\n"
                  << "  Input:    \"" << input << "\"\n"
                  << "  Expected: \"" << expected << "\"\n"
                  << "  Actual:   \"" << actual << "\"\n";
    }
}

// Convenience: run a batch of tests
static void runAllTests()
{
    // Test 1: Basic numeric literal is replaced with '?', keyword lowercased
    // Input: "SELECT 1" -> "select?;"
    runTest("Basic numeric literal",
            "SELECT 1",
            "select?;");

    // Test 2: Simple identifier is preserved in lowercase with semicolon
    // Input: "abc" -> "abc;"
    runTest("Plain identifier to lowercase",
            "abc",
            "abc;");

    // Test 3: String literal is replaced with '?', lowercasing of keywords
    // Input: "SELECT 'hello'" -> "select?;"
    runTest("String literal becomes '?'",
            "SELECT 'hello'",
            "select?;");

    // Test 4: NULL literal becomes '?'
    // Input: "NULL" -> "?;"
    runTest("NULL literal becomes '?'", 
            "NULL",
            "?;");

    // Test 5: Single numeric input (no keyword) -> "?" + semicolon
    runTest("Single numeric literal",
            "1",
            "?;");

    // Test 6: Input with trailing semicolon is preserved (not duplicated)
    // Input: "SELECT 1;" -> "select?;" (same as without semicolon)
    runTest("Trailing semicolon preserved",
            "SELECT 1;",
            "select?;");

    // Test 7: Two identifiers separated by space
    // Input: "abc def" -> "abc def;" (space preserved between identifiers)
    runTest("Two identifiers with explicit space preserved",
            "abc def",
            "abc def;");

    // Optional: more tests can be added here to cover additional branches
}

// Entry point
int main()
{
    runAllTests();

    std::cout << "\nTest summary: " << g_tests_passed << " / " << g_tests_run << " tests passed.\n";
    return (g_tests_run == g_tests_passed) ? 0 : 1;
}