// C++11 test suite for the focal function dag_activate_tx in pcap-dag.c
// This test harness uses a lightweight, in-process mock environment to exercise
// multiple branches of dag_activate_tx without relying on GTest. It relies on
// static function stubs to emulate the external dependencies used by the focal
// method. The goal is to maximize coverage by hitting true/false predicates in
// the control flow, including error paths and successful paths.
//
// Notes:
// - The test does not require the full pcap/dag runtime to be present; it mocks
//   the necessary pieces and exercises dag_activate_tx via a C linkage declaration.
// - The test harness uses simple EXPECT_* macros (non-terminating) to allow all
//   tests to run even if some fail, collecting a summary at the end.
// - The test suite covers: invalid env parsing, interface count bounds, null PCI
//   info, unsupported vDAG, alignment choice, TERF CRC parsing, and TERF strip
//   configurations.

// Do not use GTest. Implement a lightweight test harness with main().

#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <cassert>
#include <climits>
#include <config.h>
#include <pcap-util.h>
#include <pcap-dag.h>
#include <cstdio>
#include <dag_config_api.h>
#include <cstdint>
#include <dagapi.h>
#include <endian.h>
#include <dagpci.h>
#include <limits.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <string.h>
#include <cstdlib>
#include <cstring>


// -------------------- Domain knowledge and dependencies (test doubles) --------------------

#ifndef PCAP_ERRBUF_SIZE
#define PCAP_ERRBUF_SIZE 256
#endif

#ifndef ENV_TX_IFACE
#define ENV_TX_IFACE "DAG_TX_IFACE"
#endif

// Forward declare the focal function (C linkage)
extern "C" int dag_activate_tx(struct pcap_t *p);

// Minimal definitions to allow building the test harness alongside the focal code.
// These mirror the subset used by dag_activate_tx in the test environment.
typedef struct dag_card_inf_t {
  int device_code;
} dag_card_inf_t;

// Macros/constants used by the focal function
#define PCI_DEVICE_ID_VDAG 0x01
#define PCI_DEVICE_ID_DAG9_2X2 0x02
#define PCI_DEVICE_ID_DAG9_2SX2 0x03

#define ALIGN_BYTES_9_2 16
#define ALIGN_BYTES_DEFAULT 8

#define kTerfNoStrip 0
#define kTerfStrip16 1
#define kTerfStrip32 2
#define kNullAttributeUuid NULL

#define kComponentTerf 0

typedef void* attr_uuid_t;
typedef int dag_err_t;
#define kDagErrNone 0

// Lightweight pcap_t and pcap_dag compatible with the focal function's usage.
// We only expose the fields accessed by dag_activate_tx.
struct pcap_dag {
  void *dag_ref;
  uint8_t tx_iface;
  uint8_t tx_align_bytes;
  uint32_t terf_fcs_bytes;
  void *dag_root;
};

struct pcap_t {
  void *priv;
  char errbuf[PCAP_ERRBUF_SIZE];
  int fd;
};

// Function stubs (mocks) for external dependencies used by dag_activate_tx.
// They are designed to be controlled by test cases to drive the desired branches.

static int test_ifcount = 4;
static int test_device_code = PCI_DEVICE_ID_DAG9_2X2;
static dag_card_inf_t fake_inf;
static int dag_inf_is_null = 0;

// TERF FCS configuration (read via dag_config_get_uint32_attribute_ex)
static uint32_t test_cfg_uint32_value = kTerfNoStrip;
static int cfg_err_should_fail = 0;
static void *test_dag_ref_ptr = (void*)0xDEADBEEF;
static void *test_dag_root_ptr = (void*)0xFEEDBEEF;
static int cfg_comp_null = 0;
static attr_uuid_t cfg_uuid = (attr_uuid_t)0x1234;

// Helpers: reset test doubles before each test
static void reset_mocks() {
  test_ifcount = 4;
  test_device_code = PCI_DEVICE_ID_DAG9_2X2;
  dag_inf_is_null = 0;
  test_cfg_uint32_value = kTerfNoStrip;
  cfg_err_should_fail = 0;
  cfg_comp_null = 0;
  cfg_uuid = (attr_uuid_t)0x1234;
  test_dag_ref_ptr = (void*)0xDEAD0001;
  test_dag_root_ptr = (void*)0xDEAD0002;
  // ensure no stale env
  unsetenv(ENV_TX_IFACE);
}

// Stubs for the external API used by dag_activate_tx

extern "C" {

// PCI info retrieval stub
const dag_card_inf_t* dag_pciinfo(int fd) {
  (void)fd; // unused in test harness
  if (dag_inf_is_null) {
    return NULL;
  }
  fake_inf.device_code = test_device_code;
  return &fake_inf;
}

// Interface count for the dag
uint32_t dag_config_get_interface_count(void *dag_ref) {
  (void)dag_ref;
  (void)pthread_self; // suppress unused in some toolchains (no-op)
  return (uint32_t)test_ifcount;
}

// Subcomponent getter: return non-null to simulate TERF config presence
void* dag_component_get_subcomponent(void *root, int component, int zero) {
  (void)root; (void)component; (void)zero;
  if (cfg_comp_null) return NULL;
  return (void*)0x1; // non-null sentinel
}

// Config attribute UUID extractor
attr_uuid_t dag_component_get_config_attribute_uuid(void *cfg_comp, int attribute) {
  (void)cfg_comp; (void)attribute;
  if (cfg_comp_null) return kNullAttributeUuid;
  // Return a non-null dummy UUID
  return cfg_uuid;
}

// Read a 32-bit Terf FCS configuration attribute
dag_err_t dag_config_get_uint32_attribute_ex(void *dag_ref, attr_uuid_t uuid, uint32_t *out) {
  (void)dag_ref; (void)uuid;
  if (cfg_err_should_fail) return -1;
  *out = test_cfg_uint32_value;
  return kDagErrNone;
}

// errno-based error message helper (simplified for tests)
void pcapint_fmt_errmsg_for_errno(char *errbuf, size_t len, int err, const char *func) {
  (void)func;
  if (len == 0) return;
  // simple formatting to capture the errno and function name
  snprintf(errbuf, len, "%s: errno %d", func, err);
}

// String-to-integer parser used by the focal function
int32_t strtouint31(const char *str, const int32_t defaultval) {
  if (!str) return defaultval;
  char *endptr = nullptr;
  long val = strtol(str, &endptr, 10);
  if (endptr == str) return defaultval;
  if (val < INT_MIN) val = INT_MIN;
  if (val > INT_MAX) val = INT_MAX;
  return (int32_t)val;
}

} // extern "C"

// -------------------- Lightweight test framework (non-terminating) --------------------

static int g_tests_run = 0;
static int g_tests_pass = 0;
static int g_tests_fail = 0;

#define EXPECT_TRUE(cond) do { ++g_tests_run; if (cond) { ++g_tests_pass; } else { ++g_tests_fail; std::cerr << "Test " << g_tests_run << " failed: expected true.\\n"; } } while(0)
#define EXPECT_FALSE(cond) do { ++g_tests_run; if (!(cond)) { ++g_tests_pass; } else { ++g_tests_fail; std::cerr << "Test " << g_tests_run << " failed: expected false.\\n"; } } while(0)
#define EXPECT_EQ(a,b) do { ++g_tests_run; if ((a) == (b)) { ++g_tests_pass; } else { ++g_tests_fail; std::cerr << "Test " << g_tests_run << " failed: " #a " != " #b " ( " << (a) << " != " << (b) << " )\\n"; } } while(0)
#define EXPECT_STRSTR(haystack, needle) do { ++g_tests_run; if (haystack && needle && std::string(haystack).find(needle) != std::string::npos) { ++g_tests_pass; } else { ++g_tests_fail; std::cerr << "Test " << g_tests_run << " failed: substring \"" << (needle? needle : "(null)") << "\" not found in errbuf.\\n"; } } while(0)

static void print_summary() {
  std::cout << "Tests run: " << g_tests_run << ", Passed: " << g_tests_pass << ", Failed: " << g_tests_fail << std::endl;
}

// -------------------- Helper: build and run a single test case --------------------

static void do_test_case_negative_iface() {
  reset_mocks();
  // True branch: iface < 0 triggers error
  setenv(ENV_TX_IFACE, "-1", 1);
  test_ifcount = 4;
  test_device_code = PCI_DEVICE_ID_DAG9_2X2;
  dag_inf_is_null = 0;

  pcap_dag pd = {};
  pd.dag_ref = test_dag_ref_ptr;
  pd.dag_root = test_dag_root_ptr;
  pd.tx_iface = 0;
  pd.tx_align_bytes = 0;
  pd.terf_fcs_bytes = 0;

  pcap_t p = {};
  p.priv = &pd;
  p.fd = 1;

  // Call focal function
  int ret = dag_activate_tx(&p);

  // Assertions
  EXPECT_EQ(ret, -1); // PCAP_ERROR typically equals -1
  EXPECT_STRSTR(p.errbuf, "failed parsing"); // error message should indicate parsing
}

static void do_test_case_iface_out_of_range() {
  reset_mocks();
  // True branch: iface >= ifcount triggers error
  setenv(ENV_TX_IFACE, "2", 1);
  test_ifcount = 1; // card has only 1 interface
  test_device_code = PCI_DEVICE_ID_DAG9_2X2;
  dag_inf_is_null = 0;

  pcap_dag pd = {};
  pd.dag_ref = test_dag_ref_ptr;
  pd.dag_root = test_dag_root_ptr;
  pd.tx_iface = 0;
  pd.tx_align_bytes = 0;
  pd.terf_fcs_bytes = 0;

  pcap_t p = {};
  p.priv = &pd;
  p.fd = 2;

  int ret = dag_activate_tx(&p);

  EXPECT_EQ(ret, -1);
  EXPECT_STRSTR(p.errbuf, "invalid");
}

static void do_test_case_inf_null() {
  reset_mocks();
  setenv(ENV_TX_IFACE, "1", 1);
  test_ifcount = 4;
  dag_inf_is_null = 1; // dag_pciinfo returns NULL

  pcap_dag pd = {};
  pd.dag_ref = test_dag_ref_ptr;
  pd.dag_root = test_dag_root_ptr;
  pd.tx_iface = 0;
  pd.tx_align_bytes = 0;
  pd.terf_fcs_bytes = 0;

  pcap_t p = {};
  p.priv = &pd;
  p.fd = 3;

  int ret = dag_activate_tx(&p);

  EXPECT_EQ(ret, -1);
  EXPECT_STRSTR(p.errbuf, "dag_pciinfo");
}

static void do_test_case_vdag_not_supported() {
  reset_mocks();
  setenv(ENV_TX_IFACE, "1", 1);
  test_ifcount = 4;
  test_device_code = PCI_DEVICE_ID_VDAG; // vDAG is not supported for TX
  dag_inf_is_null = 0;

  pcap_dag pd = {};
  pd.dag_ref = test_dag_ref_ptr;
  pd.dag_root = test_dag_root_ptr;
  pd.tx_iface = 0;
  pd.tx_align_bytes = 0;
  pd.terf_fcs_bytes = 0;

  pcap_t p = {};
  p.priv = &pd;
  p.fd = 4;

  int ret = dag_activate_tx(&p);

  EXPECT_EQ(ret, -1);
  EXPECT_STRSTR(p.errbuf, "vDAG");
}

static void do_test_case_alignment_9_2_selected() {
  reset_mocks();
  setenv(ENV_TX_IFACE, "1", 1);
  test_ifcount = 4;
  test_device_code = PCI_DEVICE_ID_DAG9_2X2; // should trigger 9_2 alignment
  dag_inf_is_null = 0;

  pcap_dag pd = {};
  pd.dag_ref = test_dag_ref_ptr;
  pd.dag_root = test_dag_root_ptr;
  pd.tx_iface = 0;
  pd.tx_align_bytes = 0;
  pd.terf_fcs_bytes = 0;

  pcap_t p = {};
  p.priv = &pd;
  p.fd = 5;

  int ret = dag_activate_tx(&p);

  // Should succeed
  EXPECT_EQ(ret, 0);
  // Verify alignment chosen
  EXPECT_EQ(pd.tx_align_bytes, ALIGN_BYTES_9_2);
}

static void do_test_case_cfg_comp_null() {
  reset_mocks();
  setenv(ENV_TX_IFACE, "1", 1);
  test_ifcount = 4;
  test_device_code = PCI_DEVICE_ID_DAG9_2X2;
  dag_inf_is_null = 0;
  cfg_comp_null = 1; // component subcomponent becomes NULL

  pcap_dag pd = {};
  pd.dag_ref = test_dag_ref_ptr;
  pd.dag_root = test_dag_root_ptr;
  pd.tx_iface = 0;
  pd.tx_align_bytes = 0;
  pd.terf_fcs_bytes = 0;

  pcap_t p = {};
  p.priv = &pd;
  p.fd = 6;

  int ret = dag_activate_tx(&p);

  EXPECT_EQ(ret, -1);
  EXPECT_STRSTR(p.errbuf, "dag_component_get_subcomponent");
}

static void do_test_case_cfg_uuid_null() {
  reset_mocks();
  setenv(ENV_TX_IFACE, "1", 1);
  test_ifcount = 4;
  test_device_code = PCI_DEVICE_ID_DAG9_2X2;
  dag_inf_is_null = 0;
  cfg_comp_null = 0;
  cfg_uuid = NULL; // simulate kNullAttributeUuid

  pcap_dag pd = {};
  pd.dag_ref = test_dag_ref_ptr;
  pd.dag_root = test_dag_root_ptr;
  pd.tx_iface = 0;
  pd.tx_align_bytes = 0;
  pd.terf_fcs_bytes = 0;

  pcap_t p = {};
  p.priv = &pd;
  p.fd = 7;

  int ret = dag_activate_tx(&p);

  EXPECT_EQ(ret, -1);
  EXPECT_STRSTR(p.errbuf, "dag_component_get_config_attribute_uuid");
}

static void do_test_case_cfg_read_error() {
  reset_mocks();
  setenv(ENV_TX_IFACE, "1", 1);
  test_ifcount = 4;
  test_device_code = PCI_DEVICE_ID_DAG9_2X2;
  dag_inf_is_null = 0;
  cfg_err_should_fail = 1; // simulate failure reading attribute
  test_cfg_uint32_value = kTerfNoStrip; // value doesn't matter since it should fail

  pcap_dag pd = {};
  pd.dag_ref = test_dag_ref_ptr;
  pd.dag_root = test_dag_root_ptr;
  pd.tx_iface = 0;
  pd.tx_align_bytes = 0;
  pd.terf_fcs_bytes = 0;

  pcap_t p = {};
  p.priv = &pd;
  p.fd = 8;

  int ret = dag_activate_tx(&p);

  EXPECT_EQ(ret, -1);
  EXPECT_STRSTR(p.errbuf, "dag_config_get_uint32_attribute_ex");
}

static void do_test_case_terf_no_strip() {
  reset_mocks();
  setenv(ENV_TX_IFACE, "1", 1);
  test_ifcount = 4;
  test_device_code = PCI_DEVICE_ID_DAG9_2X2;
  dag_inf_is_null = 0;
  cfg_err_should_fail = 0;
  test_cfg_uint32_value = kTerfNoStrip; // no TERF stripping

  pcap_dag pd = {};
  pd.dag_ref = test_dag_ref_ptr;
  pd.dag_root = test_dag_root_ptr;
  pd.tx_iface = 0;
  pd.tx_align_bytes = 0;
  pd.terf_fcs_bytes = 0;

  pcap_t p = {};
  p.priv = &pd;
  p.fd = 9;

  int ret = dag_activate_tx(&p);

  EXPECT_EQ(ret, 0);
  EXPECT_EQ(pd.terf_fcs_bytes, 0);
}

static void do_test_case_terf_strip32() {
  reset_mocks();
  setenv(ENV_TX_IFACE, "1", 1);
  test_ifcount = 4;
  test_device_code = PCI_DEVICE_ID_DAG9_2X2;
  dag_inf_is_null = 0;
  cfg_err_should_fail = 0;
  test_cfg_uint32_value = kTerfStrip32; // 4 bytes FCS to be stripped

  pcap_dag pd = {};
  pd.dag_ref = test_dag_ref_ptr;
  pd.dag_root = test_dag_root_ptr;
  pd.tx_iface = 0;
  pd.tx_align_bytes = 0;
  pd.terf_fcs_bytes = 0;

  pcap_t p = {};
  p.priv = &pd;
  p.fd = 10;

  int ret = dag_activate_tx(&p);

  EXPECT_EQ(ret, 0);
  EXPECT_EQ(pd.terf_fcs_bytes, 4);
}

// -------------------- Main: run all tests --------------------

int main() {
  // Step 2 and Step 3: Comprehensive unit test suite
  // All tests are driven by the same DAG activation path, with test doubles to
  // exercise different branches of the focal method.

  // Candidate Keywords observed during analysis (used for traceability):
  // - ENV_TX_IFACE, strtouint31, PCI_DEVICE_ID_VDAG, DAG9_2X2, DAG9_2SX2,
  //   TERF strip configuration, TERF FCS bytes, dag_config_get_interface_count,
  //   dag_pciinfo, dag_component_get_subcomponent, dag_component_get_config_attribute_uuid,
  //   dag_config_get_uint32_attribute_ex, pcapint_fmt_errmsg_for_errno

  // Run tests
  do_test_case_negative_iface();
  do_test_case_iface_out_of_range();
  do_test_case_inf_null();
  do_test_case_vdag_not_supported();
  do_test_case_alignment_9_2_selected();
  do_test_case_cfg_comp_null();
  do_test_case_cfg_uuid_null();
  do_test_case_cfg_read_error();
  do_test_case_terf_no_strip();
  do_test_case_terf_strip32();

  print_summary();
  return (g_tests_fail == 0) ? 0 : 1;
}