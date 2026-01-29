// cmstypes_test.cpp
// Lightweight, self-contained unit tests for the focal function:
// cmsBool WriteCountAndString(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsMLU* mlu, const char* Section)
// Note: This is a self-contained emulation to exercise the function logic without requiring the full
// LittleCMS dependency tree. It mocks essential interactions (memory allocation, IO, and MLU retrieval).

#include <cstring>
#include <map>
#include <iostream>
#include <string>
#include <lcms2_internal.h>


// Lightweight type aliases to resemble the focal dependencies
typedef int cmsBool;
typedef unsigned int cmsUInt32Number;
typedef void* cmsContext;

// TRUE / FALSE constants used by the original code
#define TRUE 1
#define FALSE 0

// Mocked structures to match the focal function's signature
typedef struct _cms_typehandler_struct {
    cmsUInt32Number ContextID;
} _cms_typehandler_struct;

typedef struct cmsIOHANDLER {
    cmsBool (*Write)(struct cmsIOHANDLER* io, cmsUInt32Number size, const void* buffer);
} cmsIOHANDLER;

typedef struct cmsMLU {
    // Placeholder; our mocks ignore internal representation, using a global map instead
    int dummy;
} cmsMLU;

// Prototypes for the dependencies (mocked in this test)
extern "C" cmsBool _cmsWriteUInt32Number(cmsIOHANDLER* io, cmsUInt32Number value);
extern "C" void* _cmsMalloc(cmsContext ContextID, cmsUInt32Number size);
extern "C" void _cmsFree(cmsContext ContextID, void* p);
extern "C" cmsUInt32Number cmsMLUgetASCII(cmsMLU* mlu, const char* lang, const char* Section, char* Text, cmsUInt32Number TextSize);

// Forward declaration of the focal function under test (we re-provide a self-contained version here)
cmsBool WriteCountAndString(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsMLU* mlu, const char* Section);

// Global test state to control mocks
static bool g_failWriteUInt32Number = false;
static bool g_forceSecondCMsMLUGetASCIIFail = false;
static std::map<std::string, std::string> g_mluEntries; // Section -> Text
static std::string g_lastWrittenText;
static cmsUInt32Number g_lastWrittenSize = 0;

// Mock implementations (test doubles)

// _cmsMalloc / _cmsFree: simple heap management
void* _cmsMalloc(cmsContext /*ContextID*/, cmsUInt32Number size) {
    if (size == 0) return nullptr;
    return std::malloc(static_cast<size_t>(size));
}
void _cmsFree(cmsContext /*ContextID*/, void* p) {
    std::free(p);
}

// _cmsWriteUInt32Number: controllable to simulate success/failure
cmsBool _cmsWriteUInt32Number(cmsIOHANDLER* /*io*/, cmsUInt32Number /*value*/) {
    return g_failWriteUInt32Number ? FALSE : TRUE;
}

// cmsMLUgetASCII: mock MLU data access
cmsUInt32Number cmsMLUgetASCII(cmsMLU* /*mlu*/, const char* /*lang*/, const char* Section, char* Text, cmsUInt32Number TextSize) {
    // Maintain a per-test counter to emulate the two-step process (size query, then content)
    static int callCount = 0;
    ++callCount;

    if (Text == nullptr) {
        // Size query: return length(Text) + 1 for NUL terminator
        auto it = g_mluEntries.find(Section ? Section : "");
        cmsUInt32Number len = (it != g_mluEntries.end()) ? static_cast<cmsUInt32Number>(it->second.size()) : 0;
        return len + 1;
    } else {
        // Content fill: on the second call we may simulate a failure if requested
        if (g_forceSecondCMsMLUGetASCIIFail && callCount == 2) {
            // Reset counter for subsequent tests
            callCount = 0;
            return 0;
        }

        auto it = g_mluEntries.find(Section ? Section : "");
        const std::string& s = (it != g_mluEntries.end()) ? it->second : "";
        cmsUInt32Number needed = static_cast<cmsUInt32Number>(s.size()) + 1;
        if (TextSize < needed) {
            // Not enough space
            return 0;
        }
        // Copy string including terminator
        std::memcpy(Text, s.c_str(), s.size());
        Text[s.size()] = '\0';
        // Reset counter after a successful fill
        callCount = 0;
        return needed;
    }
}

// The focal function copied here as a self-contained version for testing.
// In a real test, you would link against the actual implementation from cmstypes.c.
cmsBool WriteCountAndString(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsMLU* mlu, const char* Section) {
{
 cmsUInt32Number TextSize;
 char* Text;
    TextSize = cmsMLUgetASCII(mlu, "PS", Section, NULL, 0);
    Text     = (char*) _cmsMalloc(self ->ContextID, TextSize);
    if (!_cmsWriteUInt32Number(io, TextSize)) return FALSE;
    if (cmsMLUgetASCII(mlu, "PS", Section, Text, TextSize) == 0) return FALSE;
    if (!io ->Write(io, TextSize, Text)) return FALSE;
    _cmsFree(self ->ContextID, Text);
    return TRUE;
}
}

// Simple assertion helpers (non-terminating, to maximize coverage)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond) do { ++g_total_tests; if(!(cond)) { std::cerr << "Test failed: " #cond " at " << __FILE__ << ":" << __LINE__ << std::endl; ++g_failed_tests; } } while(0)
#define EXPECT_EQ(a, b) do { ++g_total_tests; if(!((a) == (b))) { std::cerr << "Test failed: " #a " == " #b " (actual: " << (a) << " != " << (b) << ") at " << __FILE__ << ":" << __LINE__ << std::endl; ++g_failed_tests; } } while(0)

// Helper to reset per-test globals
void reset_test_state() {
    g_failWriteUInt32Number = false;
    g_forceSecondCMsMLUGetASCIIFail = false;
    g_mluEntries.clear();
    g_lastWrittenText.clear();
    g_lastWrittenSize = 0;
    // Reset last-written tracking by reattaching a new mock Write
}

// Mock IO write function: captures data written by the focal function
cmsBool Mock_IO_Write(cmsIOHANDLER* /*io*/, cmsUInt32Number size, const void* buffer) {
    if (buffer == nullptr && size != 0) return FALSE;
    g_lastWrittenSize = size;
    g_lastWrittenText.assign(static_cast<const char*>(buffer), static_cast<const char*>(buffer) + size);
    return TRUE;
}

// Test cases

void test_WriteCountAndString_Success_Path() {
    reset_test_state();

    // Arrange
    g_mluEntries["SectionA"] = "HelloCMS"; // Text to be written
    g_failWriteUInt32Number = false;
    g_forceSecondCMsMLUGetASCIIFail = false;

    // Prepare self, io, mlu
    _cms_typehandler_struct self;
    self.ContextID = 123; // arbitrary

    cmsIOHANDLER io;
    io.Write = Mock_IO_Write;

    cmsMLU mluDummy; // content is provided via g_mluEntries; details unused

    const char* Section = "SectionA";

    // Act
    cmsBool result = WriteCountAndString(&self, &io, &mluDummy, Section);

    // Assert
    EXPECT_TRUE(result == TRUE);
    // The first cmsMLUgetASCII call returns size = len + 1 = 9 (for "HelloCMS"), so expect 9
    EXPECT_EQ(g_lastWrittenSize, static_cast<cmsUInt32Number>(9));
    EXPECT_EQ(g_lastWrittenText, std::string("HelloCMS"));
}

void test_WriteCountAndString_WriteUInt32_Fails() {
    reset_test_state();

    g_mluEntries["SectionB"] = "ABCDEF";

    _cms_typehandler_struct self;
    self.ContextID = 999;

    cmsIOHANDLER io;
    io.Write = Mock_IO_Write; // even though Write won't be reached due to precondition

    cmsMLU mluDummy;
    const char* Section = "SectionB";

    g_failWriteUInt32Number = true; // force failure on _cmsWriteUInt32Number

    cmsBool result = WriteCountAndString(&self, &io, &mluDummy, Section);

    EXPECT_TRUE(result == FALSE);
    // Ensure no write occurred
    EXPECT_EQ(g_lastWrittenSize, static_cast<cmsUInt32Number>(0));
    EXPECT_EQ(g_lastWrittenText, std::string(""));
}

void test_WriteCountAndString_MLU_Content_Fails() {
    reset_test_state();

    g_mluEntries["SectionC"] = "XYZ"; // content available
    g_forceSecondCMsMLUGetASCIIFail = true; // Simulate failure on the second cmsMLUgetASCII call (content read)

    _cms_typehandler_struct self;
    self.ContextID = 42;

    cmsIOHANDLER io;
    io.Write = Mock_IO_Write;

    cmsMLU mluDummy;
    const char* Section = "SectionC";

    cmsBool result = WriteCountAndString(&self, &io, &mluDummy, Section);

    // Expect failure due to cmsMLUgetASCII returning 0 on content retrieval
    EXPECT_TRUE(result == FALSE);
}

void test_WriteCountAndString_IOWrite_Fails() {
    reset_test_state();

    g_mluEntries["SectionD"] = "DATA"; // content
    g_forceSecondCMsMLUGetASCIIFail = false;

    _cms_typehandler_struct self;
    self.ContextID = 7;

    cmsIOHANDLER io;
    // Attach a Write function that simulates a failure
    io.Write = [](cmsIOHANDLER* /*io*/, cmsUInt32Number /*size*/, const void* /*buffer*/) -> cmsBool {
        return FALSE;
    };

    cmsMLU mluDummy;
    const char* Section = "SectionD";

    cmsBool result = WriteCountAndString(&self, &io, &mluDummy, Section);

    EXPECT_TRUE(result == FALSE);
}

// Main harness
int main() {
    // Run tests
    test_WriteCountAndString_Success_Path();
    test_WriteCountAndString_WriteUInt32_Fails();
    test_WriteCountAndString_MLU_Content_Fails();
    test_WriteCountAndString_IOWrite_Fails();

    // Summary
    std::cout << "Total tests: " << g_total_tests << ", Failures: " << g_failed_tests << std::endl;
    if (g_failed_tests > 0) {
        std::cout << "Some tests failed. See details above." << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
}