/*
  Unit test suite for the focal method sqlite3_uri_boolean implemented in this file.
  This test harness provides minimal, self-contained C/C++17/C++11-compatible stubs
  for the dependent functions declared in the focal class dependencies block,
  then exercises sqlite3_uri_boolean under controlled scenarios.

  Key design decisions reflecting the instructions:
  - Lightweight, non-terminating test assertions (no aborts on failure).
  - Tests cover both branches of the conditional inside sqlite3_uri_boolean:
      - Parameter exists (z != nullptr)
      - Parameter does not exist (z == nullptr)
  - Coverage of true/false and non-boolean string inputs via sqlite3GetBoolean.
  - Static members/functions within the tested TU are treated as normal symbols to
    allow direct testing in this single translation unit.
  - All tests are invoked from main() to avoid requiring an external test framework.
*/

#include <cctype>
#include <cstring>
#include <sqliteInt.h>
#include <string>
#include <iostream>


// Domain knowledge hints: implement the required dependencies with correct behavior.

// Forward declare the API surface used by sqlite3_uri_boolean (as in the focal method).
extern "C" {
    // Dependency: extracts the value for a given parameter from a URI-like filename.
    const char *sqlite3_uri_parameter(const char *zFilename, const char *zParam);

    // Dependency: converts a string to a boolean value with a default fallback.
    int sqlite3GetBoolean(const char *z, int bDflt);

    // The focal method under test (redefined here for a self-contained TU).
    int sqlite3_uri_boolean(const char *zFilename, const char *zParam, int bDflt);
}

// Minimal, self-contained implementation of sqlite3_uri_parameter.
// This stub understands a URI with query parameters after '?', e.g. "...?param=value&other=val".
extern "C" const char *sqlite3_uri_parameter(const char *zFilename, const char *zParam){
    if(!zFilename || !zParam) return nullptr;

    const char *q = std::strchr(zFilename, '?');
    if(!q) return nullptr;      // no query portion -> parameter not present
    q++; // skip '?'

    size_t paramLen = std::strlen(zParam);

    // Iterate over parameters separated by '&'
    while (true) {
        // Find '=' for current parameter value
        const char *eq = std::strchr(q, '=');
        if(!eq) {
            // No '=' found for this segment; move to next if possible
            const char *amp = std::strchr(q, '&');
            if(!amp) break;
            q = amp + 1;
            continue;
        }

        // Parameter name length for comparison
        size_t nameLen = (size_t)(eq - q);
        // Compare parameter name with zParam
        if (nameLen == paramLen && std::strncmp(q, zParam, paramLen) == 0 && zParam[paramLen] == '\0') {
            // Return pointer to the value string
            return eq + 1;
        }

        // Move to next parameter
        const char *amp = std::strchr(eq, '&');
        if(!amp) break;
        q = amp + 1;
    }

    return nullptr;
}

// Helper: trim and lowercase conversion utility
static std::string toLowerCopy(const std::string &s){
    std::string t;
    t.reserve(s.size());
    for(unsigned char c : s) t.push_back(static_cast<char>(std::tolower(c)));
    return t;
}

// Minimal, self-contained implementation of sqlite3GetBoolean.
// Returns parsed boolean or the default if parsing fails.
extern "C" int sqlite3GetBoolean(const char *z, int bDflt){
    if(!z) return bDflt;
    std::string s(z);
    // Basic normalization: trim whitespace (leading/trailing)
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) start++;
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end-1]))) end--;
    std::string trimmed = s.substr(start, end - start);
    if(trimmed.empty()) return bDflt;

    std::string t = toLowerCopy(trimmed);

    if (t == "1"  || t == "true"  || t == "yes" || t == "on")  return 1;
    if (t == "0"  || t == "false" || t == "no"  || t == "off") return 0;

    // Unknown value: fall back to default
    return bDflt;
}

// Re-implementation of the focal method under test as declared in the header.
// This mirrors the logic exactly as in the provided snippet.
extern "C" int sqlite3_uri_boolean(const char *zFilename, const char *zParam, int bDflt){
    const char *z = sqlite3_uri_parameter(zFilename, zParam);
    bDflt = bDflt != 0; // normalize to boolean 0/1
    return z ? sqlite3GetBoolean(z, bDflt) : bDflt;
}

// Lightweight test framework (non-terminating assertions)
static int g_totalTests = 0;
static int g_failedTests = 0;

#define EXPECT_TRUE(cond, desc) do { \
    ++g_totalTests; \
    if(!(cond)){ \
        ++g_failedTests; \
        std::cerr << "TEST FAILED: " << (desc) << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    } \
} while(0)

#define EXPECT_EQ(expected, actual, desc) do { \
    ++g_totalTests; \
    if(!((expected) == (actual))){ \
        ++g_failedTests; \
        std::cerr << "TEST FAILED: " << (desc) \
                  << " | Expected: " << (expected) \
                  << " Actual: " << (actual) \
                  << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    } \
} while(0)

// Test 1: Parameter exists and is a true value
// Rationale: z != nullptr, sqlite3GetBoolean should interpret "true" as 1.
void test_uri_boolean_param_true(){
    // Parameter provided in URI; should parse and convert to boolean 1
    const char *zFilename = "db.sqlite?param=true";
    int result = sqlite3_uri_boolean(zFilename, "param", 0);
    EXPECT_EQ(1, result, "Parameter 'param=true' should yield boolean 1");
}

// Test 2: Parameter exists and is a false value
void test_uri_boolean_param_false(){
    const char *zFilename = "db.sqlite?param=false";
    int result = sqlite3_uri_boolean(zFilename, "param", 1);
    EXPECT_EQ(0, result, "Parameter 'param=false' should yield boolean 0");
}

// Test 3: Parameter exists with unparseable value should use default
void test_uri_boolean_param_unparseable_uses_default_true(){
    const char *zFilename = "db.sqlite?param=maybe";
    int result = sqlite3_uri_boolean(zFilename, "param", 1);
    EXPECT_EQ(1, result, "Unparseable param value should fall back to default (1)");
}
void test_uri_boolean_param_unparseable_uses_default_false(){
    const char *zFilename = "db.sqlite?param=maybe";
    int result = sqlite3_uri_boolean(zFilename, "param", 0);
    EXPECT_EQ(0, result, "Unparseable param value should fall back to default (0)");
}

// Test 4: Parameter does not exist; should return default boolean (based on bDflt)
void test_uri_boolean_param_missing_uses_default_true(){
    const char *zFilename = "db.sqlite"; // no '?param=' present
    int result = sqlite3_uri_boolean(zFilename, "param", 1);
    EXPECT_EQ(1, result, "Missing parameter should return default true");
}
void test_uri_boolean_param_missing_uses_default_false(){
    const char *zFilename = "db.sqlite"; // no '?param=' present
    int result = sqlite3_uri_boolean(zFilename, "param", 0);
    EXPECT_EQ(0, result, "Missing parameter should return default false");
}

// Test 5: Parameter exists with integer "1" and non-zero default
void test_uri_boolean_param_one_with_nonzero_default(){
    const char *zFilename = "db.sqlite?param=1";
    int result = sqlite3_uri_boolean(zFilename, "param", 5); // default treated as true
    EXPECT_EQ(1, result, "Parameter 'param=1' should yield boolean 1 regardless of non-zero default");
}

// Test 6: Parameter exists with "0" and non-zero default
void test_uri_boolean_param_zero_with_nonzero_default(){
    const char *zFilename = "db.sqlite?param=0";
    int result = sqlite3_uri_boolean(zFilename, "param", 5);
    EXPECT_EQ(0, result, "Parameter 'param=0' should yield boolean 0 even if default is non-zero");
}

// Runner to execute all tests
void runAllTests(){
    // Clarifying comments on test coverage mapping to Candidate Keywords:
    // - zFilename, zParam, bDflt: exercised across tests with present/absent parameter and various defaults.
    // - sqlite3_uri_parameter: indirectly exercised by the presence/absence of parameter.
    // - sqlite3GetBoolean: exercised with "true", "false", unparseable, and numeric inputs.
    test_uri_boolean_param_true();
    test_uri_boolean_param_false();
    test_uri_boolean_param_unparseable_uses_default_true();
    test_uri_boolean_param_unparseable_uses_default_false();
    test_uri_boolean_param_missing_uses_default_true();
    test_uri_boolean_param_missing_uses_default_false();
    test_uri_boolean_param_one_with_nonzero_default();
    test_uri_boolean_param_zero_with_nonzero_default();
}

// Main entry: run tests and report summary
int main(){
    runAllTests();

    std::cout << "Total tests: " << g_totalTests
              << ", Failures: " << g_failedTests << std::endl;

    return (g_failedTests > 0) ? 1 : 0;
}