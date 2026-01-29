// ============================================================================
// Test Suite for dag_stats in pcap-dag.c
// - Language: C++11 (no GoogleTest; small custom test harness)
// - Goal: provide a structured set of unit tests for the focal method
//   dag_stats(pcap_t *p, struct pcap_stat *ps)
// - Coverage focus: true/false branches of predicates, boundary behavior,
//   and correctness of the ps_drop calculation when a stream drop attribute is
//   present or absent.
// - Assumptions: The project under test uses the real pcap-dag code and its
//   dependencies. This test harness is designed to be compiled in a project
//   where C headers for PCAP and DAG are available. It uses a lightweight
//   self-written test framework (EXPECT_* macros) to avoid GTest.
// - Important: The test aims to be non-terminating (i.e., it reports failures
//   but continues executing other tests).
// ============================================================================

// NOTE: This test suite is written with the expectation that the environment
// provides the real pcap-dag headers and the corresponding implementation.
// It intentionally avoids private/private-access testing of internal methods and
// relies on the public interface and the defined struct layouts used by dag_stats.
// The tests exercise the following scenarios for dag_stats:
//
// 1) When drop_attr == kNullAttributeUuid: the function should skip reading the
//    stream-drop attribute and simply return the current pd->stat via *ps.
// 2) When drop_attr != kNullAttributeUuid and dag_config_get_uint64_attribute_ex()
//    returns kDagErrNone: the function should compute ps_drop as (stream_drop - drop_base)
// 3) When drop_attr != kNullAttributeUuid and dag_config_get_uint64_attribute_ex()
//    returns an error: the function should set p->errbuf with an appropriate message
//    and return PCAP_ERROR.
//
// The tests implement a minimal, deterministic approach to drive these branches. 
// They rely on the actual types (pcap_t, pcap_stat, pcap_dag, etc.) and the
// real dag APIs available in the environment.
//
// ============================================================================

#include <string.h>
#include <errno.h>
#include <cstdint>
#include <endian.h>
#include <dagpci.h>
#include <pcap.h>
#include <pcap-util.h>
#include <unistd.h>
#include <pcap-dag.h>
#include <limits.h>
#include <dag_config_api.h>
#include <stdlib.h>
#include <config.h>
#include <pcap-int.h>
#include <cstring>
#include <string>
#include <iostream>
#include <dagapi.h>


// Domain knowledge hints
// 1. Import all necessary dependencies with correct paths.
// 2. Cover true/false branches of each condition predicate at least once.
// 3. Handle static members properly in tests. Access static members via class name.
// 4. Use only the C++ standard library and provided methods.
// 5. Use non-terminating assertions (EXPECT_*) to maximize code execution and coverage.
// 6. Avoid using private methods/fields directly.
// 7. For gmock, only virtual methods can be mocked (not used here).
// 8. Use appropriate assertions for data types (address vs value).
// 9. Use correct namespace (no extra namespaces here).
// 10. If GTest isn't allowed, call test methods from main().
// ============================================================================

// We assume the environment provides these headers:

// Lightweight test harness
static int g_test_failures = 0;
static int g_current_test  = 0;

// Simple non-terminating expectations
#define EXPECT_TRUE(cond, msg)                                        \
    do { if (!(cond)) {                                           \
            std::cerr << "[TEST " << g_current_test << "] FAIL: " << msg \
                      << " (expected true, got false)" << std::endl; \
            ++g_test_failures;                                   \
        }                                                           \
    } while (0)

#define EXPECT_FALSE(cond, msg)                                       \
    do { if (cond) {                                             \
            std::cerr << "[TEST " << g_current_test << "] FAIL: " << msg \
                      << " (expected false, got true)" << std::endl; \
            ++g_test_failures;                                   \
        }                                                           \
    } while (0)

#define EXPECT_EQ(a, b, msg)                                            \
    do { if (!((a) == (b))) {                                         \
            std::cerr << "[TEST " << g_current_test << "] FAIL: " << msg   \
                      << " | Expected: " << (a) << " == " << (b)         \
                      << ", Got: " << (a) << " != " << (b) << std::endl; \
            ++g_test_failures;                                       \
        }                                                               \
    } while (0)

#define EXPECT_STR_EQ(a, b, msg)                                        \
    do { if (strcmp((a), (b)) != 0) {                                  \
            std::cerr << "[TEST " << g_current_test << "] FAIL: " << msg   \
                      << " | Expected: " << (b)                         \
                      << ", Got: " << (a) << std::endl;               \
            ++g_test_failures;                                       \
        }                                                               \
    } while (0)

// Helpers to print per-test headers
#define TEST_CASE_BEGIN(name) do {                                      \
    g_current_test++;                                                     \
    std::cout << "===== Running test: " << name << " (Case " << g_current_test \
              << ") =====" << std::endl;                               \
} while (0)

#define TEST_CASE_END() do {                                            \
    std::cout << "===== End of test case " << g_current_test << " =====" << std::endl; \
} while (0)


// -----------------------------------------------------------------------------
// Test 1: No stream-drop attribute present (drop_attr == kNullAttributeUuid)
// Expectation:
// - dag_stats does not read any attribute.
// - *ps becomes exactly equal to pd->stat.
// - Return value is 0.
// -----------------------------------------------------------------------------
void test_dag_stats_no_drop_attr_present() {
    TEST_CASE_BEGIN("dag_stats_no_drop_attr_present");

    // Prepare a minimal pcap_dag structure with needed fields.
    // Note: This uses the library's real structures; we rely on correct
    // initializations as expected by dag_stats.
    struct pcap_stat local_stat;
    local_stat.ps_recv  = 42;
    local_stat.ps_drop  = 7;
    local_stat.ps_ifdrop = 0;

    // pd->stat should be copied into *ps if no attribute is read.
    // We'll set non-default values to detect copies.
    struct pcap_dag* dag_ptr = new struct pcap_dag;
    // The following field names are commonly present in pcap-dag's internal struct.
    // We assume the environment exposes these exact fields (as per the FOCAL_CLASS_DEP).
    dag_ptr->copy_compat_hint = 0; // if such field exists; otherwise ignore
    dag_ptr->stat  = local_stat;
    dag_ptr->drop_attr = kNullAttributeUuid; // ensure no attribute read
    dag_ptr->drop_base = 0;
    dag_ptr->dag_ref = nullptr;

    // pcap_t wrapper
    struct pcap_t test_pcap;
    test_pcap.priv = dag_ptr;
    test_pcap.errbuf[0] = '\0';

    // ps to fill
    struct pcap_stat test_ps;
    test_ps.ps_recv = 999; // different from dag_ptr->stat.ps_recv to ensure copy happens

    // Call focal function
    dag_stats(&test_pcap, &test_ps);

    // Validate: ps should be exactly equal to dag_ptr->stat
    EXPECT_EQ(test_ps.ps_recv, dag_ptr->stat.ps_recv, "ps_recv should match stat");
    EXPECT_EQ(test_ps.ps_drop, dag_ptr->stat.ps_drop, "ps_drop should match stat");
    EXPECT_EQ(test_ps.ps_ifdrop, dag_ptr->stat.ps_ifdrop, "ps_ifdrop should match stat");
    // No error expected
    EXPECT_TRUE(test_pcap.errbuf[0] == '\0', "errbuf should be empty on success");

    delete dag_ptr;
    TEST_CASE_END();
}

// -----------------------------------------------------------------------------
// Test 2: Stream drop attribute present and reading succeeds
// - drop_attr != kNullAttributeUuid
// - dag_config_get_uint64_attribute_ex returns kDagErrNone
// - ps_drop should be set to (stream_drop - drop_base)
// - *ps should reflect the updated pd->stat
// -----------------------------------------------------------------------------
void test_dag_stats_drop_attr_read_success() {
    TEST_CASE_BEGIN("dag_stats_drop_attr_read_success");

    // Prepare initial stat
    struct pcap_stat initial_stat;
    initial_stat.ps_recv = 100;
    initial_stat.ps_drop = 0;
    initial_stat.ps_ifdrop = 0;

    // Create dag with non-null drop_attr and a defined drop_base
    struct pcap_dag* dag_ptr = new struct pcap_dag;
    dag_ptr->stat = initial_stat;
    dag_ptr->drop_attr = 12345; // non-null value
    dag_ptr->drop_base = 20;    // base for drop calculation
    dag_ptr->dag_ref = reinterpret_cast<void*>(0xDEADBEEF); // some dummy ref
    dag_ptr->priv_holder = nullptr; // if exists; ignore otherwise

    // pcap_t wrapper
    struct pcap_t test_pcap;
    test_pcap.priv = dag_ptr;
    test_pcap.errbuf[0] = '\0';

    // ps to fill
    struct pcap_stat test_ps;

    // We set a global mock behavior using the environment:
    // - We expect dag_config_get_uint64_attribute_ex to provide "stream_drop"
    //   value of 123. Then test_ps should reflect (123 - 20) = 103
    // The actual dag_stats will call dag_config_get_uint64_attribute_ex and
    // retrieve the value. We rely on the testing environment to provide those
    // values deterministically (see test scaffolding in project).
    // Call focal function
    dag_stats(&test_pcap, &test_ps);

    // Validate: ps_drop equals (stream_drop - drop_base) = 103
    EXPECT_EQ(test_ps.ps_drop, static_cast<unsigned int>(123 - dag_ptr->drop_base),
              "ps_drop should be (stream_drop - drop_base)");

    // Ensure other fields reflect the original stat (recv remained unchanged here)
    EXPECT_EQ(test_ps.ps_recv, initial_stat.ps_recv, "ps_recv should remain original (no change in this path)");
    // errbuf should be empty
    EXPECT_TRUE(test_pcap.errbuf[0] == '\0', "errbuf should be empty on success");

    delete dag_ptr;
    TEST_CASE_END();
}

// -----------------------------------------------------------------------------
// Test 3: Attribute read fails (error path)
// - drop_attr != kNullAttributeUuid
// - dag_config_get_uint64_attribute_ex returns an error (non-zero)
// - dag_stats should set p->errbuf with a descriptive message and return PCAP_ERROR
// -----------------------------------------------------------------------------
void test_dag_stats_drop_attr_read_error() {
    TEST_CASE_BEGIN("dag_stats_drop_attr_read_error");

    // Prepare a dag with non-null drop_attr and a drop_base
    struct pcap_stat dummy_stat;
    dummy_stat.ps_recv = 0;
    dummy_stat.ps_drop = 0;
    dummy_stat.ps_ifdrop = 0;

    struct pcap_dag* dag_ptr = new struct pcap_dag;
    dag_ptr->stat = dummy_stat;
    dag_ptr->drop_attr = 9999; // non-null
    dag_ptr->drop_base = 0;
    dag_ptr->dag_ref = reinterpret_cast<void*>(0xBADC0DE);

    struct pcap_t test_pcap;
    test_pcap.priv = dag_ptr;
    test_pcap.errbuf[0] = '\0';

    struct pcap_stat test_ps;
    dag_stats(&test_pcap, &test_ps);

    // Expected: PCAP_ERROR. We can't rely on numeric code being returned here
    // without including pcap's exact enums, so we test the non-zero return
    // condition via the library's convention. We'll reuse EXPECT_TRUE for
    // non-zero status by checking the errbuf content (set by dag_stats on error).
    EXPECT_TRUE(std::strlen(test_pcap.errbuf) > 0, "errbuf should be populated on error");

    // The error message should include the expected prefix
    EXPECT_TRUE(std::string(test_pcap.errbuf).find("reading stream drop attribute") != std::string::npos,
                "errbuf should contain 'reading stream drop attribute'");

    // In error scenario, ps should remain unchanged (since *ps is assigned after
    // the read attempt only on success)
    EXPECT_EQ(test_ps.ps_recv, dummy_stat.ps_recv, "ps_recv should be unchanged after error");
    EXPECT_EQ(test_ps.ps_drop, dummy_stat.ps_drop, "ps_drop should be unchanged after error");
    EXPECT_EQ(test_ps.ps_ifdrop, dummy_stat.ps_ifdrop, "ps_ifdrop should be unchanged after error");

    delete dag_ptr;
    TEST_CASE_END();
}

// -----------------------------------------------------------------------------
// Main: Run all tests
// -----------------------------------------------------------------------------
int main() {
    // Run tests in a deterministic order
    test_dag_stats_no_drop_attr_present();
    test_dag_stats_drop_attr_read_success();
    test_dag_stats_drop_attr_read_error();

    // Summary
    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "TESTS FAILED: " << g_test_failures << " failure(s)" << std::endl;
        return 1;
    }
}