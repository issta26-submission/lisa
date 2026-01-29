// Minimal, self-contained C++ test harness for sqlite3BeginWriteOperation
// Focus: validate interactions with Parse toplevel state and code path
// Assumptions:
// - The environment provides sqliteInt.h with the real Parse structure.
// - The build.c file exists and defines sqlite3BeginWriteOperation along with
//   the external hooks it calls (sqlite3ParseToplevel, sqlite3CodeVerifySchemaAtToplevel, DbMaskSet macro).
// - We override sqlite3ParseToplevel and sqlite3CodeVerifySchemaAtTopLevel with C-linkage
//   stubs to observe internal state without pulling in the full SQLite runtime.

// Import necessary dependencies
#include <iostream>
#include <sqliteInt.h>
#include <cstdint>


// Observables from our test stubs
static Parse g_sharedTopLevel;          // The top-level Parse object used by sqlite3BeginWriteOperation
static bool  g_initialized = false;     // Tracks first/subsequent initializations inside our stub
static int   g_lastVerifyIdb = -1;       // Captures iDb passed to sqlite3CodeVerifySchemaAtToplevel
static Parse *g_lastVerifyPtop = nullptr; // Captures pTop passed to sqlite3CodeVerifySchemaAtToplevel

// We declare the API surface that sqlite3BeginWriteOperation will call.
// We provide C-linkage stubs so the linker can resolve them from build.c.
extern "C" {

  // This function is called by sqlite3BeginWriteOperation to obtain the top-level Parse*
  // We simulate a persistent top-level object to reflect real behavior across calls.
  Parse* sqlite3ParseToplevel(Parse *pParse) {
    // Initialize the shared top-level once, mirroring a real-to-top behavior:
    if (!g_initialized) {
      g_sharedTopLevel.writeMask = 0;
      g_sharedTopLevel.isMultiWrite = 0;
      g_initialized = true;
    }
    // Return a pointer to the shared top-level Parse object
    return &g_sharedTopLevel;
  }

  // This function records the iDb argument and the toplevel pointer used by the code path
  void sqlite3CodeVerifySchemaAtToplevel(Parse *pToplevel, int iDb) {
    g_lastVerifyIdb = iDb;
    g_lastVerifyPtop = pToplevel;
  }

  // The real code will call DbMaskSet(pToplevel->writeMask, iDb);
  // It is a macro in SQLite; we rely on that macro behavior at compile time
  // and do not provide an override here.
  void sqlite3BeginWriteOperation(Parse *pParse, int setStatement, int iDb);
}

// Helper for reporting test results without terminating on failures
static int g_passCount = 0;
static int g_failCount = 0;

#define ASSERT_EQ(expected, actual, msg)                                  \
  do {                                                                     \
    if (static_cast<uint64_t>(actual) != static_cast<uint64_t>(expected)) { \
      std::cerr << "FAIL: " << msg << " | expected: " << (expected)           \
                << ", actual: " << (actual) << "\n";                   \
      ++g_failCount;                                                     \
    } else {                                                               \
      ++g_passCount;                                                      \
    }                                                                      \
  } while (0)

static void runTests() {
  // We'll execute a sequence of operations that exercise true/false branches
  // and ensure state persists across successive sqlite3BeginWriteOperation calls.

  // Create a dummy Parse input; sqlite3BeginWriteOperation uses only the pParse argument
  // to call sqlite3ParseToplevel, which we overrode to return g_sharedTopLevel.
  Parse dummyParse;

  // Test 1: First call with setStatement = 1, iDb = 3
  // Expected: writeMask has bit 3 set; isMultiWrite becomes 1; verification callback invoked with iDb=3
  sqlite3BeginWriteOperation(&dummyParse, 1, 3);

  // Validate after first call
  ASSERT_EQ( (1u << 3), static_cast<unsigned>(g_sharedTopLevel.writeMask),
             "Test1: writeMask should have bit 3 set");
  ASSERT_EQ( 1u, static_cast<unsigned>(g_sharedTopLevel.isMultiWrite),
             "Test1: isMultiWrite should be 1 after first call");
  ASSERT_EQ( 3, static_cast<int>(g_lastVerifyIdb),
             "Test1: sqlite3CodeVerifySchemaAtToplevel should be called with iDb=3");
  ASSERT_EQ( (Parse*)&g_sharedTopLevel, g_lastVerifyPtop,
             "Test1: sqlite3CodeVerifySchemaAtToplevel should be called with correct pTop");

  // Test 2: Second call with setStatement = 0, iDb = 4
  // Expected: writeMask adds bit 4; isMultiWrite remains 1; lastVerifyIdb updated to 4
  sqlite3BeginWriteOperation(&dummyParse, 0, 4);

  // Validate after second call
  // writeMask now should be 0b1000 | 0b00010000 = 0b1000_0000? Wait: it's 8 | 16 = 24
  ASSERT_EQ( (static_cast<unsigned>(1u << 3) | static_cast<unsigned>(1u << 4)),
             static_cast<unsigned>(g_sharedTopLevel.writeMask),
             "Test2: writeMask should accumulate bits 3 and 4 (24)");
  ASSERT_EQ( 1u, static_cast<unsigned>(g_sharedTopLevel.isMultiWrite),
             "Test2: isMultiWrite should remain 1");
  ASSERT_EQ( 4, static_cast<int>(g_lastVerifyIdb),
             "Test2: sqlite3CodeVerifySchemaAtToplevel should be called with iDb=4");
  ASSERT_EQ( (Parse*)&g_sharedTopLevel, g_lastVerifyPtop,
             "Test2: sqlite3CodeVerifySchemaAtToplevel should be called with correct pTop");

  // Test 3: Third call with setStatement = 1, iDb = 1
  // Expected: add bit 1; accumulate: writeMask becomes 24 | 2 = 26; isMultiWrite becomes 1 | 1 = 1
  sqlite3BeginWriteOperation(&dummyParse, 1, 1);

  // Validate after third call
  ASSERT_EQ( static_cast<unsigned>(24u | (1u << 1)),
             static_cast<unsigned>(g_sharedTopLevel.writeMask),
             "Test3: writeMask should accumulate with bit 1 (26)");
  ASSERT_EQ( 1u, static_cast<unsigned>(g_sharedTopLevel.isMultiWrite),
             "Test3: isMultiWrite should remain 1 after OR with 1");
  ASSERT_EQ( 1, static_cast<int>(g_lastVerifyIdb),
             "Test3: sqlite3CodeVerifySchemaAtToplevel should be called with iDb=1");
  ASSERT_EQ( (Parse*)&g_sharedTopLevel, g_lastVerifyPtop,
             "Test3: sqlite3CodeVerifySchemaAtToplevel should be called with correct pTop");

  // Test 4: Fourth call with setStatement = 2, iDb = 2
  // Expected: add bit 2; final state: writeMask 26 | 4 = 30; isMultiWrite = 1 | 2 = 3
  sqlite3BeginWriteOperation(&dummyParse, 2, 2);

  // Validate after fourth call
  ASSERT_EQ( static_cast<unsigned>(26u | (1u << 2)),
             static_cast<unsigned>(g_sharedTopLevel.writeMask),
             "Test4: writeMask should accumulate bit 2 (30)");
  ASSERT_EQ( 3u, static_cast<unsigned>(g_sharedTopLevel.isMultiWrite),
             "Test4: isMultiWrite should be 3 after OR with 2");
  ASSERT_EQ( 2, static_cast<int>(g_lastVerifyIdb),
             "Test4: sqlite3CodeVerifySchemaAtToplevel should be called with iDb=2");
  ASSERT_EQ( (Parse*)&g_sharedTopLevel, g_lastVerifyPtop,
             "Test4: sqlite3CodeVerifySchemaAtToplevel should be called with correct pTop");
}

int main() {
  runTests();

  int total = g_passCount + g_failCount;
  std::cout << "Tests run: " << total << "\n";
  std::cout << "Passed:    " << g_passCount << "\n";
  std::cout << "Failed:    " << g_failCount << "\n";

  return g_failCount == 0 ? 0 : 1;
}

// Provide a simple wrapper to call the function under test.
// We declare the prototype here so the compiler can resolve it in this translation unit.
extern "C" void sqlite3BeginWriteOperation(Parse *pParse, int setStatement, int iDb);