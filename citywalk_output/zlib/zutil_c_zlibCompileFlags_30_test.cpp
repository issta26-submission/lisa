// Lightweight C++11 unit test suite for the focal method zlibCompileFlags (zutil.c)
// This test suite avoids Google Test and uses a small, non-terminating assertion framework.
// It exercises the behavior of zlibCompileFlags via direct invocation and by re-implementing
// the same logic to cross-validate results. It also inspects bit-fields corresponding to
// type sizes and compile-time macros to maximize coverage.

#include <iostream>
#include <string>
#include <stdlib.h>
#include <zutil.h>


extern "C" uLong zlibCompileFlags(void);

static int g_tests = 0;
static int g_failures = 0;

// Non-terminating assertion: logs failure but continues running tests
#define EXPECT_TRUE(cond) do { \
    ++g_tests; \
    if(!(cond)) { \
        std::cerr << "[FAIL] " << __func__ << " line " << __LINE__ << ": " #cond << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ_UL(a, b) do { \
    ++g_tests; \
    unsigned long long _a = static_cast<unsigned long long>(a); \
    unsigned long long _b = static_cast<unsigned long long>(b); \
    if(_a != _b) { \
        std::cerr << "[FAIL] " << __func__ << " line " << __LINE__ << ": " #a " (" << _a << ") != " #b " (" << _b << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

/*
Test 1: Basic equivalence between zlibCompileFlags and a manual reimplementation.
- Rationale: Ensure the function computes the same flags as the explicit logic we replicate,
  including the handling of type-size switches and compile-time macros.
- Approach: Call zlibCompileFlags() and compare with computeFlagsManual() (a close reimplementation).
*/
static unsigned long computeFlagsManual(void)
{
    unsigned long flags = 0;
    // uInt size branch
    switch ((int)(sizeof(uInt))) {
        case 2:     break;
        case 4:     flags += 1;     break;
        case 8:     flags += 2;     break;
        default:    flags += 3;
    }
    // uLong size branch
    switch ((int)(sizeof(uLong))) {
        case 2:     break;
        case 4:     flags += 1 << 2;        break;
        case 8:     flags += 2 << 2;        break;
        default:    flags += 3 << 2;
    }
    // voidpf size branch
    switch ((int)(sizeof(voidpf))) {
        case 2:     break;
        case 4:     flags += 1 << 4;        break;
        case 8:     flags += 2 << 4;        break;
        default:    flags += 3 << 4;
    }
    // z_off_t size branch
    switch ((int)(sizeof(z_off_t))) {
        case 2:     break;
        case 4:     flags += 1 << 6;        break;
        case 8:     flags += 2 << 6;        break;
        default:    flags += 3 << 6;
    }
#ifdef ZLIB_DEBUG
    flags += 1 << 8;
#endif
#ifdef ZLIB_WINAPI
    flags += 1 << 10;
#endif
#ifdef BUILDFIXED
    flags += 1 << 12;
#endif
#ifdef DYNAMIC_CRC_TABLE
    flags += 1 << 13;
#endif
#ifdef NO_GZCOMPRESS
    flags += 1L << 16;
#endif
#ifdef NO_GZIP
    flags += 1L << 17;
#endif
#ifdef PKZIP_BUG_WORKAROUND
    flags += 1L << 20;
#endif
#ifdef FASTEST
    flags += 1L << 21;
#endif
#if defined(STDC) || defined(Z_HAVE_STDARG_H)
#  ifdef NO_vsnprintf
    flags += 1L << 25;
#    ifdef HAS_vsprintf_void
    flags += 1L << 26;
#    endif
#  else
#    ifdef HAS_vsnprintf_void
    flags += 1L << 26;
#    endif
#  endif
#else
    flags += 1L << 24;
#  ifdef NO_snprintf
    flags += 1L << 25;
#    ifdef HAS_sprintf_void
    flags += 1L << 26;
#    endif
#  else
#    ifdef HAS_snprintf_void
    flags += 1L << 26;
#    endif
#  endif
#endif
    return flags;
}

static void test_basic_equivalence() {
    // Test 1: Basic equivalence with manual reimplementation
    unsigned long actual = zlibCompileFlags();
    unsigned long expected = computeFlagsManual();
    EXPECT_EQ_UL(actual, expected);
    // Also verify that a direct recomputation matches across two calls (consistency check)
    unsigned long actual2 = zlibCompileFlags();
    EXPECT_EQ_UL(actual2, actual);
}

/*
Test 2: Bit-field integrity according to type sizes.
- Rationale: Ensure that the lower 8 bits corresponding to sizeof(uInt), sizeof(uLong),
  sizeof(voidpf), and sizeof(z_off_t) are encoded as expected.
- Approach: Compare individual masked portions of the flags against independent computations
  derived from the same sizeof-based switch logic.
*/
static void test_bit_field_by_size() {
    unsigned long f = zlibCompileFlags();

    // Lower 2 bits: sizeof(uInt)
    int exp1;
    switch ((int)(sizeof(uInt))) {
        case 2: exp1 = 0; break;
        case 4: exp1 = 1; break;
        case 8: exp1 = 2; break;
        default: exp1 = 3;
    }
    EXPECT_TRUE((int)(f & 0x3) == exp1);

    // Bits 2-3: sizeof(uLong)
    int exp2;
    switch ((int)(sizeof(uLong))) {
        case 2: exp2 = 0; break;
        case 4: exp2 = 4; break;   // 1 << 2
        case 8: exp2 = 8; break;   // 2 << 2
        default: exp2 = 12; break; // 3 << 2
    }
    EXPECT_TRUE((int)(f & 0x0C) == exp2);

    // Bits 4-5: sizeof(voidpf)
    int exp3;
    switch ((int)(sizeof(voidpf))) {
        case 2: exp3 = 0; break;
        case 4: exp3 = 16; break;  // 1 << 4
        case 8: exp3 = 32; break;  // 2 << 4
        default: exp3 = 48; break; // 3 << 4
    }
    EXPECT_TRUE((int)(f & 0x30) == exp3);

    // Bits 6-7: sizeof(z_off_t)
    int exp4;
    switch ((int)(sizeof(z_off_t))) {
        case 2: exp4 = 0; break;
        case 4: exp4 = 64; break;  // 1 << 6
        case 8: exp4 = 128; break; // 2 << 6
        default: exp4 = 192; break; // 3 << 6
    }
    EXPECT_TRUE((int)(f & 0xC0) == exp4);
}

/*
Test 3: Compile-time macro flag presence influences.
- Rationale: Validate that optional compile-time macros contribute their designated bits.
- Approach: Use preprocessor checks to assert the corresponding bit is set/unset according to macro definitions.
*/
static void test_macro_bits_presence() {
    unsigned long f = zlibCompileFlags();

    // ZLIB_DEBUG -> bit 8
#ifdef ZLIB_DEBUG
    EXPECT_TRUE((f & (1u << 8)) != 0);
#else
    EXPECT_TRUE((f & (1u << 8)) == 0);
#endif

    // ZLIB_WINAPI -> bit 10
#ifdef ZLIB_WINAPI
    EXPECT_TRUE((f & (1u << 10)) != 0);
#else
    EXPECT_TRUE((f & (1u << 10)) == 0);
#endif

    // BUILDFIXED -> bit 12
#ifdef BUILDFIXED
    EXPECT_TRUE((f & (1u << 12)) != 0);
#else
    EXPECT_TRUE((f & (1u << 12)) == 0);
#endif

    // DYNAMIC_CRC_TABLE -> bit 13
#ifdef DYNAMIC_CRC_TABLE
    EXPECT_TRUE((f & (1u << 13)) != 0);
#else
    EXPECT_TRUE((f & (1u << 13)) == 0);
#endif

    // NO_GZCOMPRESS -> bit 16
#ifdef NO_GZCOMPRESS
    EXPECT_TRUE((f & (1ull << 16)) != 0);
#else
    EXPECT_TRUE((f & (1ull << 16)) == 0);
#endif

    // NO_GZIP -> bit 17
#ifdef NO_GZIP
    EXPECT_TRUE((f & (1ull << 17)) != 0);
#else
    EXPECT_TRUE((f & (1ull << 17)) == 0);
#endif

    // PKZIP_BUG_WORKAROUND -> bit 20
#ifdef PKZIP_BUG_WORKAROUND
    EXPECT_TRUE((f & (1ull << 20)) != 0);
#else
    EXPECT_TRUE((f & (1ull << 20)) == 0);
#endif

    // FASTEST -> bit 21
#ifdef FASTEST
    EXPECT_TRUE((f & (1ull << 21)) != 0);
#else
    EXPECT_TRUE((f & (1ull << 21)) == 0);
#endif
}

/*
Test 4: Stdarg-related branch coverage.
- Rationale: Exercise the conditional blocks that depend on STDC/Z_HAVE_STDARG_H and availability
  of vsnprintf-related functions. This helps ensure the test suite covers both possible code paths.
- Approach: Use preprocessor directives to verify the presence or absence of bits 25 and 26
  matching NO_vsnprintf/HAS_vsnprintf_void or HAS_sprintf_void/HAS_snprintf_void as applicable.
*/
static void test_stdarg_branch_coverage() {
    unsigned long f = zlibCompileFlags();

#if defined(STDC) || defined(Z_HAVE_STDARG_H)
    // Path where the compiler supports stdarg.h
#  ifdef NO_vsnprintf
    EXPECT_TRUE((f & (1ull << 25)) != 0);
#    ifdef HAS_vsprintf_void
    EXPECT_TRUE((f & (1ull << 26)) != 0);
#    else
    EXPECT_TRUE((f & (1ull << 26)) == 0);
#    endif
#  else
#    ifdef HAS_vsnprintf_void
    EXPECT_TRUE((f & (1ull << 26)) != 0);
#    else
    EXPECT_TRUE((f & (1ull << 26)) == 0);
#    endif
#  endif
#else
    // Path where the compiler does not provide stdarg.h pathway
    EXPECT_TRUE((f & (1ull << 24)) != 0);
#  ifdef NO_snprintf
    EXPECT_TRUE((f & (1ull << 25)) != 0);
#    ifdef HAS_sprintf_void
    EXPECT_TRUE((f & (1ull << 26)) != 0);
#    else
    EXPECT_TRUE((f & (1ull << 26)) == 0);
#    endif
#  else
#    ifdef HAS_snprintf_void
    EXPECT_TRUE((f & (1ull << 26)) != 0);
#    else
    EXPECT_TRUE((f & (1ull << 26)) == 0);
#    endif
#  endif
#endif
}

/*
Test 5: Public API consistency across multiple invocations (sanity).
- Rationale: Ensure no hidden state or side effects alter the return value on repeated calls.
- Approach: Call zlibCompileFlags() multiple times and compare for consistency.
*/
static void test_consistency_over_calls() {
    unsigned long a = zlibCompileFlags();
    unsigned long b = zlibCompileFlags();
    EXPECT_EQ_UL(a, b);
}

int main() {
    // Run all tests and summarize results.
    test_basic_equivalence();
    test_bit_field_by_size();
    test_macro_bits_presence();
    test_stdarg_branch_coverage();
    test_consistency_over_calls();

    // Some additional cross-checks
    test_consistency_over_calls();

    std::cout << "Total tests run: " << g_tests << ", Failures: " << g_failures << std::endl;
    return (g_failures == 0) ? 0 : 1;
}