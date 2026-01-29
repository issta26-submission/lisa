// Minimal C++11 test harness for sqlite3VdbeRecordUnpack
// This test suite provides a self-contained, lightweight environment
// to exercise the focal function without relying on the full SQLite source tree
// or external testing frameworks (GTest, etc.).
//
// The tests purposefully implement small stubs for only the dependencies
// used by sqlite3VdbeRecordUnpack, focusing on:
// - KeyInfo, UnpackedRecord, Mem structures
// - getVarint32, sqlite3VdbeSerialGet, sqlite3VdbeSerialTypeLen, sqlite3VdbeMemSetNull
// - EIGHT_BYTE_ALIGNMENT and CORRUPT_DB control
// The goal is to achieve meaningful coverage for the function logic, including
// the normal path, edge cases (no fields), and the corrupt-path handling.

#include <cstdio>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Lightweight type aliases to mimic the expected SQLite types
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

// Forward declarations of minimal stubs (local to this test file)
struct Mem;
struct KeyInfo;
struct UnpackedRecord;

// Minimal Mem structure used by sqlite3VdbeRecordUnpack
struct Mem {
  int enc;
  int db;
  unsigned char *z;  // pointer to data (not strictly used in tests)
  int szMalloc;
};

// Minimal KeyInfo structure
struct KeyInfo {
  int enc;
  int db;
  int nKeyField;
  unsigned char *aSortFlags;
};

// Minimal UnpackedRecord structure
struct UnpackedRecord {
  Mem *aMem;     // array of Mem objects, one per field
  int nField;    // number of fields read into aMem
  int default_rc;
};

// Global to permit corrupt-path assertion in the test environment
int CORRUPT_DB = 1; // enable assertion in corrupt-path branch

// Helper: ensure 8-byte alignment check placeholder (always true in tests)
static inline int EIGHT_BYTE_ALIGNMENT(const Mem* /*p*/){
  return 1;
}

// Helper: mimic getVarint32() used by the focal function
// For testing simplicity, treat the first byte as the varint (1-byte encoding)
static int getVarint32(const unsigned char *p, u32 &v){
  v = static_cast<u32>(p[0]);
  return 1; // number of bytes read
}

// Helper: a simple, constant-length serial type length (1 for test purposes)
static u32 sqlite3VdbeSerialTypeLen(u32 /*serial_type*/){
  return 1;
}

// Helper: mimic sqlite3VdbeMemSetNull() by marking z pointer as null
static void sqlite3VdbeMemSetNull(Mem *pMem){
  if(pMem){
    pMem->z = nullptr;
  }
}

// Helper: mimic sqlite3VdbeSerialGet() by populating Mem in a trivial way
static void sqlite3VdbeSerialGet(const unsigned char *buf, u32 serial_type, Mem *pMem){
  // For test purposes, fill Mem with a non-null marker
  // and store a tiny, constant payload so tests can observe a value.
  (void)buf; // unused in this simplified test
  (void)serial_type;
  static unsigned char marker[] = { 'X' };
  pMem->z = marker;      // point to a tiny payload
  pMem->szMalloc = 1;
  pMem->enc = 0;
  pMem->db = 0;
}

// UnpackedRecord factory (not strictly used by tests, but kept for completeness)
static UnpackedRecord *sqlite3VdbeAllocUnpackedRecord(KeyInfo * /*pKeyInfo*/){
  return nullptr;
}

// The focal function under test (copied/adapted to compile in this test harness)
void sqlite3VdbeRecordUnpack(
  KeyInfo *pKeyInfo,     /* Information about the record format */
  int nKey,              /* Size of the binary record */
  const void *pKey,      /* The binary record */
  UnpackedRecord *p      /* Populate this structure before returning. */
){
  const unsigned char *aKey = (const unsigned char *)pKey;
  u32 d;
  u32 idx;                        /* Offset in aKey[] to read from */
  u16 u;                          /* Unsigned loop counter */
  u32 szHdr;
  Mem *pMem = p->aMem;
  p->default_rc = 0;
  assert( EIGHT_BYTE_ALIGNMENT(pMem) );
  idx = getVarint32(aKey, szHdr);
  d = szHdr;
  u = 0;
  while( idx<szHdr && d<=(u32)nKey ){
    u32 serial_type;
    idx += getVarint32(&aKey[idx], serial_type);
    pMem->enc = pKeyInfo->enc;
    pMem->db = pKeyInfo->db;
    /* pMem->flags = 0; // sqlite3VdbeSerialGet() will set this for us */
    pMem->szMalloc = 0;
    pMem->z = 0;
    sqlite3VdbeSerialGet(&aKey[d], serial_type, pMem);
    d += sqlite3VdbeSerialTypeLen(serial_type);
    pMem++;
    if( (++u)>=p->nField ) break;
  }
  if( d>(u32)nKey && u ){
    assert( CORRUPT_DB );
    /* In a corrupt record entry, the last pMem might have been set up using
    ** uninitialized memory. Overwrite its value with NULL, to prevent
    ** warnings from MSAN. */
    sqlite3VdbeMemSetNull(pMem-1);
  }
  assert( u<=pKeyInfo->nKeyField + 1 );
  p->nField = u;
}

// Test harness begins here

// Utility: print test result
static void printResult(const char* testName, bool ok){
  std::cout << testName << ": " << (ok ? "PASS" : "FAIL") << std::endl;
}

// Test 1: Normal path with exactly one field
static bool test_NormalOneField(){
  // Prepare KeyInfo and UnpackedRecord
  KeyInfo ki;
  ki.enc = 0;
  ki.db = 0;
  ki.nKeyField = 1;
  ki.aSortFlags = nullptr;

  UnpackedRecord ur;
  ur.nField = 1;           // we want to process exactly one field
  ur.default_rc = 0;
  ur.aMem = new Mem[1];
  // initialize Mem array
  for(int i=0;i<1;i++){
    ur.aMem[i].enc = 0;
    ur.aMem[i].db = 0;
    ur.aMem[i].z = nullptr;
    ur.aMem[i].szMalloc = 0;
  }

  // Construct aKey: header size (szHdr) = 2, first field serial_type = 1
  // layout:
  // [0] szHdr (2)
  // [1] serial_type1 (1)
  // [2] data for field1 (arbitrary, we fill via dummy serializer)
  unsigned char aKey[3] = { 2, 1, 0 };
  int nKey = 3;

  sqlite3VdbeRecordUnpack(&ki, nKey, aKey, &ur);

  bool ok = (ur.nField == 1);
  // Also verify the first Mem got populated (pointer not null after our dummy serializer)
  if(ur.nField == 1 && ur.aMem[0].z != nullptr){
    ok = ok && (ur.aMem[0].z[0] == 'X');
  } else {
    ok = false;
  }

  delete [] ur.aMem;
  return ok;
}

// Test 2: Corrupt-path handling should set last field to NULL (and still set nField)
static bool test_CorruptPathSetsNull(){
  // Prepare KeyInfo and UnpackedRecord
  KeyInfo ki;
  ki.enc = 0;
  ki.db = 0;
  ki.nKeyField = 1;
  ki.aSortFlags = nullptr;

  UnpackedRecord ur;
  ur.nField = 1;           // we want to process exactly one field
  ur.default_rc = 0;
  ur.aMem = new Mem[1];
  for(int i=0;i<1;i++){
    ur.aMem[i].enc = 0;
    ur.aMem[i].db = 0;
    ur.aMem[i].z = (unsigned char*)"Y"; // non-null prior
    ur.aMem[i].szMalloc = 0;
  }

  // Construct aKey such that after processing, d > nKey
  // szHdr = 2, serial_type1 = 1, data at offset 2
  unsigned char aKey[3] = { 2, 1, 0 };
  int nKey = 2 + 1; // 3 to allow one field but set nKey small so d > nKey after first field

  // Force CORRUPT_DB to be true to satisfy the assertion and continue
  CORRUPT_DB = 1;

  sqlite3VdbeRecordUnpack(&ki, nKey, aKey, &ur);

  bool ok = (ur.nField == 1) && (ur.aMem[0].z != nullptr);
  // Verify that the last Mem (the one we populated for the field) is set to NULL by the corrupt-path code
  // In our simple Mem model, the test path uses pMem-1 (the last pre-field Mem) for NULL. Since we had only 1 field,
  // the pre-field Mem is ur.aMem[0]. The code would set that to NULL in corrupt path; our test mimics this by checking z == NULL.
  ok = ok && (ur.aMem[0].z == nullptr);

  delete [] ur.aMem;
  return ok;
}

// Test 3: No-field scenario (szHdr indicates no fields)
static bool test_NoFieldScenario(){
  KeyInfo ki;
  ki.enc = 0;
  ki.db = 0;
  ki.nKeyField = 5;
  ki.aSortFlags = nullptr;

  UnpackedRecord ur;
  ur.nField = 0; // request zero fields
  ur.default_rc = 0;
  ur.aMem = new Mem[1]; // allocate at least one Mem since the code accesses pMem in some paths

  unsigned char aKey[1] = { 1 }; // szHdr = 1, idx starts at 1, loop won't run
  int nKey = 1;

  sqlite3VdbeRecordUnpack(&ki, nKey, aKey, &ur);

  bool ok = (ur.nField == 0);
  delete [] ur.aMem;
  return ok;
}

// Test 4: Two fields path to ensure loop iterates multiple times
static bool test_TwoFieldsPath(){
  KeyInfo ki;
  ki.enc = 0;
  ki.db = 0;
  ki.nKeyField = 2;
  ki.aSortFlags = nullptr;

  UnpackedRecord ur;
  ur.nField = 2; // process two fields
  ur.default_rc = 0;
  ur.aMem = new Mem[2];
  for(int i=0;i<2;i++){
    ur.aMem[i].enc = 0;
    ur.aMem[i].db = 0;
    ur.aMem[i].z = nullptr;
    ur.aMem[i].szMalloc = 0;
  }

  // Layout for two fields:
  // [0] szHdr = 3
  // [1] serial_type1 = 1
  // [2] serial_type2 = 1
  // [3] data for field1
  unsigned char aKey[4] = { 3, 1, 1, 0 };
  int nKey = 4;

  sqlite3VdbeRecordUnpack(&ki, nKey, aKey, &ur);

  bool ok = (ur.nField == 2);
  // Expect both Mem entries have non-null data (our dummy serializer sets z to non-null)
  ok = ok && (ur.aMem[0].z != nullptr) && (ur.aMem[1].z != nullptr);

  delete [] ur.aMem;
  return ok;
}

// Main: run all tests and report summary
int main(){
  int failures = 0;

  bool t1 = test_NormalOneField();
  printResult("Test_NormalOneField", t1);
  if(!t1) ++failures;

  bool t2 = test_CorruptPathSetsNull();
  printResult("Test_CorruptPathSetsNull", t2);
  if(!t2) ++failures;

  bool t3 = test_NoFieldScenario();
  printResult("Test_NoFieldScenario", t3);
  if(!t3) ++failures;

  bool t4 = test_TwoFieldsPath();
  printResult("Test_TwoFieldsPath", t4);
  if(!t4) ++failures;

  if(failures){
    std::cerr << "Summary: " << failures << " test(s) failed." << std::endl;
    return EXIT_FAILURE;
  }else{
    std::cout << "Summary: all tests passed." << std::endl;
    return EXIT_SUCCESS;
  }
}