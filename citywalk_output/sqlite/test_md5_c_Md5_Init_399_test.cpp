// Test suite for Md5_Init in the provided focal code.
// This test avoids GTest and uses a lightweight, non-terminating assertion approach.
// The test uses a small fake Tcl-like environment to intercept calls to Tcl_CreateCommand
// and verify that Md5_Init registers the expected commands with correct properties.

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <sqlite3.h>
#include <string>
#include <iostream>


// ----------------------------------------------------------------------------
// Minimal TCL-like scaffolding to support Md5_Init without external dependencies.
// ----------------------------------------------------------------------------

// TCL_OK equivalent
#define TCL_OK 0

// Forward declare a minimal Tcl_Interp type (opaque for the test)
struct Tcl_Interp;

// Typedef matching the expected Tcl command procedure signature
typedef int (*Tcl_CmdProc)(void* clientData, Tcl_Interp* interp, int argc, const char **argv);

// We need a C linkage for the Tcl_CreateCommand to avoid C++ name mangling
extern "C" {

// A record to capture each registered command invocation details
struct CommandCall {
    std::string name;    // Command name registered
    void* proc;            // Pointer to the command procedure (md5_cmd or md5file_cmd)
    void* clientData;      // Client data passed (MD5DigestToBase16 or MD5DigestToBase10x8)
};

// Global (translation-unit scope) collection to inspect registrations after Md5_Init runs
static std::vector<CommandCall> g_calls;

// Stub implementations of the two command entrypoints used by Md5_Init.
// They must exist so that Md5_Init can take their addresses as Tcl_CmdProc*.
static int md5_cmd(void* cd, Tcl_Interp* interp, int argc, const char **argv) {
    return 0;
}
static int md5file_cmd(void* cd, Tcl_Interp* interp, int argc, const char **argv) {
    return 0;
}

// The two static utility callbacks (passed as clientData to Tcl_CreateCommand)
void MD5DigestToBase16(unsigned char* digest, char* zBuf) { /* test stub */ }
void MD5DigestToBase10x8(unsigned char digest[16], char zDigest[50]) { /* test stub */ }

// The actual Md5_Init function under test (copied to this test file for self-containment).
int Md5_Init(Tcl_Interp *interp){
  Tcl_CreateCommand(interp, "md5", (Tcl_CmdProc*)md5_cmd,
                    MD5DigestToBase16, 0);
  Tcl_CreateCommand(interp, "md5-10x8", (Tcl_CmdProc*)md5_cmd,
                    MD5DigestToBase10x8, 0);
  Tcl_CreateCommand(interp, "md5file", (Tcl_CmdProc*)md5file_cmd,
                    MD5DigestToBase16, 0);
  Tcl_CreateCommand(interp, "md5file-10x8", (Tcl_CmdProc*)md5file_cmd,
                    MD5DigestToBase10x8, 0);
  return TCL_OK;
}

// Stubbed Tcl_CreateCommand to capture registration details instead of performing real Tcl work.
int Tcl_CreateCommand(Tcl_Interp* interp, const char* cmdName, Tcl_CmdProc* cmdProc,
                      void* clientData, void* deleteProc){
    CommandCall c;
    c.name = cmdName ? cmdName : "";
    c.proc = (void*)cmdProc;
    c.clientData = clientData;
    g_calls.push_back(c);
    return 0;
}

// Minimal, opaque Tcl_Interp definition (implementation not required for test)
struct Tcl_Interp {
    // Intentionally empty; test only relies on its pointer identity.
};

// A tiny, lightweight assertion framework (non-terminating, collects failures)
struct TestLogger {
    int failures = 0;
    std::vector<std::string> messages;

    void logFailure(const std::string& msg) {
        ++failures;
        messages.push_back(msg);
    }

    void report(const std::string& testName) const {
        if (failures == 0) {
            std::cout << "[PASS] " << testName << "\n";
        } else {
            std::cout << "[FAIL] " << testName << " - " << failures << " issue(s) detected:\n";
            for (const auto& m : messages) {
                std::cout << "  - " << m << "\n";
            }
        }
    }
};

// ----------------------------------------------------------------------------
// Unit Test 1: Verify Md5_Init registers four commands with expected names and
//            that the command procedure pointers are consistent across identical commands
//            and that clientData pointers group by their intended base (16 vs 10x8).
// ----------------------------------------------------------------------------
bool test_Md5_Init_RegistersCommands() {
    TestLogger logger;
    g_calls.clear();

    // Prepare a fake interpreter
    Tcl_Interp* interp = new Tcl_Interp();

    // Execute the function under test
    int ret = Md5_Init(interp);

    // Cleanup
    delete interp;

    // Check return value
    if (ret != TCL_OK) {
        logger.logFailure("Md5_Init did not return TCL_OK (TCL_OK expected).");
        logger.report("Md5_Init_RegisterCommands");
        return false;
    }

    // Expect exactly four registrations
    if (g_calls.size() != 4) {
        logger.logFailure("Md5_Init registered an unexpected number of commands. Expected 4.");
        logger.report("Md5_Init_RegisterCommands");
        return false;
    }

    // Expected command names in order
    const std::string expectedNames[4] = {"md5", "md5-10x8", "md5file", "md5file-10x8"};
    for (int i = 0; i < 4; ++i) {
        if (g_calls[i].name != expectedNames[i]) {
            logger.logFailure("Command name at position " + std::to_string(i) +
                              " expected '" + expectedNames[i] + "', got '" + g_calls[i].name + "'.");
        }
    }

    // Procs: 0th and 1st should be the same (md5_cmd), 2nd and 3rd the same (md5file_cmd)
    if (g_calls[0].proc != g_calls[1].proc) {
        logger.logFailure("md5 and md5-10x8 should share the same command proc (md5_cmd).");
    }
    if (g_calls[2].proc != g_calls[3].proc) {
        logger.logFailure("md5file and md5file-10x8 should share the same command proc (md5file_cmd).");
    }
    if (g_calls[0].proc == g_calls[2].proc) {
        logger.logFailure("md5_cmd and md5file_cmd should be distinct function pointers.");
    }

    // ClientData: 0th and 2nd should be the same pointer (MD5DigestToBase16),
    // 1st and 3rd should be the same pointer (MD5DigestToBase10x8)
    if (g_calls[0].clientData == nullptr || g_calls[2].clientData == nullptr) {
        logger.logFailure("MD5DigestToBase16 clientData should be non-null for md5/md5file commands.");
    }
    if (g_calls[1].clientData == nullptr || g_calls[3].clientData == nullptr) {
        logger.logFailure("MD5DigestToBase10x8 clientData should be non-null for md5-10x8/md5file-10x8 commands.");
    }
    if (g_calls[0].clientData != g_calls[2].clientData) {
        logger.logFailure("md5 and md5file should share the same MD5DigestToBase16 clientData pointer.");
    }
    if (g_calls[1].clientData != g_calls[3].clientData) {
        logger.logFailure("md5-10x8 and md5file-10x8 should share the same MD5DigestToBase10x8 clientData pointer.");
    }

    logger.report("Md5_Init_RegisterCommands");
    return (logger.failures == 0);
}

// ----------------------------------------------------------------------------
// Main: Run tests
// ----------------------------------------------------------------------------
int main() {
    bool all_passed = true;

    all_passed &= test_Md5_Init_RegistersCommands();

    // Additional tests could be added here following the same pattern.

    if (all_passed) {
        std::cout << "[OVERALL PASS] All tests passed.\n";
        return 0;
    } else {
        std::cout << "[OVERALL FAIL] Some tests failed. See details above.\n";
        return 1;
    }
}