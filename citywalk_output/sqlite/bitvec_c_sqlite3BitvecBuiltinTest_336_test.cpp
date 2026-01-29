// Unit test suite for sqlite3BitvecBuiltinTest (C code under test) using a lightweight C++11 test harness.
// This test avoids Google Test and uses a small custom framework with non-terminating assertions.
// It also provides a minimal in-process stub for the Bitvec-related dependencies so that
// sqlite3BitvecBuiltinTest can be exercised within a single executable.

#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Forward declare the Bitvec type used by sqlite3BitvecBuiltinTest.
// We provide a minimal in-test implementation of the required sqlite3Bitvec* functions
// (Create/Destroy/Set/Clear/Test/Size) to allow the focal function to run.
// The real bitvec implementation is provided by the project; here we supply a lightweight
// in-process substitute that matches the expected interface.
struct Bitvec;            // forward declaration to match external API
typedef struct Bitvec Bitvec;

// Constants
// Do not assume the presence of a particular value for BITVEC_SZ here; the real project defines it.
// We simply implement the required API; the focal function uses BITVEC_SZ in its own compilation unit.
// We rely on the project headers for the macro BITVEC_SZ during linkage.

// Memory allocation wrappers that mirror sqlite3MallocZero, sqlite3_malloc64, and sqlite3_free
extern "C" {
    void* sqlite3MallocZero(std::size_t n);
    void  sqlite3_free(void* p);
    void* sqlite3_malloc64(std::size_t n);
    int   sqlite3_randomness(int n, void* p);
}

// Minimal Bitvec API used by sqlite3BitvecBuiltinTest
extern "C" {
    Bitvec* sqlite3BitvecCreate(uint32_t iSize);
    int     sqlite3BitvecTest(Bitvec *p, uint32_t i);
    int     sqlite3BitvecSet(Bitvec *p, uint32_t i);
    void    sqlite3BitvecClear(Bitvec *p, uint32_t i, void *pBuf);
    void    sqlite3BitvecDestroy(Bitvec *p);
    uint32_t sqlite3BitvecSize(Bitvec *p);
}

// The focal function under test is sqlite3BitvecBuiltinTest(int sz, int *aOp).
// Declare it so we can link against the implementation in bitvec.c
extern "C" int sqlite3BitvecBuiltinTest(int sz, int *aOp);

// Lightweight test harness (non-terminating assertions)
struct TestSuite {
    std::vector<std::string> failures;
    int total{0};

    void expect(bool cond, const std::string& msg) {
        ++total;
        if (!cond) {
            failures.push_back(msg);
            std::cerr << "TEST FAIL: " << msg << std::endl;
        } else {
            // Optional verbose success (commented out to keep output clean)
            // std::cout << "TEST PASS: " << msg << std::endl;
        }
    }

    void summarize() const {
        std::cout << "-------------------- Test Summary --------------------" << std::endl;
        std::cout << "Total checks: " << total << ", Failures: " << failures.size() << std::endl;
        for (const auto& f : failures) {
            std::cout << "  - " << f << std::endl;
        }
        std::cout << "------------------------------------------------------" << std::endl;
    }
};

// Minimal Bitvec implementation (in-test) to back sqlite3BitvecBuiltinTest calls.
// We provide a compatible interface, not the production implementation.
struct Bitvec {
    uint32_t iSize;
    unsigned char *bits;
};

// Simple non-OOP helpers to operate on 1-based bit indexing (consistent with the test's usage).
static inline void bit_set(unsigned char* bits, uint32_t i) {
    if (i == 0) return;
    uint32_t idx = (i - 1) >> 3;
    uint32_t off = (i - 1) & 7;
    bits[idx] |= (unsigned char)(1u << off);
}
static inline void bit_clear(unsigned char* bits, uint32_t i) {
    if (i == 0) return;
    uint32_t idx = (i - 1) >> 3;
    uint32_t off = (i - 1) & 7;
    bits[idx] &= (unsigned char)~(1u << off);
}
static inline int bit_test(const unsigned char* bits, uint32_t i) {
    if (i == 0) return 0;
    uint32_t idx = (i - 1) >> 3;
    uint32_t off = (i - 1) & 7;
    return (bits[idx] >> off) & 1;
}

// Implement the sqlite3* wrappers needed by sqlite3BitvecBuiltinTest
extern "C" {

void* sqlite3MallocZero(std::size_t n) {
    void* p = std::malloc(n);
    if (p) std::memset(p, 0, n);
    return p;
}

void sqlite3_free(void* p) {
    std::free(p);
}

void* sqlite3_malloc64(std::size_t n) {
    return std::malloc(n);
}

int sqlite3_randomness(int n, void* p) {
    // Deterministic but varied bytes for reproducibility
    unsigned char* q = static_cast<unsigned char*>(p);
    for (int i = 0; i < n; ++i) {
        q[i] = static_cast<unsigned char>(i + 1);
    }
    return 0;
}

// Bitvec implementation (tiny, in-test substitute)
Bitvec* sqlite3BitvecCreate(uint32_t iSize) {
    if (iSize == 0) return nullptr;
    Bitvec* pv = static_cast<Bitvec*>(std::malloc(sizeof(Bitvec)));
    if (!pv) return nullptr;
    pv->iSize = iSize;
    size_t bytes = (iSize + 7) / 8;
    pv->bits = static_cast<unsigned char*>(std::malloc(bytes));
    if (!pv->bits) {
        std::free(pv);
        return nullptr;
    }
    std::memset(pv->bits, 0, bytes);
    return pv;
}

int sqlite3BitvecTest(Bitvec *p, uint32_t i) {
    if (p == nullptr) return 0;
    if (i == 0 || i > p->iSize) return 0;
    return bit_test(p->bits, i);
}

int sqlite3BitvecSet(Bitvec *p, uint32_t i) {
    if (p == nullptr) return 0;
    if (i == 0 || i > p->iSize) return 1;
    bit_set(p->bits, i);
    return 0;
}

void sqlite3BitvecClear(Bitvec *p, uint32_t i, void *pBuf) {
    (void)pBuf; // workspace not used in our fake implementation
    if (p == nullptr) return;
    if (i == 0 || i > p->iSize) return;
    bit_clear(p->bits, i);
}

void sqlite3BitvecDestroy(Bitvec *p) {
    if (p) {
        if (p->bits) std::free(p->bits);
        std::free(p);
    }
}

uint32_t sqlite3BitvecSize(Bitvec *p) {
    return p ? p->iSize : 0;
}

} // extern "C"

// A simple test driver for sqlite3BitvecBuiltinTest
int main();
} // extern "C"

// Test driver implementation
int main() {
    TestSuite ts;

    // Helper to invoke the focal function with given operation array
    auto run = [&](const char* label, int sz, const int* aOp) {
        int rc = sqlite3BitvecBuiltinTest(sz, const_cast<int*>(aOp));
        ts.expect(true, label); // dummy to maintain sequence; we will assert rc below
        // We actually want to assert the rc equals 0 for deterministic cases (except the known mismatch case)
        // Recompute the expected rc for specific test cases based on the provided aOp content.
        // Since we cannot return rc here directly for assertion in a lambda easily, we perform a follow-up check:
        // We'll record rc value by re-running and returning; but sqlite3BitvecBuiltinTest has already run.
        // To keep the harness simple, we re-run in a separate function that captures rc value inside a global or static.
        // For simplicity in this harness, we won't attempt to lint rc here; instead, we perform asserts after specific calls
        // using dedicated test wrappers below (each test will call its own function to check rc).
        return rc;
    };

    // Since the lambda above doesn't expose rc cleanly for per-test assertion after each call without duplicating work,
    // we instead implement explicit test sequences with direct calls and rc capture.

    // Test Case 1: Early exit when sz == 0 (pBitvecCreate would be given 0, which should yield null and rc == -1)
    {
        int aOp[] = {0};
        int rc = sqlite3BitvecBuiltinTest(0, aOp);
        ts.expect(rc == -1, "Early exit on sz=0 should return -1");
    }

    // Test Case 2: Basic operation path (op=1) with deterministic sequence
    // aOp layout: {op, count, val_for_i, increment, 0}
    // op=1 -> uses i = aOp[2]-1; then aOp[2] += aOp[3]; count decremented to 0 -> advance
    {
        int aOp[] = {1, 1, 2, 3, 0}; // expected: set bit at i+1 where i = (2-1) = 1 -> bit 2
        int rc = sqlite3BitvecBuiltinTest(5, aOp);
        ts.expect(rc == 0, "Basic op=1 path should result in rc == 0 (Bitvec matches pV)");
    }

    // Test Case 3: op=2 (clear path) with deterministic sequence
    // This should clear the bit at position i+1
    {
        int aOp[] = {2, 1, 2, 3, 0}; // i = 2-1 = 1; after operation, clears bit 2
        int rc = sqlite3BitvecBuiltinTest(5, aOp);
        ts.expect(rc == 0, "Op=2 path should result in rc == 0 when bitvec matches (clear path)");
    }

    // Test Case 4: op=5 (set without touching Bitvec) to hit the 'op==5' branch
    // It should set the linear array bit but not call sqlite3BitvecSet; expects mismatch => rc != 0
    {
        int aOp[] = {5, 1, 2, 3, 0};
        int rc = sqlite3BitvecBuiltinTest(5, aOp);
        ts.expect(rc == 2, "Op=5 path should yield mismatch rc==2 (pV vs Bitvec) due to skipped BitvecSet");
    }

    // Test Case 5: op=3 (randomness path) with deterministic randomness
    // randomness path uses sqlite3_randomness to fill 'i'; should still produce matching results
    {
        int aOp[] = {3, 1, 0, 0, 0};
        int rc = sqlite3BitvecBuiltinTest(5, aOp);
        ts.expect(rc == 0, "Op=3 randomness path should yield rc==0 (Bitvec matches pV)");
    }

    ts.summarize();
    return ts.failures.empty() ? 0 : 1;
}