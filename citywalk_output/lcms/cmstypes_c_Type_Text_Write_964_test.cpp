// Minimal self-contained unit-test for the focal method logic of Type_Text_Write.
// Note: This test recreates the relevant pieces of the CMS LCMS environment
// in order to exercise the core logic of Type_Text_Write without requiring
// the full external library. It follows the requested test-generation approach
// with lightweight, non-terminating EXPECT-like assertions.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


// Domain-specific simplifications and mocks to mimic necessary APIs.

typedef unsigned int cmsUInt32Number;
typedef bool cmsBool;

// Self-contained type declarations to mirror the original signatures
// (kept minimal for testability in a standalone file).
struct _cms_typehandler_struct {
    void* ContextID;
};

struct cmsMLU {};

struct cmsIOHANDLER {
    // Signature matches: cmsBool Write(struct cmsIOHANDLER*, cmsUInt32Number, const char*)
    cmsBool (*Write)(struct cmsIOHANDLER*, cmsUInt32Number, const char*);
};

// Forward declarations of mocked dependencies
static cmsUInt32Number cmsMLUgetASCII(struct cmsMLU* mlu, int Language, int Country, char* Buffer, cmsUInt32Number BufferSize);

// Global (test-scoped) state to drive mocks
static cmsUInt32Number g_size_first_call = 6;        // default size returned on first call
static bool g_return_zero_on_first_call = false;     // if true, first call returns 0
static bool g_malloc_ok = true;                      // simulate _cmsMalloc success
static bool g_write_ok = true;                       // simulate io->Write success
static bool g_free_called = false;                   // tracks _cmsFree invocation
static void* g_last_alloc_ptr = nullptr;             // last allocated block for leak checks
static cmsUInt32Number g_last_written_size = 0;      // last write size observed
static std::string g_last_written_data;               // last written data (raw bytes including embedded '\\0')
static const char* g_mock_text = "Hello";            // text to return on second cmsMLUgetASCII call

static const int cmsNoLanguage = 0;
static const int cmsNoCountry  = 0;

// cmsUNUSED_PARAMETER macro (as in CMS code)
#define cmsUNUSED_PARAMETER(x) (void)(x)

// Mock implementation of _cmsMalloc / _cmsFree
static void* _cmsMalloc(void* ContextID, cmsUInt32Number size) {
    (void)ContextID; // context not used in mock
    if (!g_malloc_ok) return NULL;
    void* p = std::malloc((size_t)size);
    if (p) g_last_alloc_ptr = p;
    return p;
}
static void _cmsFree(void* ContextID, void* ptr) {
    (void)ContextID;
    if (ptr) {
        std::free(ptr);
        g_free_called = true;
        g_last_alloc_ptr = nullptr;
    }
}

// Mock Write function used by the Type_Text_Write call
static cmsBool MockWrite(struct cmsIOHANDLER* io, cmsUInt32Number size, const char* Buffer) {
    (void)io; // not used by test logic
    g_last_written_size = size;
    g_last_written_data.assign(Buffer, size);
    return g_write_ok;
}

// cmsMLUgetASCII mock: two-call behavior
// - First call (with Buffer == nullptr) returns the size required (including trailing '\0')
// - Second call fills Buffer with the string and returns (value ignored by caller)
static cmsUInt32Number cmsMLUgetASCII(struct cmsMLU* mlu, int Language, int Country, char* Buffer, cmsUInt32Number BufferSize) {
    (void)mlu; (void)Language; (void)Country;
    if (Buffer == nullptr) {
        // First call: return size requested
        if (g_return_zero_on_first_call) return 0;
        return g_size_first_call;
    } else {
        // Second call: fill with g_mock_text, respecting BufferSize
        const char* src = g_mock_text;
        cmsUInt32Number need = (cmsUInt32Number)std::strlen(src) + 1; // include terminator
        cmsUInt32Number toCopy = BufferSize < need ? BufferSize : need;
        std::memcpy(Buffer, src, (size_t)toCopy);
        // If BufferSize is smaller than needed, ensure last byte is still assigned as much as possible
        return 0;
    }
}

// Implementation of the focal method Type_Text_Write in this test file.
// It mirrors the behavior described in the prompt, using the mocked dependencies above.
static cmsBool Type_Text_Write(struct _cms_typehandler_struct* self, struct cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems)
{
{
    cmsMLU* mlu = (cmsMLU*) Ptr;
    cmsUInt32Number size;
    cmsBool  rc;
    char* Text;
    // Get the size of the string. Note there is an extra "\0" at the end
    size = cmsMLUgetASCII(mlu, cmsNoLanguage, cmsNoCountry, NULL, 0);
    if (size == 0) return false;       // Cannot be zero!
    // Create memory
    Text = (char*) _cmsMalloc(self ->ContextID, size);
    if (Text == NULL) return false;
    cmsMLUgetASCII(mlu, cmsNoLanguage, cmsNoCountry, Text, size);
    // Write it, including separator
    rc = io ->Write(io, size, Text);
    _cmsFree(self ->ContextID, Text);
    return rc;
    cmsUNUSED_PARAMETER(nItems);
}
}

// Simple test harness state
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { ++g_tests_run; if(!(cond)) { ++g_tests_failed; std::cerr << "FAIL: " << (msg) << "\n"; } } while(0)
#define EXPECT_FALSE(cond, msg) do { ++g_tests_run; if((cond))  { ++g_tests_failed; std::cerr << "FAIL: " << (msg) << "\n"; } } while(0)
#define RESET_TEST_STATE() do { g_size_first_call = 6; g_return_zero_on_first_call = false; g_malloc_ok = true; g_write_ok = true; g_free_called = false; g_last_alloc_ptr = nullptr; g_last_written_size = 0; g_last_written_data.clear(); g_tests_run = 0; g_tests_failed = 0; } while(0)

// Test 1: When the initial size query returns 0, Type_Text_Write should fail (return FALSE)
static void test_SizeZeroReturnsFalse() {
    RESET_TEST_STATE();

    // Configure: first cmsMLUgetASCII call should yield size 0
    g_return_zero_on_first_call = true;

    struct _cms_typehandler_struct self;
    self.ContextID = nullptr;

    cmsIOHANDLER io;
    io.Write = MockWrite;

    cmsMLU mlu;
    void* Ptr = &mlu;

    cmsBool result = Type_Text_Write(&self, &io, Ptr, 0);

    EXPECT_TRUE(result == false, "Type_Text_Write should return FALSE when first size query yields 0");
}

// Test 2: Successful path: proper size, allocation, write, and free
static void test_SuccessWrite() {
    RESET_TEST_STATE();

    // Configure: first cmsMLUgetASCII call yields size (6 for "Hello" + '\0')
    g_size_first_call = 6;
    g_return_zero_on_first_call = false;
    g_malloc_ok = true;
    g_write_ok = true;

    struct _cms_typehandler_struct self;
    self.ContextID = (void*)0xDEADBEEF; // arbitrary context id

    cmsIOHANDLER io;
    io.Write = MockWrite;

    cmsMLU mlu;
    void* Ptr = &mlu;

    cmsBool result = Type_Text_Write(&self, &io, Ptr, 0);

    EXPECT_TRUE(result == true, "Type_Text_Write should return TRUE on successful write");
    EXPECT_TRUE(g_last_alloc_ptr != nullptr, "Memory should be allocated for Text buffer");
    EXPECT_TRUE(g_free_called, "Allocated buffer should be freed after write");
    EXPECT_TRUE(g_last_written_size == 6, "Write should be invoked with correct size (6)");
    // g_last_written_data should contain "Hello" plus trailing '\0'
    if (g_last_written_data.size() >= 6) {
        bool prefixOk = (g_last_written_data.substr(0,5) == std::string("Hello"));
        bool terminatorOk = (g_last_written_data[5] == '\0');
        EXPECT_TRUE(prefixOk && terminatorOk, "Written data should be 'Hello' with trailing null terminator");
    } else {
        EXPECT_TRUE(false, "Written data length is incorrect");
    }
}

// Test 3: Allocation failure should cause Type_Text_Write to return FALSE
static void test_MallocFailure() {
    RESET_TEST_STATE();

    g_malloc_ok = false; // simulate allocator failure

    struct _cms_typehandler_struct self;
    self.ContextID = nullptr;

    cmsIOHANDLER io;
    io.Write = MockWrite;

    cmsMLU mlu;
    void* Ptr = &mlu;

    cmsBool result = Type_Text_Write(&self, &io, Ptr, 0);

    EXPECT_TRUE(result == false, "Type_Text_Write should return FALSE when allocation fails");
    EXPECT_TRUE(g_last_alloc_ptr == nullptr, "No allocation should be held when allocation fails");
    EXPECT_FALSE(g_free_called, "No Free should be called when allocation fails");
}

// Test 4: Write failure should cause Type_Text_Write to return FALSE after successful allocation
static void test_WriteFailure() {
    RESET_TEST_STATE();

    g_size_first_call = 6;
    g_malloc_ok = true;
    g_write_ok = false; // simulate write failure

    struct _cms_typehandler_struct self;
    self.ContextID = nullptr;

    cmsIOHANDLER io;
    io.Write = MockWrite;

    cmsMLU mlu;
    void* Ptr = &mlu;

    cmsBool result = Type_Text_Write(&self, &io, Ptr, 0);

    EXPECT_TRUE(result == false, "Type_Text_Write should return FALSE when write operation fails");
}

// Test 5: Ensure that when first size query returns 0 (explicit), test handles correctly without crash
static void test_FirstCallReturnsZeroExplicit() {
    RESET_TEST_STATE();

    g_size_first_call = 0;     // size query returns 0
    g_return_zero_on_first_call = true;

    struct _cms_typehandler_struct self;
    self.ContextID = nullptr;

    cmsIOHANDLER io;
    io.Write = MockWrite;

    cmsMLU mlu;
    void* Ptr = &mlu;

    cmsBool result = Type_Text_Write(&self, &io, Ptr, 0);

    EXPECT_TRUE(result == false, "Type_Text_Write should handle explicit zero first size without crash");
}

// Runner
int main() {
    // Run tests
    test_SizeZeroReturnsFalse();
    test_SuccessWrite();
    test_MallocFailure();
    test_WriteFailure();
    test_FirstCallReturnsZeroExplicit();

    // Summary
    std::cout << "Tests run: " << g_tests_run << "\n";
    std::cout << "Tests failed: " << g_tests_failed << "\n";

    if (g_tests_failed != 0) {
        std::cerr << "Some tests FAILED.\n";
        return 1;
    }
    std::cout << "All tests PASSED.\n";
    return 0;
}