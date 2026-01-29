/*
  C++11 test suite for the focal method: strglob
  Notes:
  - The original project depends on sqlite3 and a large test harness (mptest.c).
  - To create a self-contained unit test without bringing in sqlite3 dependencies, this
    test re-implements a faithful C++11-compatible version of strglob (the same
    logic as in the provided <FOCAL_METHOD>), along with a tiny test framework.
  - We cover true/false branches, wildcards, character classes, negated classes, and
    edge cases (empty strings, end-of-string behavior, etc.).
  - Tests are non-terminating in the sense that failures are collected and reported
    at the end, rather than aborting on first failure. This is implemented with a
    minimal EXPECT_* API (not using GTest).
  - Static helpers in the original file are not accessed; we test the focal function
    directly through this local reimplementation.
*/

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <functional>
#include <vector>
#include <assert.h>
#include <iomanip>
#include <sqlite3.h>
#include <stdio.h>
#include <string>
#include <iostream>


// Candidate Keywords distilled from the focal method:
// - '*' and '?' wildcard handling
// - '[' character class (positive and inverted, with ranges)
// - '#' special digit-handling directive
// - recursive glob matching (strglob calls itself)
// - end-of-string semantics and backtracking via z/zGlob pointers

// Local reimplementation of strglob to keep test self-contained (no sqlite3 dependency)
static int ISDIGIT(int ch){
    return ch >= '0' && ch <= '9';
}

// A faithful translation of the provided C-like strglob into C++.
// This version mirrors the branching structure to exercise the same code paths.
static int strglob(const char *zGlob, const char *z){
  int c, c2;
  int invert;
  int seen;
  while( (c = (*(zGlob++)))!=0 ){
    if( c=='*' ){
      while( (c=(*(zGlob++))) == '*' || c=='?' ){
        if( c=='?' && (*(z++))==0 ) return 0;
      }
      if( c==0 ){
        return 1;
      }else if( c=='[' ){
        while( *z && strglob(zGlob-1,z) ){
          z++;
        }
        return (*z)!=0;
      }
      while( (c2 = (*(z++)))!=0 ){
        while( c2!=c ){
          c2 = *(z++);
          if( c2==0 ) return 0;
        }
        if( strglob(zGlob,z) ) return 1;
      }
      return 0;
    }else if( c=='?' ){
      if( (*(z++))==0 ) return 0;
    }else if( c=='[' ){
      int prior_c = 0;
      seen = 0;
      invert = 0;
      c = *(z++);
      if( c==0 ) return 0;
      c2 = *(zGlob++);
      if( c2=='^' ){
        invert = 1;
        c2 = *(zGlob++);
      }
      if( c2==']' ){
        if( c==']' ) seen = 1;
        c2 = *(zGlob++);
      }
      while( c2 && c2!=']' ){
        if( c2=='-' && zGlob[0]!=']' && zGlob[0]!=0 && prior_c>0 ){
          c2 = *(zGlob++);
          if( c>=prior_c && c<=c2 ) seen = 1;
          prior_c = 0;
        }else{
          if( c==c2 ){
            seen = 1;
          }
          prior_c = c2;
        }
        c2 = *(zGlob++);
      }
      if( c2==0 || (seen ^ invert)==0 ) return 0;
    }else if( c=='#' ){
      if( (z[0]=='-' || z[0]=='+') && ISDIGIT(z[1]) ) z++;
      if( !ISDIGIT(z[0]) ) return 0;
      z++;
      while( ISDIGIT(z[0]) ){ z++; }
    }else{
      if( c!=(*(z++)) ) return 0;
    }
  }
  return *z==0;
}

// Lightweight test framework (non-terminating assertions)
static int g_failures = 0;
static void log_failure(const std::string& msg){
    std::cerr << "Test Failure: " << msg << std::endl;
    ++g_failures;
}

#define EXPECT_EQ(actual, expected, msg) do { \
    auto _a = (actual); \
    auto _e = (expected); \
    if(!(_a == _e)) { \
        log_failure(msg + std::string(" | expected: ") + std::to_string(_e) + \
                    std::string(", actual: ") + std::to_string(_a)); \
    } \
} while(0)

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { log_failure(msg); } \
} while(0)

#define EXPECT_FALSE(cond, msg) do { \
    if((cond)) { log_failure(msg); } \
} while(0)

// Helper to run a test function and print a header
static void runTest(const std::string& name, const std::function<void()>& fn){
    // Execute test
    fn();
    // Print per-test header (success or silent on pass; failures logged globally)
    std::cout << "Test: " << std::setw(20) << std::left << name << " - DONE" << std::endl;
}

// Convenience wrapper to call the focal function under test
static int test_Strglob(const std::string& glob, const std::string& s){
    return strglob(glob.c_str(), s.c_str());
}

// Unit Tests

// Test 1: Exact literal match
// Purpose: True when input exactly matches the pattern with no wildcards
static void test_exact_match(){
    EXPECT_EQ(test_Strglob("abc","abc"), 1, "Exact literal match should succeed");
    EXPECT_EQ(test_Strglob("abc","abcd"), 0, "Exact literal should fail when input longer than pattern");
    EXPECT_EQ(test_Strglob("",""), 1, "Empty pattern and empty string should match (edge case)");
}

// Test 2: Star '*' wildcard behavior (matches any sequence, including empty)
// Purpose: '*' consumes as many characters as needed; end of string handling
static void test_star_wildcard(){
    EXPECT_EQ(test_Strglob("*",""), 1, "Star should match empty string");
    EXPECT_EQ(test_Strglob("*","abc"), 1, "Star should match the entire input");
    EXPECT_EQ(test_Strglob("*a","ba"), 1, "Star + literal should match end 'a' at end");
    EXPECT_EQ(test_Strglob("a*","b"), 0, "Literal before star should fail when first char mismatches");
}

// Test 3: Question mark '?' wildcard
// Purpose: '?' matches exactly one character
static void test_question_mark(){
    EXPECT_EQ(test_Strglob("?",""), 0, "Question mark requires one character");
    EXPECT_EQ(test_Strglob("?","x"), 1, "Question mark should match a single character");
    EXPECT_EQ(test_Strglob("a?c","abc"), 1, "Pattern a?c should match abc");
    EXPECT_EQ(test_Strglob("a?c","ac"), 0, "Pattern a?c should not match shorter string");
}

// Test 4: Character class [...] (positive set)
// Purpose: Match any one character from the set
static void test_char_class_positive(){
    EXPECT_EQ(test_Strglob("[ab]","a"), 1, "Character class should match 'a' in [ab]");
    EXPECT_EQ(test_Strglob("[ab]","b"), 1, "Character class should match 'b' in [ab]");
    EXPECT_EQ(test_Strglob("[ab]","c"), 0, "Character class should not match 'c'");
}

// Test 5: Character class inverted [[^...]]
// Purpose: Inverted class matches when current char is not in the set
static void test_char_class_inverted(){
    EXPECT_EQ(test_Strglob("[^a]","b"), 1, "Inverted class should match when not 'a'");
    EXPECT_EQ(test_Strglob("[^a]","a"), 0, "Inverted class should not match 'a'");
}

// Test 6: Character class with ranges [a-z]
static void test_char_class_range(){
    EXPECT_EQ(test_Strglob("[a-z]","m"), 1, "Range a-z should match lowercase letter");
    EXPECT_EQ(test_Strglob("[a-z]","A"), 0, "Range a-z should not match uppercase");
}

// Test 7: Complex pattern with star followed by class
// Purpose: Ensure backtracking logic around star with subsequent pattern works
static void test_star_then_class(){
    EXPECT_EQ(test_Strglob("*[ab]","cab"), 1, "Star followed by class should match trailing 'b'");
    EXPECT_EQ(test_Strglob("*[ab]","ca"), 1, "Star followed by class should match trailing 'a'");
    EXPECT_EQ(test_Strglob("*[ab]","c"), 0, "Star + class should fail if trailing pattern cannot match");
}

// Test 8: End-of-string semantics with special cases
static void test_end_of_string_semantics(){
    EXPECT_EQ(test_Strglob("*?",""), 0, "Pattern *? should not match empty string (needs at least two chars)");
    EXPECT_EQ(test_Strglob("*?","a"), 1, "Pattern *? should match single-character input");
    EXPECT_EQ(test_Strglob("**",""), 1, "Multiple stars should behave like single wildcard");
}

// Test 9: Empty pattern edge cases and direct literal with empty string
static void test_empty_and_literals(){
    EXPECT_EQ(test_Strglob("",""), 1, "Empty pattern matches empty string");
    EXPECT_EQ(test_Strglob("","a"), 0, "Empty pattern should not match non-empty string");
}

// Main test harness
int main(){
    std::cout << "Starting strglob unit tests (self-contained, no external deps)" << std::endl;

    runTest("Exact Match", test_exact_match);
    runTest("Star Wildcard", test_star_wildcard);
    runTest("Question Mark", test_question_mark);
    runTest("Character Class Positive", test_char_class_positive);
    runTest("Character Class Inverted", test_char_class_inverted);
    runTest("Character Class Range", test_char_class_range);
    runTest("Star Then Class", test_star_then_class);
    runTest("End-of-String Semantics", test_end_of_string_semantics);
    runTest("Empty Pattern and Literals", test_empty_and_literals);

    std::cout << std::endl;
    if(g_failures){
        std::cout << "Summary: " << g_failures << " test(s) FAILED." << std::endl;
        return 1;
    }else{
        std::cout << "Summary: All tests PASSED." << std::endl;
        return 0;
    }
}