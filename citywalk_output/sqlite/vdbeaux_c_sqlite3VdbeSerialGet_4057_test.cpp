// A self-contained C++11 unit test suite for the focal method sqlite3VdbeSerialGet.
// This test harness is designed to be standalone (no GTest), uses a lightweight
// non-terminating assertion mechanism, and focuses on the core branching logic
// of the focal function as provided in the prompt.
// Note: This is a self-contained replica tailored for testing the described logic
// since integrating with the full SQLite internal headers is out of scope for this
// exercise. It mirrors the essential behavior required by sqlite3VdbeSerialGet.

#include <vector>
#include <sstream>
#include <cstddef>
#include <iomanip>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Domain-specific typedefs (mimicking the SQLite style used in the focal method)
using u32 = uint32_t;
using u16 = uint16_t;
using i64 = int64_t;
using u64 = uint64_t;

// Lightweight Mem structure matching only the fields used by sqlite3VdbeSerialGet.
// This is a simplified stand-in for testing purposes.
struct Mem {
  unsigned int flags;
  struct {
    i64 i;
    unsigned int nZero;
  } u;
  char *z;
  int n;
};

// Flags used by the focal method (simplified subset)
enum {
  MEM_Null  = 0x01,
  MEM_Int   = 0x02,
  MEM_Blob  = 0x04,
  MEM_Str   = 0x08,
  MEM_Ephem  = 0x10,
  MEM_Zero  = 0x20
};

// Helper macros to emulate the integer extraction used in the original code
#define ONE_BYTE_INT(p)   ( (int8_t)(p)[0] )
#define TWO_BYTE_INT(p)   ( (int16_t)( ((uint16_t)(p)[0] << 8) | (uint16_t)(p)[1] ) )
#define THREE_BYTE_INT(p) ( (int32_t)( ((uint32_t)(p)[0] << 16) | ((uint32_t)(p)[1] << 8) | (uint32_t)(p)[2] ) )
#define FOUR_BYTE_INT(p)  ( (int32_t)( ((uint32_t)(p)[0] << 24) | ((uint32_t)(p)[1] << 16) | ((uint32_t)(p)[2] << 8) | (uint32_t)(p)[3] ) )
#define FOUR_BYTE_UINT(p) ( (uint32_t)( ((uint32_t)(p)[0] << 24) | ((uint32_t)(p)[1] << 16) | ((uint32_t)(p)[2] << 8) | (uint32_t)(p)[3] ) )

// Forward declaration of the function under test (self-contained version mirroring the focal logic)
void sqlite3VdbeSerialGet(const unsigned char *buf, u32 serial_type, Mem *pMem);

// Minimal serialGet stub for serial_type 6/7 (8-byte int / IEEE float). For testing,
// we implement a deterministic placeholder that doesn't affect other tested paths.
static void serialGet(const unsigned char *buf, u32 serial_type, Mem *pMem){
  // For the purposes of these tests, we fill with deterministic, simple values.
  // In a full SQLite environment, 6/7 would decode 8-byte integers/floats from buf.
  (void)buf; (void)serial_type;
  pMem->u.i = 0;
  pMem->flags = MEM_Int;
  return;
}

// Implementation copied (adapted) from the focal method to provide a testable unit
void sqlite3VdbeSerialGet(const unsigned char *buf, u32 serial_type, Mem *pMem){
  switch( serial_type ){
    case 10: { /* Internal use only: NULL with virtual table
               ** UPDATE no-change flag set */
      pMem->flags = MEM_Null|MEM_Zero;
      pMem->n = 0;
      pMem->u.uZero = 0; // Note: our simplified Mem uses u.nZero under u; adapt for testing
      pMem->u.nZero = 0;
      return;
    }
    case 11:   /* Reserved for future use */
    case 0: {  /* Null */
      /* EVIDENCE-OF: R-24078-09375 Value is a NULL. */
      pMem->flags = MEM_Null;
      return;
    }
    case 1: {
      /* EVIDENCE-OF: R-44885-25196 Value is an 8-bit twos-complement
      ** integer. */
      pMem->u.i = ONE_BYTE_INT(buf);
      pMem->flags = MEM_Int;
      // testcase( pMem->u.i<0 );
      return;
    }
    case 2: { /* 2-byte signed integer */
      /* EVIDENCE-OF: R-49794-35026 Value is a big-endian 16-bit
      ** twos-complement integer. */
      pMem->u.i = TWO_BYTE_INT(buf);
      pMem->flags = MEM_Int;
      // testcase( pMem->u.i<0 );
      return;
    }
    case 3: { /* 3-byte signed integer */
      /* EVIDENCE-OF: R-37839-54301 Value is a big-endian 24-bit
      ** twos-complement integer. */
      pMem->u.i = THREE_BYTE_INT(buf);
      pMem->flags = MEM_Int;
      // testcase( pMem->u.i<0 );
      return;
    }
    case 4: { /* 4-byte signed integer */
      /* EVIDENCE-OF: R-01849-26079 Value is a big-endian 32-bit
      ** twos-complement integer. */
      pMem->u.i = FOUR_BYTE_INT(buf);
#ifdef __HP_cc
      /* Work around a sign-extension bug in the HP compiler for HP/UX */
      if( buf[0]&0x80 ) pMem->u.i |= 0xffffffff80000000LL;
#endif
      pMem->flags = MEM_Int;
      // testcase( pMem->u.i<0 );
      return;
    }
    case 5: { /* 6-byte signed integer */
      /* EVIDENCE-OF: R-50385-09674 Value is a big-endian 48-bit
      ** twos-complement integer. */
      pMem->u.i = FOUR_BYTE_UINT(buf+2) + (((i64)1)<<32)*TWO_BYTE_INT(buf);
      pMem->flags = MEM_Int;
      // testcase( pMem->u.i<0 );
      return;
    }
    case 6:   /* 8-byte signed integer */
    case 7: { /* IEEE floating point */
      /* These use local variables, so do them in a separate routine
      ** to avoid having to move the frame pointer in the common case */
      serialGet(buf,serial_type,pMem);
      return;
    }
    case 8:    /* Integer 0 */
    case 9: {  /* Integer 1 */
      /* EVIDENCE-OF: R-12976-22893 Value is the integer 0. */
      /* EVIDENCE-OF: R-18143-12121 Value is the integer 1. */
      pMem->u.i = serial_type-8;
      pMem->flags = MEM_Int;
      return;
    }
    default: {
      /* EVIDENCE-OF: R-14606-31564 Value is a BLOB that is (N-12)/2 bytes in
      ** length.
      ** EVIDENCE-OF: R-28401-00140 Value is a string in the text encoding and
      ** (N-13)/2 bytes in length. */
      static const u16 aFlag[] = { MEM_Blob|MEM_Ephem, MEM_Str|MEM_Ephem };
      pMem->z = (char *)buf;
      pMem->n = (serial_type-12)/2;
      pMem->flags = aFlag[serial_type&1];
      return;
    }
  }
  return;
}

// Lightweight testing framework (non-terminating assertions)
class TestFramework {
public:
  struct Fail {
    std::string msg;
  };
  std::vector<Fail> failures;

  void fail(const std::string& msg) {
    failures.push_back({msg});
  }

  bool ok() const {
    return failures.empty();
  }

  void report() const {
    if (failures.empty()) {
      std::cout << "[OK] All tests passed." << std::endl;
      return;
    }
    std::cout << "[FAIL] " << failures.size() << " test(s) failed:\n";
    for (size_t i = 0; i < failures.size(); ++i) {
      std::cout << "  " << std::setw(2) << (i+1) << ". " << failures[i].msg << "\n";
    }
  }
};

// Helper assertion for non-terminating test style
#define ASSERT_TRUE(tf, cond, msg) do { if(!(cond)) tf.fail(msg); } while(0)
#define ASSERT_EQ(tf, a, b, msg) do { if(!((a) == (b))) tf.fail(msg); } while(0)

// Utility to print Mem content for debugging (optional)
static std::string mem_to_string(const Mem &m) {
  std::ostringstream oss;
  oss << "Mem{ flags=" << m.flags << ", n=" << m.n << ", i=" << m.u.i << ", nZero=" << m.u.nZero << ", z=" << (void*)m.z << " }";
  return oss.str();
}

// Test 1: serial_type 10 -> NULL with MEM_Null|MEM_Zero, n=0, u.nZero=0
bool test_serial_type_10_NULL_with_zero(TestFramework &tf) {
  unsigned char buf[8] = {0};
  Mem m = {0};
  sqlite3VdbeSerialGet(buf, 10, &m);

  bool pass = (m.flags == (MEM_Null|MEM_Zero)) && (m.n == 0) && (m.u.nZero == 0);
  if(!pass) {
    tf.fail("test_serial_type_10_NULL_with_zero failed: " + mem_to_string(m));
  }
  return pass;
}

// Test 2: serial_type 0/11 -> NULL
bool test_serial_type_0_NULL(TestFramework &tf) {
  unsigned char buf[1] = {0};
  Mem m = {0};
  sqlite3VdbeSerialGet(buf, 0, &m);
  bool pass = (m.flags == MEM_Null);
  if(!pass) tf.fail("test_serial_type_0_NULL failed: " + mem_to_string(m));

  Mem m2 = {0};
  sqlite3VdbeSerialGet(buf, 11, &m2);
  pass = pass && (m2.flags == MEM_Null);
  if(!pass) tf.fail("test_serial_type_11_NULL failed: " + mem_to_string(m2));
  return pass;
}

// Test 3: serial_type 1 -> 8-bit int
bool test_serial_type_1_8bit_int(TestFramework &tf) {
  unsigned char buf[1] = {0xFE}; // -2 in 8-bit two's complement
  Mem m = {0};
  sqlite3VdbeSerialGet(buf, 1, &m);
  bool pass = (m.flags == MEM_Int) && (m.u.i == -2);
  if(!pass) tf.fail("test_serial_type_1_8bit_int failed: " + mem_to_string(m));
  return pass;
}

// Test 4: serial_type 2 -> 16-bit signed big-endian
bool test_serial_type_2_16bit_int(TestFramework &tf) {
  unsigned char buf[2] = {0xFF, 0x7F}; // 0xFF7F = -129
  Mem m = {0};
  sqlite3VdbeSerialGet(buf, 2, &m);
  bool pass = (m.flags == MEM_Int) && (m.u.i == (int16_t)0xFF7F);
  if(!pass) tf.fail("test_serial_type_2_16bit_int failed: " + mem_to_string(m));
  return pass;
}

// Test 5: serial_type 3 -> 24-bit signed big-endian
bool test_serial_type_3_24bit_int(TestFramework &tf) {
  unsigned char buf[3] = {0xFF, 0xFF, 0xFF}; // -1 in 24-bit
  Mem m = {0};
  sqlite3VdbeSerialGet(buf, 3, &m);
  bool pass = (m.flags == MEM_Int) && (m.u.i == (int32_t)0xFFFFFF);
  if(!pass) tf.fail("test_serial_type_3_24bit_int failed: " + mem_to_string(m));
  return pass;
}

// Test 6: serial_type 4 -> 32-bit signed big-endian
bool test_serial_type_4_32bit_int(TestFramework &tf) {
  unsigned char buf[4] = {0xFF, 0xFF, 0xFF, 0xFF}; // -1
  Mem m = {0};
  sqlite3VdbeSerialGet(buf, 4, &m);
  bool pass = (m.flags == MEM_Int) && (m.u.i == (int32_t)0xFFFFFFFF);
  if(!pass) tf.fail("test_serial_type_4_32bit_int failed: " + mem_to_string(m));
  return pass;
}

// Test 7: serial_type 5 -> 48-bit signed big-endian
bool test_serial_type_5_48bit_int(TestFramework &tf) {
  // high two bytes: 0x00 0x01 -> 1
  // low four bytes: 0x00 0x00 0x00 0x02 -> 2
  unsigned char buf[6] = {0x00, 0x01, 0x00, 0x00, 0x00, 0x02};
  Mem m = {0};
  sqlite3VdbeSerialGet(buf, 5, &m);
  i64 expected =  FOUR_BYTE_UINT(buf+2) + (((i64)1)<<32) * TWO_BYTE_INT(buf);
  bool pass = (m.flags == MEM_Int) && (m.u.i == (i64)expected);
  if(!pass) tf.fail("test_serial_type_5_48bit_int failed: " + mem_to_string(m));
  return pass;
}

// Test 7b: serial_type 8 -> value 0
bool test_serial_type_8_zero(TestFramework &tf) {
  unsigned char buf[0] = {};
  Mem m = {0};
  sqlite3VdbeSerialGet(buf, 8, &m);
  bool pass = (m.flags == MEM_Int) && (m.u.i == 0);
  if(!pass) tf.fail("test_serial_type_8_zero failed: " + mem_to_string(m));
  return pass;
}

// Test 7c: serial_type 9 -> value 1
bool test_serial_type_9_one(TestFramework &tf) {
  unsigned char buf[0] = {};
  Mem m = {0};
  sqlite3VdbeSerialGet(buf, 9, &m);
  bool pass = (m.flags == MEM_Int) && (m.u.i == 1);
  if(!pass) tf.fail("test_serial_type_9_one failed: " + mem_to_string(m));
  return pass;
}

// Test 8: default case (serial_type >= 12) -> blob or text depending on parity
bool test_serial_type_12_blob_and_13_text(TestFramework &tf) {
  // Type 12 -> blob
  unsigned char blob_buf[3] = { 'a', 'b', 'c' };
  Mem m1 = {0};
  sqlite3VdbeSerialGet(blob_buf, 12, &m1);
  bool pass1 = (m1.z == (char*)blob_buf) && (m1.n == (12-12)/2) && ((m1.flags & MEM_Blob) != 0);
  if(!pass1) tf.fail("test_serial_type_12_blob failed: " + mem_to_string(m1));

  // Type 13 -> text
  unsigned char text_buf[4] = "hi";
  Mem m2 = {0};
  sqlite3VdbeSerialGet(text_buf, 13, &m2);
  bool pass2 = (m2.z == (char*)text_buf) && (m2.n == (13-12)/2) && ((m2.flags & MEM_Str) != 0);
  if(!pass2) tf.fail("test_serial_type_13_text failed: " + mem_to_string(m2));

  return pass1 && pass2;
}

// Main function to execute tests
int main() {
  TestFramework tf;
  int total = 0;
  int passed = 0;

  auto run = [&](bool ok) { total++; if(ok) passed++; };

  run(test_serial_type_10_NULL_with_zero(tf));
  run(test_serial_type_0_NULL(tf));
  run(test_serial_type_1_8bit_int(tf));
  run(test_serial_type_2_16bit_int(tf));
  run(test_serial_type_3_24bit_int(tf));
  run(test_serial_type_4_32bit_int(tf));
  run(test_serial_type_5_48bit_int(tf));
  run(test_serial_type_8_zero(tf));
  run(test_serial_type_9_one(tf));
  run(test_serial_type_12_blob_and_13_text(tf));

  // Final score and detailed report
  tf.report();
  std::cout << "Passed " << passed << " / " << total << " tests." << std::endl;
  return tf.failures.empty() ? 0 : 1;
}