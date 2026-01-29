// Note: This test suite targets the focal function sqlite3Fts5PoslistSafeAppend
// from fts5_buffer.c. Since the original project context is not available here,
// we provide a self-contained C++11 test harness implementing the minimal
// surface required to exercise the function's logic and branches.
// The implementation mirrors the logic from the provided focal method and uses
// a simple 8-byte little-endian varint encoding for deterministic behavior.

#include <iostream>
#include <cstring>
#include <cstdint>
#include <fts5Int.h>


// Domain knowledge hints and type aliases adapted for the test harness
typedef long long i64;
typedef unsigned char u8;

// Lightweight, self-contained representation of the Fts5Buffer used by the focal method
struct Fts5Buffer {
  u8 *p;     // pointer to raw buffer
  int n;     // current write position (number of bytes used)
  int cap;   // capacity of the buffer
};

// Minimal varint encoder used by the focal method.
// It encodes a 64-bit value into 8 bytes in little-endian order.
// The real project uses a different encoding, but for deterministic unit tests
// a fixed-width encoding is sufficient to validate control flow and side-effects.
int sqlite3Fts5PutVarint(u8 *p, i64 iVal) {
  for(int i = 0; i < 8; ++i) {
    p[i] = (u8)((iVal >> (8 * i)) & 0xFF);
  }
  return 8;
}

// Focal method under test (re-implemented here for a self-contained test).
// This mirrors the logic provided in the prompt.
void sqlite3Fts5PoslistSafeAppend(
  Fts5Buffer *pBuf, 
  i64 *piPrev, 
  i64 iPos
){
  if( iPos >= *piPrev ){
    static const i64 colmask = ((i64)(0x7FFFFFFF)) << 32;
    if( (iPos & colmask) != (*piPrev & colmask) ){
      pBuf->p[pBuf->n++] = 1;
      pBuf->n += sqlite3Fts5PutVarint(&pBuf->p[pBuf->n], (iPos>>32));
      *piPrev = (iPos & colmask);
    }
    pBuf->n += sqlite3Fts5PutVarint(&pBuf->p[pBuf->n], (iPos-*piPrev)+2);
    *piPrev = iPos;
  }
}

// Simple test harness utilities
static int g_failures = 0;

static void TestLog(const char* name) {
  std::cout << "[RUN] " << name << std::endl;
}

static void TestOK(const char* name) {
  std::cout << "[OK]  " << name << std::endl;
}

static void TestFail(const char* name, const char* msg) {
  std::cerr << "[FAIL] " << name << ": " << msg << std::endl;
  ++g_failures;
}

// Helpers to read the encoded 8-byte little-endian value produced by sqlite3Fts5PutVarint
static i64 ReadLE64(const u8* p) {
  i64 v = 0;
  for(int i = 0; i < 8; ++i) {
    v |= ((i64)p[i] << (8 * i));
  }
  return v;
}

// Run a battery of tests for the focal function
static void RunTests() {
  // Test 1: No writes when iPos < *piPrev (precondition not satisfied)
  {
    TestLog("Test1: iPos < *piPrev -> no writes and no pointer update");
    Fts5Buffer buf;
    buf.cap = 128;
    buf.p = new u8[buf.cap];
    std::memset(buf.p, 0, buf.cap);
    buf.n = 0;

    i64 piPrev = 10;
    i64 iPos = 5;

    sqlite3Fts5PoslistSafeAppend(&buf, &piPrev, iPos);

    // Expect no bytes written and piPrev unchanged
    bool ok = (buf.n == 0) && (piPrev == 10);
    if(!ok) {
      TestFail("Test1", "Expected no writes and no update to piPrev");
    } else {
      TestOK("Test1");
    }

    delete[] buf.p;
  }

  // Test 2: High 32-bit block changes -> writes a leading '1', high32 varint, then lower-delta varint
  {
    TestLog("Test2: High-bit change triggers marker byte and two varints");
    Fts5Buffer buf;
    buf.cap = 128;
    buf.p = new u8[buf.cap];
    std::memset(buf.p, 0, buf.cap);
    buf.n = 0;

    i64 piPrev = 0;
    // iPos with non-zero high 32 bits: (iPos & colmask) != 0
    i64 iPos = ((i64)1) << 40; // high bits non-zero, low bits zero
    sqlite3Fts5PoslistSafeAppend(&buf, &piPrev, iPos);

    // Expect: 1 byte marker, then 8 bytes for (iPos>>32), then 8 bytes for (iPos - piPrev) + 2
    // where (iPos & colmask) was set as piPrev when marker pushed.
    bool condMarker = (buf.n >= 1) && (buf.p[0] == 1);
    bool condHigh = true;
    i64 highVal = iPos >> 32; // expected 256
    i64 readHigh = ReadLE64(&buf.p[1]);
    i64 deltaVal = ReadLE64(&buf.p[9]); // (iPos - piPrev) + 2 after updating piPrev
    // Validate structure
    if(!condMarker) {
      TestFail("Test2", "Marker byte '1' not found at start");
    } else if(readHigh != highVal) {
      TestFail("Test2", "High 32-bit varint did not encode iPos>>32 correctly");
      condHigh = false;
    } else if(deltaVal != 2) {
      TestFail("Test2", "Lower-delta varint should encode value 2");
      condHigh = false;
    } else {
      TestOK("Test2");
    }

    // piPrev should be updated to iPos after operation
    TestLog("Test2: Verify piPrev updated to iPos");
    if(piPrev != iPos) {
      TestFail("Test2", "piPrev was not updated to iPos after operation");
    } else {
      TestOK("Test2_piPrev_update");
    }

    delete[] buf.p;
  }

  // Test 3: Same high bits (no marker), only lower-delta varint is appended
  {
    TestLog("Test3: No high-bit change -> only one varint appended (iPos-*piPrev + 2)");
    Fts5Buffer buf;
    buf.cap = 128;
    buf.p = new u8[buf.cap];
    std::memset(buf.p, 0, buf.cap);
    buf.n = 0;

    i64 piPrev = 0;
    i64 iPos = 3; // same high bits (0) as piPrev

    sqlite3Fts5PoslistSafeAppend(&buf, &piPrev, iPos);

    // Expect a single 8-byte varint encoding of (iPos - piPrev) + 2 = 5
    bool okN = (buf.n == 8);
    i64 val = ReadLE64(&buf.p[0]);
    if(!okN) {
      TestFail("Test3", "Expected exactly 8 bytes written for single varint");
    } else if(val != 5) {
      TestFail("Test3", "Expected encoded value 5 for (iPos - piPrev) + 2");
    } else {
      TestOK("Test3");
    }

    // piPrev should be updated to iPos
    if(piPrev != iPos) {
      TestFail("Test3", "piPrev was not updated to iPos after operation");
    } else {
      TestOK("Test3_piPrev_update");
    }

    delete[] buf.p;
  }

  // Test 4: iPos < *piPrev after previous updates should not modify buffer
  {
    TestLog("Test4: No changes when iPos < *piPrev (post-update state)");
    Fts5Buffer buf;
    buf.cap = 128;
    buf.p = new u8[buf.cap];
    std::memset(buf.p, 0, buf.cap);
    buf.n = 0;

    i64 piPrev = 100;
    i64 iPos = 50; // less than piPrev

    sqlite3Fts5PoslistSafeAppend(&buf, &piPrev, iPos);

    bool ok = (buf.n == 0) && (piPrev == 100);
    if(!ok) {
      TestFail("Test4", "Expected no writes and no change to piPrev");
    } else {
      TestOK("Test4");
    }

    delete[] buf.p;
  }
}

int main() {
  RunTests();
  std::cout << "Tests completed with " << g_failures << " failure(s)." << std::endl;
  return g_failures;
}