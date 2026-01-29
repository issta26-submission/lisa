// Test suite for sqlite3Fts5ConfigParseRank
// This test suite is written for C++11 without using GTest.
// It exercises various branches of sqlite3Fts5ConfigParseRank as implemented in fts5_config.c.

#include <fts5Int.h>
#include <sqlite3.h>
#include <string>
#include <cstring>
#include <iostream>


// Include SQLite headers to use SQLITE_OK/SQLITE_ERROR and memory management helpers.
extern "C" {
}

// Declare the function under test (extern "C" to avoid name mangling)
extern "C" int sqlite3Fts5ConfigParseRank(const char *zIn, char **pzRank, char **pzRankArgs);

// Helper assertion infrastructure (non-terminating, to maximize coverage)
static int g_failures = 0;

#define EXPECT_EQ_INT(actual, expected, msg) do { \
    if ((actual) != (expected)) { \
        ++g_failures; \
        std::cout << "EXPECT_FAIL: " msg " | expected: " << (expected) << " got: " << (actual) << "\n"; \
    } \
} while(false)

#define EXPECT_NULL(ptr, msg) do { \
    if ((ptr) != nullptr) { \
        ++g_failures; \
        std::cout << "EXPECT_FAIL: " msg " | pointer should be NULL but is non-NULL\n"; \
    } \
} while(false)

#define EXPECT_STREQ_OR_NULL(a, b, msg) do { \
    if ((a) == nullptr && (b) == nullptr) { \
        /* both null, ok */ \
    } else if ((a) == nullptr || (b) == nullptr) { \
        ++g_failures; \
        std::cout << "EXPECT_FAIL: " msg " | one pointer is NULL, the other is not\n"; \
    } else if (std::strcmp((a), (b)) != 0) { \
        ++g_failures; \
        std::cout << "EXPECT_FAIL: " msg " | expected: \"" << (b) << "\" got: \"" << (a) << "\"\n"; \
    } \
} while(false)

static void test_parse_rank_basic_no_args() {
    // Test: "rankName()" -> rc OK, rank == "rankName", rankArgs == NULL
    const char *input = "rankName()";
    char *rank = nullptr;
    char *args = nullptr;

    int rc = sqlite3Fts5ConfigParseRank(input, &rank, &args);

    EXPECT_EQ_INT(rc, SQLITE_OK, "parse rank with no args should return SQLITE_OK");
    EXPECT_NOT_EQ(rank, nullptr, "rank should be non-null when rc==OK"); // can't use macro; manual check below
    if (rank != nullptr) {
        EXPECT_STREQ_OR_NULL(rank, "rankName", "rank name content mismatch");
    } else {
        std::cout << "EXPECTED: rank non-null but got null\n";
    }
    EXPECT_NULL(args, "args should be NULL when there are no arguments");

    // Cleanup
    if (rank) sqlite3_free(rank);
    if (args) sqlite3_free(args);
}

static void test_parse_rank_null_input() {
    // Test: NULL input should yield SQLITE_ERROR and both outputs NULL
    char *rank = nullptr;
    char *args = nullptr;

    int rc = sqlite3Fts5ConfigParseRank(nullptr, &rank, &args);

    EXPECT_EQ_INT(rc, SQLITE_ERROR, "NULL input should return SQLITE_ERROR");
    EXPECT_NULL(rank, "rank must be NULL on error");
    EXPECT_NULL(args, "args must be NULL on error");
}

static void test_parse_rank_with_whitespace() {
    // Test: Leading/trailing whitespace around rank name
    const char *input = "   rankName  ( )  ";
    char *rank = nullptr;
    char *args = nullptr;

    int rc = sqlite3Fts5ConfigParseRank(input, &rank, &args);

    EXPECT_EQ_INT(rc, SQLITE_OK, "parse rank with whitespace should succeed");
    if (rank) {
        EXPECT_STREQ_OR_NULL(rank, "rankName", "rank name content mismatch with whitespace");
    } else {
        std::cout << "EXPECTED: rank non-null but got null\n";
        ++g_failures;
    }
    EXPECT_NULL(args, "args should be NULL for empty argument list");

    if (rank) sqlite3_free(rank);
    if (args) sqlite3_free(args);
}

static void test_parse_rank_missing_bareword() {
    // Test: Input that starts with whitespace then '(' should error (no bareword)
    const char *input = "   ( )";
    char *rank = nullptr;
    char *args = nullptr;

    int rc = sqlite3Fts5ConfigParseRank(input, &rank, &args);

    EXPECT_EQ_INT(rc, SQLITE_ERROR, "missing bareword should produce error");
    EXPECT_NULL(rank, "rank should be NULL on error");
    EXPECT_NULL(args, "args should be NULL on error");
}

static void test_parse_rank_missing_open_paren() {
    // Test: rank name without an opening parenthesis should error
    const char *input = "rankName";
    char *rank = nullptr;
    char *args = nullptr;

    int rc = sqlite3Fts5ConfigParseRank(input, &rank, &args);

    EXPECT_EQ_INT(rc, SQLITE_ERROR, "missing '(' after rank should produce error");
    EXPECT_NULL(rank, "rank should be NULL on error");
    EXPECT_NULL(args, "args should be NULL on error");
}

static void test_parse_rank_with_args() {
    // Test: "rankName(abc)" -> rankName and args "abc"
    const char *input = "rankName(abc)";
    char *rank = nullptr;
    char *args = nullptr;

    int rc = sqlite3Fts5ConfigParseRank(input, &rank, &args);

    EXPECT_EQ_INT(rc, SQLITE_OK, "parse rank with one arg should succeed");
    if (rank) {
        EXPECT_STREQ_OR_NULL(rank, "rankName", "rank name content mismatch");
    } else {
        std::cout << "EXPECTED: rank non-null but got null\n";
        ++g_failures;
    }
    if (args) {
        EXPECT_STREQ_OR_NULL(args, "abc", "args content mismatch");
    } else {
        std::cout << "EXPECTED: args non-null but got null\n";
        ++g_failures;
    }

    if (rank) sqlite3_free(rank);
    if (args) sqlite3_free(args);
}

static void test_parse_rank_args_with_spaces() {
    // Test: "rankName( a, b )" should yield rank "rankName" and args "a, b"
    const char *input = "rankName( a, b )";
    char *rank = nullptr;
    char *args = nullptr;

    int rc = sqlite3Fts5ConfigParseRank(input, &rank, &args);

    EXPECT_EQ_INT(rc, SQLITE_OK, "parse rank args with spaces should succeed");
    if (rank) {
        EXPECT_STREQ_OR_NULL(rank, "rankName", "rank name content mismatch with spaces");
    } else {
        std::cout << "EXPECTED: rank non-null but got null\n";
        ++g_failures;
    }
    if (args) {
        EXPECT_STREQ_OR_NULL(args, "a, b", "args content mismatch with spaces");
    } else {
        std::cout << "EXPECTED: args non-null but got null\n";
        ++g_failures;
    }

    if (rank) sqlite3_free(rank);
    if (args) sqlite3_free(args);
}

int main() {
    // Run tests
    test_parse_rank_null_input();
    test_parse_rank_basic_no_args();
    test_parse_rank_with_whitespace();
    test_parse_rank_missing_bareword();
    test_parse_rank_missing_open_paren();
    test_parse_rank_with_args();
    test_parse_rank_args_with_spaces();

    // Summary
    int total = 7;
    std::cout << "Test suite completed. Failures: " << g_failures << " / " << total << "\n";
    return (g_failures == 0) ? 0 : 1;
}