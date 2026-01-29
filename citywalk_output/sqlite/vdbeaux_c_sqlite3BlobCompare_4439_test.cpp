// Test suite for sqlite3BlobCompare function from vdbeaux.c
// Target: C++11, no Google Test. Use a lightweight in-process test harness.
// Assumptions:
// - The environment provides the public SQLite C API header "sqlite3.h".
// - We rely on the Mem struct and MEM_Zero flag from SQLite.
// - sqlite3BlobCompare is linked from the project under test (vdbeaux.c).
// - Tests exercise true/false branches of the predicate logic in sqlite3BlobCompare.
// - We avoid private/internal APIs; we only use the public Mem type and sqlite3BlobCompare.

#include <functional>
#include <stdexcept>
#include <vector>
#include <sqlite3.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>


// Include SQLite public API
extern "C" {
}

// Lightweight test harness
static int g_failures = 0;

#define EXPECT_EQ(a, b) do { \
    if ((a) != (b)) { \
        std::cerr << "Expectation failed: " #a " == " #b " (" << (a) << " != " << (b) << ")" << std::endl; \
        ++g_failures; \
        throw std::runtime_error("EXPECT_EQ failed"); \
    } \
} while(0)

#define TEST_OK(msg) do { std::cout << "[PASS] " << msg << std::endl; } while(0)
#define TEST_FAIL(msg) do { std::cerr << "[FAIL] " << msg << std::endl; ++g_failures; } while(0)

// Helper to create a Mem value as a blob with given content.
// Note: We rely on the public Mem layout provided by sqlite3.h.
// The tests only manipulate fields required by sqlite3BlobCompare:
// - z: blob content
// - n: blob length
// - flags: may include MEM_Zero
// - u.nZero: number of leading/trailing zero bytes when MEM_Zero is used
static Mem makeBlobMem(const unsigned char* data, int n, unsigned int flags, int nZero) {
  Mem m;
  // Initialize fields used by sqlite3BlobCompare
  // Content pointer
  m.z = const_cast<unsigned char*>(data);
  // Length
  m.n = n;
  // Flags (e.g., MEM_Zero)
  m.flags = flags;
  // Access to u.nZero (as used by sqlite3BlobCompare)
  // Depending on SQLite version, the exact member may differ;
  // we rely on the common layout where u.nZero exists.
  // Some SQLite versions expose nZero directly; if not, this line may need adjustment.
  // We attempt to set through the nested union 'u' if present; otherwise fall back gracefully.
  // Since we can't rely on exact internal layout here, use a try-catch-like approach:
  // The following assignment will typically be valid in SQLite where Mem has u.nZero.
  // If not, this path may be a no-op on environments with a different layout.
#if defined(_MSC_VER)
  // MSVC often allows direct member access with unions
  m.u.nZero = nZero;
#else
  // GCC/Clang: attempt direct access; if not available, pragma or alternate layout would be needed.
  m.u.nZero = nZero;
#endif
  return m;
}

// Alternative helper when we need a zero z pointer (to simulate all-zero content)
static Mem makeBlobMemAllZero(int n, unsigned int flags, int nZero) {
  static unsigned char zeroBlob[256] = {0}; // enough for tests
  const unsigned char* data = zeroBlob;
  if (n > (int)sizeof(zeroBlob)) {
    // Fallback: allocate dynamic memory (kept simple for tests)
    unsigned char* dyn = new unsigned char[n];
    std::memset(dyn, 0, n);
    Mem m = makeBlobMem(dyn, n, flags, nZero);
    // Note: we won't free dyn here to avoid stability concerns in test harness
    // (leak tolerated in small unit tests for clarity)
    return m;
  }
  return makeBlobMem(data, n, flags, nZero);
}

static void test_blobCompare_nonZero_equalContent() {
  // Case: both blobs non-zero, identical content -> expect 0
  const unsigned char blob1[] = { 'A', 'B', 'C' };
  const unsigned char blob2[] = { 'A', 'B', 'C' };

  Mem m1 = makeBlobMem(blob1, 3, 0, 0);
  Mem m2 = makeBlobMem(blob2, 3, 0, 0);

  int res = sqlite3BlobCompare(&m1, &m2);
  EXPECT_EQ(res, 0);
  TEST_OK("sqlite3BlobCompare: non-zero, equal content -> 0");
}

static void test_blobCompare_nonZero_lexicographic() {
  // Case: different content -> expect negative (or positive) according to lexicographic order
  const unsigned char blob1[] = { 'A', 'B', 'C' }; // "ABC"
  const unsigned char blob2[] = { 'A', 'B', 'D' }; // "ABD"

  Mem m1 = makeBlobMem(blob1, 3, 0, 0);
  Mem m2 = makeBlobMem(blob2, 3, 0, 0);

  int res = sqlite3BlobCompare(&m1, &m2);
  // "ABC" < "ABD" so expect negative
  EXPECT_EQ(res < 0, true);
  TEST_OK("sqlite3BlobCompare: non-zero, lexicographically less");
}

static void test_blobCompare_lengthDifferenceWithPrefix() {
  // Case: content same for min(n1,n2), lengths differ -> return n1 - n2
  const unsigned char blob1[] = { 'A', 'B', 'C' }; // n1 = 3
  const unsigned char blob2[] = { 'A', 'B', 'C', 'D' }; // n2 = 4

  Mem m1 = makeBlobMem(blob1, 3, 0, 0);
  Mem m2 = makeBlobMem(blob2, 4, 0, 0);

  int res = sqlite3BlobCompare(&m1, &m2);
  EXPECT_EQ(res, -1);
  TEST_OK("sqlite3BlobCompare: length difference after equal prefix -> n1 - n2");
}

static void test_blobCompare_bothZero_flags() {
  // Case: both MEM_Zero flag set -> compare by u.nZero
  const unsigned char blob1[] = { 0, 0, 0 }; // content doesn't matter when zeros
  const unsigned char blob2[] = { 0, 0, 0 };

  Mem m1 = makeBlobMem(blob1, 3, MEM_Zero, 5); // nZero = 5
  Mem m2 = makeBlobMem(blob2, 3, MEM_Zero, 3); // nZero = 3

  int res = sqlite3BlobCompare(&m1, &m2);
  EXPECT_EQ(res, 5 - 3);
  TEST_OK("sqlite3BlobCompare: MEM_Zero on both -> compare by nZero values");
}

static void test_blobCompare_oneZero_and_nonAllZeroOther() {
  // Case: pB1 MEM_Zero, pB2 not MEM_Zero, pB2 not all zero -> expect -1
  const unsigned char blob2[] = { 'X' }; // non-zero content

  Mem m1 = makeBlobMemAllZero(2, MEM_Zero, 2); // nZero = 2
  Mem m2 = makeBlobMem(blob2, 1, 0, 0);       // not MEM_Zero, not all-zero

  int res = sqlite3BlobCompare(&m1, &m2);
  EXPECT_EQ(res, -1);
  TEST_OK("sqlite3BlobCompare: MEM_Zero vs non-zero blob -> -1 when other is non-all-zero");
}

static void test_blobCompare_nonZero_vs_zeroAllZeroOther() {
  // Case: pB2 MEM_Zero, pB1 not MEM_Zero, pB1 all-zero -> expect n1 - nZero
  const unsigned char blob1[] = { 0, 0, 0 }; // all-zero content

  Mem m1 = makeBlobMem(blob1, 3, 0, 0);        // pB1 not MEM_Zero but all-zero
  Mem m2 = makeBlobMemAllZero(3, MEM_Zero, 4); // pB2 MEM_Zero with nZero = 4

  int res = sqlite3BlobCompare(&m1, &m2);
  // If pB1 isAllZero, path returns n1 - pB2->u.nZero
  EXPECT_EQ(res, 3 - 4);
  TEST_OK("sqlite3BlobCompare: non-zero blob vs MEM_Zero-all-zero -> n1 - nZero");
}

static void test_blobCompare_mixed_zero_and_nonzero_branch_plus_content() {
  // Case: pB1 non-zero content, pB2 MEM_Zero, pB1 not all-zero -> expect +1
  const unsigned char blob1[] = { 'Z' }; // non-zero
  Mem m1 = makeBlobMem(blob1, 1, 0, 0);

  Mem m2 = makeBlobMemAllZero(2, MEM_Zero, 4); // nZero = 4

  int res = sqlite3BlobCompare(&m1, &m2);
  EXPECT_EQ(res, +1);
  TEST_OK("sqlite3BlobCompare: non-zero content with pB2 MEM_Zero all-zero -> +1");
}

static void test_blobCompare_allZeroBothDifferentNZero() {
  // Case: both MEM_Zero but different n values, ensure difference is returned
  const unsigned char blob1[] = { 0, 0, 0, 0 };
  const unsigned char blob2[] = { 0, 0, 0, 0 };

  Mem m1 = makeBlobMem(blob1, 4, MEM_Zero, 7);
  Mem m2 = makeBlobMem(blob2, 4, MEM_Zero, 2);

  int res = sqlite3BlobCompare(&m1, &m2);
  EXPECT_EQ(res, 7 - 2);
  TEST_OK("sqlite3BlobCompare: MEM_Zero on both with different nZero -> difference of nZero");
}

// Entry point for running tests
int main() {
  // Run test cases in sequence, catching any unexpected exceptions
  try {
    test_blobCompare_nonZero_equalContent();
    test_blobCompare_nonZero_lexicographic();
    test_blobCompare_lengthDifferenceWithPrefix();
    test_blobCompare_bothZero_flags();
    test_blobCompare_oneZero_and_nonAllZeroOther();
    test_blobCompare_nonZero_vs_zeroAllZeroOther();
    test_blobCompare_mixed_zero_and_nonzero_branch_plus_content();
    test_blobCompare_allZeroBothDifferentNZero();
  } catch (const std::exception& ex) {
    std::cerr << "Test aborted due to exception: " << ex.what() << std::endl;
    g_failures++; // count as failure
  }

  if (g_failures == 0) {
    std::cout << "\nAll tests passed." << std::endl;
    return 0;
  } else {
    std::cerr << "\nTests completed with " << g_failures << " failure(s)." << std::endl;
    return 1;
  }
}