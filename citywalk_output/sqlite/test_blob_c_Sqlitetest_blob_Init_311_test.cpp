/* File: tcl.h
   Lightweight mock of Tcl headers sufficient for compiling tests
   of the focal function in test_blob.c without requiring an external Tcl installation.
*/
#ifndef TCL_H
#define TCL_H

// Normalize the CONST keyword usage used in the source:
// In the original code, Tcl_Obj *CONST objv[] is used.
// Define CONST to expand to C++-friendly const.
#define CONST const

typedef void* ClientData;

// Forward declarations to satisfy the function signatures used in test_blob.c.
typedef struct Tcl_Interp Tcl_Interp;
typedef struct Tcl_Obj Tcl_Obj;

// Tcl_ObjCmdProc signature used by Tcl_CreateObjCommand
typedef int Tcl_ObjCmdProc(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);

// Optional delete proc signature (not used by our tests, but present for type compatibility)
typedef void Tcl_ObjCmdDeleteProc(ClientData clientData);

// TCL_OK is typically defined in Tcl headers; our tests rely on it being 0.
#define TCL_OK 0

#endif // TCL_H



/* File: test_blob_test.cpp
   Objective-C style C++ test harness for the focal function:
   int Sqlitetest_blob_Init(Tcl_Interp *interp)
   from test_blob.c

   Approach:
   - Provide a minimal mock of Tcl_CreateObjCommand to capture registrations performed
     by Sqlitetest_blob_Init.
   - Call Sqlitetest_blob_Init with a dummy interpreter and verify:
     • It returns TCL_OK (0).
     • It registers exactly the expected 5 blob-related commands in the specified order.
     • Each registered command has a non-null function pointer (xProc) assigned.
   - Exercise basic postconditions and demonstrate non-terminating checks to validate behavior
     without aborting on the first failure.
*/
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <assert.h>
#include <tcl.h>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Bring in the mock Tcl header

// Forward declaration of the focal function under test (from test_blob.c)
extern "C" int Sqlitetest_blob_Init(Tcl_Interp *interp);

// Global storage to capture the commands registered by Sqlitetest_blob_Init
static std::vector<std::string> g_registeredCmdNames;
static std::vector<void*>       g_registeredCmdProcs;

// Lightweight non-terminating test helpers (do not abort on failure)
static void log_failure(const char* file, int line, const char* msg){
    std::cerr << "[Test Failure] " << file << ":" << line << " - " << msg << std::endl;
}
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) log_failure(__FILE__, __LINE__, #cond); \
} while(0)

#define EXPECT_STR_EQ(actual, expected) do { \
    if(std::strcmp((actual), (expected)) != 0) { \
        log_failure(__FILE__, __LINE__, ("Expected " + std::string(expected) + " but got " + std::string(actual)).c_str()); \
    } \
} while(0)

#define EXPECT_EQ_INT(actual, expected) do { \
    if((actual) != (expected)) { \
        log_failure(__FILE__, __LINE__, ("Expected " #actual " == " #expected).c_str()); \
    } \
} while(0)

// Mock implementation of Tcl_CreateObjCommand to intercept registrations.
// This function must be visible to the link stage for test_blob.c's call site.
extern "C" void Tcl_CreateObjCommand(Tcl_Interp *interp,
                                   const char *zName,
                                   Tcl_ObjCmdProc *xProc,
                                   ClientData clientData,
                                   void (*deleteProc)(ClientData)){
    (void)interp;              // In tests, interp is not essential for the verification
    (void)clientData;
    (void)deleteProc;
    if(zName) {
        g_registeredCmdNames.emplace_back(zName);
        g_registeredCmdProcs.push_back(reinterpret_cast<void*>(xProc));
    } else {
        g_registeredCmdNames.emplace_back("(null)");
        g_registeredCmdProcs.push_back(nullptr);
    }
}

// Test runner
int main() {
    // Prepare a fresh state for each test scenario
    g_registeredCmdNames.clear();
    g_registeredCmdProcs.clear();

    // Create a dummy interpreter object (its internals are irrelevant for the test)
    Tcl_Interp dummyInterp;
    // 1) True path: call the focal method and verify normal registration flow
    int ret = Sqlitetest_blob_Init(&dummyInterp);

    // 1.a Result check: should return TCL_OK (0)
    EXPECT_TRUE(ret == TCL_OK);

    // 1.b Check that exactly 5 commands were registered
    EXPECT_EQ_INT((int)g_registeredCmdNames.size(), 5);

    // 1.c Verify the exact order and names of the registered commands
    static const char* expectedNames[5] = {
        "sqlite3_blob_open",
        "sqlite3_blob_close",
        "sqlite3_blob_bytes",
        "sqlite3_blob_read",
        "sqlite3_blob_write"
    };

    for(int i = 0; i < 5; ++i){
        EXPECT_STR_EQ(g_registeredCmdNames[i].c_str(), expectedNames[i]);
        // 1.d Ensure a non-null function pointer was passed for each command
        EXPECT_TRUE(g_registeredCmdProcs[i] != nullptr);
    }

    // 2) Additional basic behavior: calling again should attempt to register another set
    //    of the same commands (no deduplication logic in the focal function).
    //    This tests static local initialization behavior related to multiple invocations.
    int ret2 = Sqlitetest_blob_Init(&dummyInterp);

    EXPECT_TRUE(ret2 == TCL_OK);
    // After second invocation, there should be 10 registrations in total
    EXPECT_EQ_INT((int)g_registeredCmdNames.size(), 10);

    // 3) Sanity check: ensure names of the first and second batch are identical and in order
    for(int i = 0; i < 5; ++i){
        EXPECT_STR_EQ(g_registeredCmdNames[i].c_str(), expectedNames[i]);
        EXPECT_TRUE(g_registeredCmdProcs[i] != nullptr);
        // Second batch
        EXPECT_STR_EQ(g_registeredCmdNames[i+5].c_str(), expectedNames[i]);
        EXPECT_TRUE(g_registeredCmdProcs[i+5] != nullptr);
    }

    // If we reached here, basic coverage and sanity checks passed (non-fatal on failures).
    std::cout << "All non-fatal tests executed. Review any non-empty failure logs above." << std::endl;
    return 0;
}