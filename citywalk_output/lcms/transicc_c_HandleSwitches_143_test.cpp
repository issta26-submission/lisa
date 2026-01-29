// Test suite for the HandleSwitches function (transicc.c) using a self-contained mock
// environment. This test is written for C++11 compilation, without gtest, and uses a
// lightweight in-file test harness to verify behavior via global state changes.
// The test re-implements minimal dependencies (xgetopt, FatalError, etc.) to
// exercise branching in HandleSwitches while avoiding process termination.

#include <cstring>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstdarg>
#include <utils.h>


// Domain-specific types and macros (mocked to be compatible with the focal code)
typedef int cmsBool;
typedef double cmsFloat64Number;

#define TRUE 1
#define FALSE 0

// Global state (mirroring the dependencies used by HandleSwitches)
char* IncludePart = NULL;
cmsBool BlackPointCompensation = FALSE;
int PrecalcMode = 0;
cmsFloat64Number AdaptationStateSet = 0.0; // will be set via cmsSetAdaptationState
cmsBool lIsFloat = FALSE;
cmsBool GamutCheck = FALSE;

char* cInProf = NULL;
cmsBool lIsDeviceLink = FALSE;
char* cOutProf = NULL;
char* cProofing = NULL;
int ProofingIntent = 0;
int Verbose = 0;
cmsBool Width16 = FALSE;
cmsBool InHexa = FALSE;

int xoptind = 1;
char* xoptarg = NULL;

// Minimal implementations to satisfy the focal function's dependencies
void Help(void) {
    // No-op for tests
}

void FatalError(const char* fmt, ...) {
    // Non-terminating stub for testing: print to cerr to aid debugging but do not exit
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    // In a real run this would terminate; for tests we simply note the error behavior.
}

// Stub for cmsSetAdaptationState used by the HandleSwitches function
void cmsSetAdaptationState(cmsFloat64Number val) {
    AdaptationStateSet = val;
}

// Forward declaration of the function under test (the exact code is re-embedded below)
void HandleSwitches(int argc, char *argv[]);

// ----------------------------------------------------------------------------------
// A compact, self-contained xgetopt-like helper to feed command-line options to
// HandleSwitches. This is a minimal stand-in for the project's xgetopt, only
// supporting the subset of features used by our tests.
// It expects options of the form:
//   -single-letter (option may take an argument if in the optstring with a ':')
//   - '!' option requires an argument (as in the original code's optstring)
// It does not aim to be a full getopt replacement; it is sufficient for test scenarios.
static struct {
    int argc;
    char** argv;
    const char* optstring;
    int posArgIndex;     // which argv element we are inspecting (starting at 1)
    int posInOptString;    // position in the current argv[argc] option (after '-')
    bool initialized;
} g_xgetopt = {0, nullptr, nullptr, 1, 1, false};

void ResetXGetopt(int argc, char* argv[]) {
    g_xgetopt.argc = argc;
    g_xgetopt.argv = argv;
    g_xgetopt.optstring = "bBC:c:d:D:eEgGI:i:L:l:m:M:nNO:o:p:P:QqSsT:t:V:v:WwxX!:-:";
    g_xgetopt.posArgIndex = 1;
    g_xgetopt.posInOptString = 1;
    g_xgetopt.initialized = true;
    xoptind = 1;
    xoptarg = NULL;
}

// Very small helper to check if a character is present as an option in the optstring
static bool optionRequiresArg(char opt) {
    const char* s = g_xgetopt.optstring;
    while (*s) {
        if (*s == opt) {
            // If the next character is ':', this option requires an argument
            if (*(s + 1) == ':') return true;
            // Special case: '!' appears in the optstring with its own semantics (requires arg)
            // We handle this by returning true if the option '!' is encountered
            if (*(s + 1) == '!') return true;
            return false;
        }
        ++s;
    }
    return false;
}

// A simplified xgetopt sufficient for our tests
int xgetopt(int argc, char *argv[], const char* optstring) {
    (void)optstring; // not used directly, we rely on g_xgetopt.optstring
    if (!g_xgetopt.initialized) {
        ResetXGetopt(argc, argv);
    }

    // End of options
    if (g_xgetopt.posArgIndex >= argc) {
        xoptind = argc;
        xoptarg = NULL;
        return EOF;
    }

    char* arg = argv[g_xgetopt.posArgIndex];

    // If not an option or it's "--", stop processing options
    if (arg == NULL || arg[0] != '-' || (arg[0] == '-' && arg[1] == '-' && arg[2] == '\0')) {
        xoptind = g_xgetopt.posArgIndex;
        xoptarg = NULL;
        return EOF;
    }

    // Special case: single '-' means a request for help string in original code
    if (strcmp(arg, "-") == 0) {
        // Return '-' as option; xoptarg should be the next argument if available
        if ((g_xgetopt.posArgIndex + 1) < argc) {
            xoptarg = argv[g_xgetopt.posArgIndex + 1];
            g_xgetopt.posArgIndex += 2;
        } else {
            xoptarg = NULL;
            g_xgetopt.posArgIndex += 1;
        }
        return '-';
    }

    // Typical form: "-x"
    if (arg[0] == '-') {
        // If there are characters after '-', take the first as option
        char optChar = arg[1];
        bool requiresArg = optionRequiresArg(optChar);

        // Move to next token for further parsing in future calls
        if (strlen(arg) > 2) {
            // Treat "-cx" like "-c x" is not supported; instead provide the rest as arg for simplicity
            // We'll consider the rest as the argument for the option
            if (requiresArg) {
                xoptarg = &arg[2];
                g_xgetopt.posArgIndex += 1;
            } else {
                // not expected; fall back to treating as the option without arg
                xoptarg = NULL;
                g_xgetopt.posArgIndex += 1;
            }
        } else {
            // No concatenated argument; look to the next argv for the option's argument if needed
            if (requiresArg) {
                if ((g_xgetopt.posArgIndex + 1) < argc) {
                    xoptarg = argv[g_xgetopt.posArgIndex + 1];
                    g_xgetopt.posArgIndex += 2;
                } else {
                    // Missing argument; still advance
                    xoptarg = NULL;
                    g_xgetopt.posArgIndex += 1;
                }
            } else {
                xoptarg = NULL;
                g_xgetopt.posArgIndex += 1;
            }
        }

        // Update the global index for non-option arguments
        if (g_xgetopt.posArgIndex >= argc) {
            xoptind = argc;
        } else {
            xoptind = g_xgetopt.posArgIndex;
        }

        return optChar;
    }

    // If we reached here, no valid option found
    xoptind = g_xgetopt.posArgIndex;
    xoptarg = NULL;
    return EOF;
}

// ----------------------------------------------------------------------------------
// The focal method embedded for unit testing.
// This is a near-copy of the provided HandleSwitches, adapted to compile in this test
// environment. We intentionally keep the structure identical to preserve branch
// coverage semantics, but the external calls (Help, FatalError, cmsSetAdaptationState)
// are provided above in test scaffolding.
void HandleSwitches(int argc, char *argv[]) {
{
    int s;
    while ((s = xgetopt(argc, argv,
        "bBC:c:d:D:eEgGI:i:L:l:m:M:nNO:o:p:P:QqSsT:t:V:v:WwxX!:-:")) != EOF) {
    switch (s){
        case '-':
            if (strcmp(xoptarg, "help") == 0)
            {
                Help();
                exit(0);
            }
            else
            {
                FatalError("Unknown option - run without args to see valid ones.\n");
            }
            break;

        case '!': 
            IncludePart = xoptarg;
            break;
        case 'b':
        case 'B': 
            BlackPointCompensation = TRUE;
            break;
        case 'c':
        case 'C':
            PrecalcMode = atoi(xoptarg);
            if (PrecalcMode < 0 || PrecalcMode > 3)
                FatalError("Unknown precalc mode '%d'", PrecalcMode);
            break;
        case 'd':
        case 'D': {
            cmsFloat64Number ObserverAdaptationState = atof(xoptarg);
            if (ObserverAdaptationState < 0 || 
                ObserverAdaptationState > 1.0)
                FatalError("Adaptation states should be between 0 and 1");
            cmsSetAdaptationState(ObserverAdaptationState);
                  }
                  break;
        case 'e':
        case 'E': 
            lIsFloat = FALSE;
            break;
        case 'g':
        case 'G':
            GamutCheck = TRUE;
            break;
        case 'i':
        case 'I':
            if (lIsDeviceLink)
                FatalError("icctrans: Device-link already specified");
            cInProf = xoptarg;
            break;  
        case 'l':
        case 'L': 
            cInProf = xoptarg;
            lIsDeviceLink = TRUE;
            break;
            // No extra intents for proofing
        case 'm':
        case 'M':
            ProofingIntent = atoi(xoptarg);
            if (ProofingIntent > 3)
                FatalError("Unknown Proofing Intent '%d'", ProofingIntent);        
            break;      
            // For compatibility
        case 'n':
        case 'N':
            Verbose = 0;
            break;
            // Output profile        
        case 'o':
        case 'O':
            if (lIsDeviceLink)
                FatalError("icctrans: Device-link already specified"); 
            cOutProf = xoptarg;
            break;
            // Proofing profile
        case 'p':
        case 'P':
            cProofing = xoptarg;
            break;      
            // Quantize (get rid of decimals)
        case 'q':
        case 'Q': 
            lQuantize = TRUE;
            break;
            // Inhibit unbounded mode
        case 's':
        case 'S':
               lUnbounded = FALSE;
               break;
            // The intent
        case 't':
        case 'T':
            Intent = atoi(xoptarg);            
            break;
            // Verbosity level
        case 'V':
        case 'v':
            Verbose = atoi(xoptarg);
            if (Verbose < 0 || Verbose > 3) {
                FatalError("Unknown verbosity level '%d'", Verbose);
            }
            break;
            // Wide (16 bits)
        case 'W':
        case 'w':
            Width16 = TRUE;
            break;
            // Hexadecimal        
        case 'x':
        case 'X':
            InHexa = TRUE;
            break;
        default:            
            FatalError("Unknown option - run without args to see valid ones.\n");
            }       
    }

    // If output CGATS involved, switch to float
    if ((argc - xoptind) > 2) {
        lIsFloat = TRUE;
    }
}
}

// ----------------------------------------------------------------------------------
// Simple test harness

struct TestResult {
    std::string name;
    bool passed;
    std::string details;
};

// Helper to reset all globals to a known baseline before each test
void ResetGlobals() {
    IncludePart = NULL;
    BlackPointCompensation = FALSE;
    PrecalcMode = 0;
    AdaptationStateSet = 0.0;
    lIsFloat = FALSE;
    GamutCheck = FALSE;
    cInProf = NULL;
    lIsDeviceLink = FALSE;
    cOutProf = NULL;
    cProofing = NULL;
    ProofingIntent = 0;
    Verbose = 0;
    Width16 = FALSE;
    InHexa = FALSE;
    xoptind = 1;
    xoptarg = NULL;
}

// Test 1: No arguments should not alter defaults and should return normally
bool test_no_args() {
    ResetGlobals();
    char* argv0 = (char*)"prog";
    char* argv[] = { argv0 };
    int argc = 1;

    // Reset xgetopt state for this test
    ResetXGetopt(argc, argv);

    HandleSwitches(argc, argv);

    // Expectations: defaults remain unchanged
    bool ok = (BlackPointCompensation == FALSE) &&
              (PrecalcMode == 0) &&
              (AdaptationStateSet == 0.0) &&
              (GamutCheck == FALSE) &&
              (cInProf == NULL) &&
              (lIsDeviceLink == FALSE) &&
              (cOutProf == NULL) &&
              (Verbose == 0) &&
              (Width16 == FALSE) &&
              (InHexa == FALSE);
    return ok;
}

// Test 2: -b should enable BlackPointCompensation
bool test_switch_b() {
    ResetGlobals();
    char* argv[] = { (char*)"prog", (char*)"-b" };
    int argc = 2;

    ResetXGetopt(argc, argv);

    HandleSwitches(argc, argv);

    return (BlackPointCompensation == TRUE);
}

// Test 3: -c <n> should set PrecalcMode when within valid range
bool test_switch_c_valid() {
    ResetGlobals();
    char* argv[] = { (char*)"prog", (char*)"-c", (char*)"2" };
    int argc = 3;

    ResetXGetopt(argc, argv);

    HandleSwitches(argc, argv);

    return (PrecalcMode == 2);
}

// Test 4: -d <state> should set adaptation state within [0,1]
bool test_switch_d_valid() {
    ResetGlobals();
    char* argv[] = { (char*)"prog", (char*)"-d", (char*)"0.5" };
    int argc = 3;

    ResetXGetopt(argc, argv);

    HandleSwitches(argc, argv);

    return (AdaptationStateSet == 0.5);
}

// Test 5: -i <inProf> should set input profile
bool test_switch_i() {
    ResetGlobals();
    char* argv[] = { (char*)"prog", (char*)"-i", (char*)"in.icc" };
    int argc = 3;

    ResetXGetopt(argc, argv);

    HandleSwitches(argc, argv);

    return (cInProf != NULL && std::strcmp(cInProf, "in.icc") == 0);
}

// Test 6: -l <prof> should set cInProf and mark device-link
bool test_switch_l() {
    ResetGlobals();
    char* argv[] = { (char*)"prog", (char*)"-l", (char*)"device.icc" };
    int argc = 3;

    ResetXGetopt(argc, argv);

    HandleSwitches(argc, argv);

    return (cInProf != NULL && std::strcmp(cInProf, "device.icc") == 0 && lIsDeviceLink == TRUE);
}

// Test 7: Multiple options in one go with no fatal paths
bool test_switch_multiple() {
    ResetGlobals();
    char* argv[] = {
        (char*)"prog",
        (char*)"-b",
        (char*)"-c", (char*)"1",
        (char*)"-d", (char*)"0.2",
        (char*)"-n",      // Verbose = 0
        (char*)"-q",       // lQuantize = TRUE
        (char*)"-s",       // lUnbounded = FALSE
        (char*)"-t", (char*)"2",
        (char*)"-V", (char*)"1",
        (char*)"-W",
        (char*)"-x",
    };
    int argc = sizeof(argv)/sizeof(argv[0]);

    ResetXGetopt(argc, argv);

    // Ensure initial state
    HandleSwitches(argc, argv);

    // Validate subset of state changes
    bool ok = (BlackPointCompensation == TRUE) &&
              (PrecalcMode == 1) &&
              (AdaptationStateSet == 0.2) &&
              (Verbose == 0) &&
              (Width16 == TRUE) &&
              (InHexa == TRUE) &&
              (lUnbounded == FALSE); // expecting the lUnbounded set to FALSE by -s

    return ok;
}

// Test 8: -! <part> should set IncludePart to provided argument
bool test_switch_exclaim() {
    ResetGlobals();
    char* argv[] = { (char*)"prog", (char*)"-!", (char*)"PART_A" };
    int argc = 3;

    ResetXGetopt(argc, argv);

    HandleSwitches(argc, argv);

    return (IncludePart != NULL && std::strcmp(IncludePart, "PART_A") == 0);
}

// ----------------------------------------------------------------------------------
// Entry point for tests
int main() {
    // Initialize or reset any global state used by the tests
    ResetGlobals();

    std::vector<TestResult> results;

    // Test 1
    {
        TestResult tr;
        tr.name = "TestNoArgs";
        tr.passed = test_no_args();
        tr.details = tr.passed ? "Passed" : "Failed";
        results.push_back(tr);
    }

    // Test 2
    {
        TestResult tr;
        tr.name = "TestFlag_B";
        tr.passed = test_switch_b();
        tr.details = tr.passed ? "Passed" : "Failed";
        results.push_back(tr);
    }

    // Test 3
    {
        TestResult tr;
        tr.name = "TestFlag_C_Valid";
        tr.passed = test_switch_c_valid();
        tr.details = tr.passed ? "Passed" : "Failed";
        results.push_back(tr);
    }

    // Test 4
    {
        TestResult tr;
        tr.name = "TestFlag_D_Valid";
        tr.passed = test_switch_d_valid();
        tr.details = tr.passed ? "Passed" : "Failed";
        results.push_back(tr);
    }

    // Test 5
    {
        TestResult tr;
        tr.name = "TestFlag_I";
        tr.passed = test_switch_i();
        tr.details = tr.passed ? "Passed" : "Failed";
        results.push_back(tr);
    }

    // Test 6
    {
        TestResult tr;
        tr.name = "TestFlag_L";
        tr.passed = test_switch_l();
        tr.details = tr.passed ? "Passed" : "Failed";
        results.push_back(tr);
    }

    // Test 7
    {
        TestResult tr;
        tr.name = "TestMultipleFlags";
        tr.passed = test_switch_multiple();
        tr.details = tr.passed ? "Passed" : "Failed";
        results.push_back(tr);
    }

    // Test 8
    {
        TestResult tr;
        tr.name = "TestBangExclaim";
        tr.passed = test_switch_exclaim();
        tr.details = tr.passed ? "Passed" : "Failed";
        results.push_back(tr);
    }

    // Print summary
    int passed = 0;
    for (const auto& r : results) {
        std::cout << r.name << ": " << (r.passed ? "PASS" : "FAIL") << " - " << r.details << std::endl;
        if (r.passed) ++passed;
    }
    std::cout << "Total: " << results.size() << ", Passed: " << passed << std::endl;

    // Return success if all tests pass
    return (passed == (int)results.size()) ? 0 : 1;
}