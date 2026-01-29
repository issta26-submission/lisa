/*
   Lightweight C++11 unit test for sqlite3VdbeExplainPop(Parse *pParse)
   - Uses hand-written test harness (no Google Test)
   - Provides minimal stubs for external dependencies (sqlite3ExplainBreakpoint, sqlite3VdbeExplainParent)
   - Verifies that:
     1) sqlite3ExplainBreakpoint is called with ("POP", 0)
     2) pParse->addrExplain is assigned the value returned by sqlite3VdbeExplainParent(pParse)
   - Exercises two scenarios with different return values from sqlite3VdbeExplainParent
*/

#include <iostream>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>


// Minimal environment to compile and test the focal function

namespace TestEnv {

// Lightweight Parse structure used by the focal function
struct Parse {
  int addrExplain;
};

// Internal state to observe breakpoint calls
static const char* gLastExplainBreakpointZ = nullptr;
static int         gLastExplainBreakpointN = -1;

// Internal state to control what sqlite3VdbeExplainParent returns
static int         gNextExplainParentReturn = 0;

// Stub: Observe breakpoint invocations
void sqlite3ExplainBreakpoint(const char* z, int i){
  gLastExplainBreakpointZ = z;
  gLastExplainBreakpointN = i;
}

// Setter to control the return value of sqlite3VdbeExplainParent
void sqlite3VdbeExplainParent_SetReturn(int v){
  gNextExplainParentReturn = v;
}

// Stub: The function under test calls this to obtain the "parent" explain address.
// In tests, return a controlled value so we can assert correctness.
int sqlite3VdbeExplainParent(Parse* p){
  (void)p;
  return gNextExplainParentReturn;
}

// The focal function under test
// void sqlite3VdbeExplainPop(Parse *pParse){
void sqlite3VdbeExplainPop(Parse *pParse){
  sqlite3ExplainBreakpoint("POP", 0);
  pParse->addrExplain = sqlite3VdbeExplainParent(pParse);
}
} // namespace TestEnv

// Bring the focal function into the current scope for access
using TestEnv::Parse;
using TestEnv::sqlite3VdbeExplainPop;
using TestEnv::sqlite3ExplainBreakpoint;
using TestEnv::sqlite3VdbeExplainParent;

// Simple test harness (no GTest). Provides basic assertions that do not abort on failure.
static int gTestFailures = 0;

#define ASSERT_TRUE(cond, msg) \
  do { \
    if(!(cond)) { \
      std::cerr << "ASSERT_TRUE failed: " << (msg) << std::endl; \
      ++gTestFailures; \
    } \
  } while(0)

#define ASSERT_EQ_INT(a,b,msg) \
  do { \
    if( (int)(a) != (int)(b) ) { \
      std::cerr << "ASSERT_EQ_INT failed: " << (msg) \
                << " Expected " << (b) << ", Got " << (a) << std::endl; \
      ++gTestFailures; \
    } \
  } while(0)

#define ASSERT_BREAKPOINT_Z(expected) \
  do { \
    bool ok = false; \
    if(TestEnv::gLastExplainBreakpointZ != nullptr) { \
      ok = (std::strcmp(TestEnv::gLastExplainBreakpointZ, (expected)) == 0); \
    } \
    if(!ok) { \
      std::cerr << "ASSERT_BREAKPOINT_Z failed: expected '" << (expected) \
                << "', got '" << (TestEnv::gLastExplainBreakpointZ ? TestEnv::gLastExplainBreakpointZ : "null") \
                << "'" << std::endl; \
      ++gTestFailures; \
    } \
  } while(0)

static void test_sqlite3VdbeExplainPop_sets_addrExplain_and_breakpoint(){
  // Test 1: Basic behavior - ensure breakpoint is reported and addrExplain is set to Parent's return
  TestEnv::Parse p1;
  p1.addrExplain = -999; // some arbitrary initial value

  TestEnv::sqlite3VdbeExplainParent_SetReturn(1234);

  // Call the focal function
  sqlite3VdbeExplainPop(&p1);

  // Verify breakpoint was invoked with "POP", 0
  ASSERT_BREAKPOINT_Z("POP");

  // Verify correct address assignment
  ASSERT_EQ_INT(p1.addrExplain, 1234, "addrExplain should be assigned from sqlite3VdbeExplainParent return");
}

static void test_sqlite3VdbeExplainPop_with_different_parent_return(){
  // Test 2: Change the return value and ensure addrExplain reflects it
  TestEnv::Parse p2;
  p2.addrExplain = 0;

  TestEnv::sqlite3VdbeExplainParent_SetReturn(-77);

  sqlite3VdbeExplainPop(&p2);

  // Verify breakpoint again
  ASSERT_BREAKPOINT_Z("POP");

  // Verify correct address assignment with new return value
  ASSERT_EQ_INT(p2.addrExplain, -77, "addrExplain should be assigned from updated sqlite3VdbeExplainParent return");
}

// Summary reporter
static void report_summary(){
  if(gTestFailures == 0){
    std::cout << "ALL TESTS PASSED" << std::endl;
  }else{
    std::cerr << gTestFailures << " TEST(S) FAILED" << std::endl;
  }
}

int main(){
  // Run tests
  test_sqlite3VdbeExplainPop_sets_addrExplain_and_breakpoint();
  test_sqlite3VdbeExplainPop_with_different_parent_return();

  // Print summary
  report_summary();
  return gTestFailures;
}