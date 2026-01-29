// Unit tests for the focal method: Type_Text_Read in cmstypes.c
// NOTE: This test suite is written for C++11 without GoogleTest.
// It uses lightweight EXPECT_* macros that do not terminate on failure and a
// small test harness that runs tests from main().

#include <cstring>
#include <iostream>
#include <climits>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include the necessary LittleCMS types and functions
// The test targets a C API; ensure C linkage for the function under test.
extern "C" {
}

// Forward-declare the minimal layout of the type handler struct used by the focal method.
// We only rely on the ContextID member being accessible.
// This is a common testing technique when the real internal layout is not exposed.
struct _cms_typehandler_struct {
    cmsContext ContextID;
};

// Forward declaration of the function under test (as provided by the library).
extern "C" void *Type_Text_Read(struct _cms_typehandler_struct* self,
                                cmsIOHANDLER* io,
                                cmsUInt32Number* nItems,
                                cmsUInt32Number SizeOfTag);

// Lightweight assertion helpers (non-terminating)
#define EXPECT_TRUE(cond) do {                                           \
    if (!(cond)) {                                                       \
        std::cerr << "[EXPECT_TRUE] Failed at line " << __LINE__           \
                  << ": " << #cond << std::endl;                       \
        }                                                                  \
} while (0)

#define EXPECT_FALSE(cond) do {                                          \
    if (cond) {                                                          \
        std::cerr << "[EXPECT_FALSE] Failed at line " << __LINE__          \
                  << ": " << #cond << std::endl;                       \
        }                                                                 \
} while (0)

#define EXPECT_EQ(a,b) do {                                              \
    if (!((a) == (b))) {                                                 \
        std::cerr << "[EXPECT_EQ] Failed at line " << __LINE__             \
                  << ": " << #a << " (" << (a) << ") != " << #b            \
                  << " (" << (b) << ")" << std::endl;                   \
    }                                                                     \
} while (0)

#define EXPECT_NEQ(a,b) do {                                             \
    if ((a) == (b)) {                                                    \
        std::cerr << "[EXPECT_NEQ] Failed at line " << __LINE__            \
                  << ": " << #a << " should not equal " << #b << std::endl; \
    }                                                                    \
} while (0)

#define EXPECT_NOTNULL(p) do {                                           \
    if ((p) == nullptr) {                                              \
        std::cerr << "[EXPECT_NOTNULL] Failed at line " << __LINE__        \
                  << ": " << #p << " is NULL" << std::endl;             \
    }                                                                    \
} while (0)

#define EXPECT_NULL(p) do {                                              \
    if ((p) != nullptr) {                                              \
        std::cerr << "[EXPECT_NULL] Failed at line " << __LINE__          \
                  << ": " << #p << " is not NULL" << std::endl;         \
    }                                                                    \
} while (0)

// -----------------------------------------------------------------------------
// Mock I/O layer to feed Type_Text_Read with controllable input data
// The cmsIOHANDLER::Read callback will be invoked by the focal function.
// We provide two variants: one that succeeds always (reads all data) and one
// that simulates a partial read (to exercise error handling).
// -----------------------------------------------------------------------------

// Context carried by the mock IO (data buffer and read position)
struct MockIOContext {
    const char* data;
    cmsUInt32Number len;
    cmsUInt32Number pos;
};

// Success Read implementation: copies requested bytes from buffer to dest
static cmsUInt32Number MockRead_Success(cmsIOHANDLER* io, void* Buffer,
                                       cmsUInt32Number size, cmsUInt32Number Count)
{
    // io->Context is assumed to point to MockIOContext
    MockIOContext* ctx = reinterpret_cast<MockIOContext*>(io->Context);
    if (ctx == nullptr) return 0;

    cmsUInt32Number remaining = (ctx->len > ctx->pos) ? (ctx->len - ctx->pos) : 0;
    cmsUInt32Number toCopy = Count;
    if (toCopy > remaining) toCopy = remaining;
    if (toCopy > 0 && size > 0 && Buffer != nullptr) {
        std::memcpy(Buffer, ctx->data + ctx->pos, static_cast<size_t>(toCopy) * static_cast<size_t>(size));
        ctx->pos += toCopy;
    }
    return toCopy;
}

// Failure Read implementation: simulates partial read (less than requested)
static cmsUInt32Number MockRead_Fail(cmsIOHANDLER* io, void* Buffer,
                                    cmsUInt32Number size, cmsUInt32Number Count)
{
    // Always read zero bytes to simulate failure
    (void)io; (void)Buffer; (void)size; (void)Count;
    return 0;
}

// Helper to initialize a cmsIOHANDLER with a given Read function and a context
static void InitMockIO(cmsIOHANDLER* io,
                       cmsReadFunc ReadFunc,
                       MockIOContext* ctx)
{
    // Zero-initialize then set function pointer and context
    std::memset(io, 0, sizeof(*io));
    // The actual cmsIOHANDLER struct in lcms exposes a Read callback named 'Read'
    // and a Context pointer used by the callback to retrieve user data.
    // We assume the field names Context and Read exist in the public API.
    io->Context = ctx;
    io->Read = ReadFunc;
    // Write is not used in these tests
    io->Write = nullptr;
}

// -----------------------------------------------------------------------------
// Test 1: UINT_MAX SizeOfTag should trigger Error path and return NULL
// -----------------------------------------------------------------------------
void test_Text_Read_SizeOfTag_UIntMax_ReturnsNull()
{
    // Prepare a minimal self with a dummy context
    struct _cms_typehandler_struct self;
    self.ContextID = reinterpret_cast<cmsContext>(0);

    // Create an empty MockIO (Read won't be called due to SizeOfTag check)
    cmsIOHANDLER io;
    cmsUInt32Number nItems = 0;

    // SizeOfTag == UINT_MAX should cause early goto Error and return NULL
    void* result = Type_Text_Read(&self, &io, &nItems, UINT_MAX);

    EXPECT_NULL(result);
    // nItems should remain 0 on error
    EXPECT_EQ(nItems, 0);
}

// -----------------------------------------------------------------------------
// Test 2: Normal successful read path
// - Reads 5 bytes from the mock IO into a Text buffer
// - Should return a non-NULL cmsMLU*, and nItems should be set to 1
// -----------------------------------------------------------------------------
void test_Text_Read_Success_ReadsTextAndReturnsMlu()
{
    // Self with dummy context
    struct _cms_typehandler_struct self;
    self.ContextID = reinterpret_cast<cmsContext>(0);

    // Prepare input data "Hello" (5 bytes)
    MockIOContext ctx;
    const char* payload = "Hello";
    ctx.data = payload;
    ctx.len = static_cast<cmsUInt32Number>(std::strlen(payload));
    ctx.pos = 0;

    cmsIOHANDLER io;
    InitMockIO(&io, MockRead_Success, &ctx);

    cmsUInt32Number nItems = 0;

    void* result = Type_Text_Read(&self, &io, &nItems, ctx.len);
    EXPECT_NOTNULL(result);
    if (result != nullptr) {
        // nItems should be 1 on success
        EXPECT_EQ(nItems, 1);

        // Attempt to release resources allocated by the library
        cmsMLU* mlu = reinterpret_cast<cmsMLU*>(result);
        cmsMLUfree(mlu); // Clean up
    }
}

// -----------------------------------------------------------------------------
// Test 3: Read failure path (partial read) should return NULL and clean up
// - Simulates a failed Read (0 bytes read when Count bytes requested)
// -----------------------------------------------------------------------------
void test_Text_Read_ReadFailure_ReturnsNull()
{
    // Self with dummy context
    struct _cms_typehandler_struct self;
    self.ContextID = reinterpret_cast<cmsContext>(0);

    // Prepare input data (not actually used since Read will fail)
    MockIOContext ctx;
    const char* payload = "ABCDE";
    ctx.data = payload;
    ctx.len = static_cast<cmsUInt32Number>(std::strlen(payload));
    ctx.pos = 0;

    cmsIOHANDLER io;
    InitMockIO(&io, MockRead_Fail, &ctx);

    cmsUInt32Number nItems = 0;

    void* result = Type_Text_Read(&self, &io, &nItems, ctx.len);
    EXPECT_NULL(result);
    // Ensure nItems remains 0 on error path
    EXPECT_EQ(nItems, 0);
}

// -----------------------------------------------------------------------------
// Main: Run all tests
// -----------------------------------------------------------------------------
int main()
{
    std::cout << "Running Type_Text_Read unit tests (no GTest, plain C++11)\n";

    test_Text_Read_SizeOfTag_UIntMax_ReturnsNull();
    test_Text_Read_Success_ReadsTextAndReturnsMlu();
    test_Text_Read_ReadFailure_ReturnsNull();

    std::cout << "Tests completed.\n";
    return 0;
}