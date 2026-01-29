#include <vector>
#include <btreeInt.h>
#include <string>
#include <cstring>
#include <iostream>
#include <cassert>


// Lightweight in-tree test harness and stubs to exercise the focal method
// without GTest. This follows the constraints from <DOMAIN_KNOWLEDGE>.

// Define basic macros expected by the focal function
#ifndef CONST
#define CONST const
#endif

#ifndef TCL_OK
#define TCL_OK 0
#endif

#ifndef SQLITE_TCLAPI
#define SQLITE_TCLAPI
#endif

// GLOBAL macro emulation used by the focal method
#ifndef GLOBAL
#define GLOBAL(type, symbol) (*(type *)&symbol)
#endif

// Forward-declare Tcl-like structures used by the focal function
struct Tcl_Obj;
struct Tcl_Interp;

// Very small Tcl-like object model sufficient for testing
enum TclObjType { TCL_STRING = 0, TCL_INT = 1, TCL_LIST = 2 };

struct Tcl_Obj {
    int type;
    std::string s;           // for STRING
    int intVal;                // for INT
    std::vector<Tcl_Obj*> l;  // for LIST
    Tcl_Obj() : type(TCL_LIST), intVal(0) {}
};

// Minimal Tcl Interpreter stub
struct Tcl_Interp {
    Tcl_Obj* result;
};

// Minimal emulation of Tcl API used by sqlite3BtreeSharedCacheReport
Tcl_Obj* Tcl_NewObj() {
    return new Tcl_Obj();
}
Tcl_Obj* Tcl_NewStringObj(const char* s, int len) {
    Tcl_Obj* o = new Tcl_Obj();
    o->type = TCL_STRING;
    if (s) o->s = std::string(s, (len == -1 ? static_cast<size_t>(std::strlen(s)) : static_cast<size_t>(len)));
    else o->s = "";
    return o;
}
Tcl_Obj* Tcl_NewIntObj(int i) {
    Tcl_Obj* o = new Tcl_Obj();
    o->type = TCL_INT;
    o->intVal = i;
    return o;
}
void Tcl_ListObjAppendElement(Tcl_Interp* /*interp*/, Tcl_Obj* listPtr, Tcl_Obj* elem) {
    if (!listPtr) return;
    if (listPtr->type != TCL_LIST) listPtr->type = TCL_LIST;
    listPtr->l.push_back(elem);
}
void Tcl_SetObjResult(Tcl_Interp* interp, Tcl_Obj* obj) {
    interp->result = obj;
}
Tcl_Obj* Tcl_GetObjResult(Tcl_Interp* interp) {
    return interp ? interp->result : nullptr;
}

// Domain: Pager and Btree related small structs to support the focal method
struct Pager {
    const char* zFilename;
};

struct BtShared {
    Pager* pPager;
    int nRef;
    BtShared* pNext;
};

// The test harness will provide this global to simulate the environment
BtShared *sqlite3SharedCacheList = nullptr; // head of the shared cache list

// Simulated sqlite3PagerFilename function
const char* sqlite3PagerFilename(Pager* pPager, int /*dummy*/) {
    return pPager ? pPager->zFilename : nullptr;
}

// The focal method under test (reproduced here to form a single translation unit)
#ifndef SQLITE_OMIT_SHARED_CACHE
int SQLITE_TCLAPI sqlite3BtreeSharedCacheReport(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
  extern BtShared *sqlite3SharedCacheList;
  BtShared *pBt;
  Tcl_Obj *pRet = Tcl_NewObj();
  for(pBt=GLOBAL(BtShared*,sqlite3SharedCacheList); pBt; pBt=pBt->pNext){
    const char *zFile = sqlite3PagerFilename(pBt->pPager, 1);
    Tcl_ListObjAppendElement(interp, pRet, Tcl_NewStringObj(zFile, -1));
    Tcl_ListObjAppendElement(interp, pRet, Tcl_NewIntObj(pBt->nRef));
  }
  Tcl_SetObjResult(interp, pRet);
  return TCL_OK;
}
#endif

// Lightweight test harness helpers

// Extract the list of (filename, refcount) pairs from the Tcl-like result
static std::vector<std::pair<std::string,int>> ReadResult(Tcl_Interp* interp) {
    std::vector<std::pair<std::string,int>> out;
    Tcl_Obj* root = Tcl_GetObjResult(interp);
    if (!root || root->type != TCL_LIST) return out;
    for (size_t i = 0; i + 1 < root->l.size(); i += 2) {
        Tcl_Obj* fileObj = root->l[i];
        Tcl_Obj* refObj  = root->l[i+1];
        std::string fname = (fileObj && fileObj->type == TCL_STRING) ? fileObj->s : "";
        int refVal = (refObj && refObj->type == TCL_INT) ? refObj->intVal : 0;
        out.emplace_back(fname, refVal);
    }
    return out;
}

// Helper: clear all allocated Tcl_Obj in a simple way (avoid memory management concerns for tests)
static void FreeResult(Tcl_Obj* obj) {
    if (!obj) return;
    // recursively free if it's a list
    if (obj->type == TCL_LIST) {
        for (auto* child : obj->l) FreeResult(child);
        obj->l.clear();
    }
    delete obj;
}

// Test 1: Empty shared cache list should yield an empty result list
static bool Test_EmptyList() {
    sqlite3SharedCacheList = nullptr; // empty list

    Tcl_Interp interp;
    interp.result = nullptr;

    // Call focal method
    int rc = sqlite3BtreeSharedCacheReport(nullptr, &interp, 0, nullptr);

    bool ok = true;
    if (rc != 0) {
        std::cerr << "[Test_EmptyList] Focal method did not return TCL_OK (rc=" << rc << ")\n";
        ok = false;
    }
    auto res = ReadResult(&interp);
    if (!res.empty()) {
        std::cerr << "[Test_EmptyList] Expected empty result, got " << res.size() << " entries\n";
        ok = false;
    }
    // Cleanup
    FreeResult(interp.result);
    return ok;
}

// Test 2: Single entry in shared cache list
static bool Test_SingleEntry() {
    // Build a single BtShared node
    Pager p1; p1.zFilename = "single_file.db";
    BtShared b1;
    b1.pPager = &p1;
    b1.nRef = 5;
    b1.pNext = nullptr;

    sqlite3SharedCacheList = &b1;

    Tcl_Interp interp;
    interp.result = nullptr;

    int rc = sqlite3BtreeSharedCacheReport(nullptr, &interp, 0, nullptr);
    bool ok = true;
    if (rc != 0) {
        std::cerr << "[Test_SingleEntry] Focal method did not return TCL_OK (rc=" << rc << ")\n";
        ok = false;
    }

    auto res = ReadResult(&interp);
    if (res.size() != 1) {
        std::cerr << "[Test_SingleEntry] Expected 1 entry, got " << res.size() << "\n";
        ok = false;
    } else {
        if (res[0].first != "single_file.db" || res[0].second != 5) {
            std::cerr << "[Test_SingleEntry] Mismatch: expected (single_file.db,5), got ("<<res[0].first<<","<<res[0].second<<")\n";
            ok = false;
        }
    }

    FreeResult(interp.result);
    return ok;
}

// Test 3: Multiple entries: preserves order and pairs
static bool Test_MultipleEntries() {
    // Build three BtShared nodes
    Pager p1; p1.zFilename = "a.db";
    BtShared b1; b1.pPager = &p1; b1.nRef = 1;
    BtShared *head = &b1;

    Pager p2; p2.zFilename = "b.db";
    BtShared b2; b2.pPager = &p2; b2.nRef = 2;
    b1.pNext = &b2;

    Pager p3; p3.zFilename = "c.db";
    BtShared b3; b3.pPager = &p3; b3.nRef = 3;
    b2.pNext = &b3;
    b3.pNext = nullptr;

    sqlite3SharedCacheList = head;

    Tcl_Interp interp;
    interp.result = nullptr;

    int rc = sqlite3BtreeSharedCacheReport(nullptr, &interp, 0, nullptr);
    bool ok = true;
    if (rc != 0) {
        std::cerr << "[Test_MultipleEntries] Focal method did not return TCL_OK (rc=" << rc << ")\n";
        ok = false;
    }

    auto res = ReadResult(&interp);
    if (res.size() != 3) {
        std::cerr << "[Test_MultipleEntries] Expected 3 entries, got " << res.size() << "\n";
        ok = false;
    } else {
        const std::vector<std::pair<std::string,int>> expected = {
            {"a.db", 1}, {"b.db", 2}, {"c.db", 3}
        };
        for (size_t i = 0; i < expected.size(); ++i) {
            if (res[i] != expected[i]) {
                std::cerr << "[Test_MultipleEntries] Mismatch at index " << i
                          << ": expected (" << expected[i].first << "," << expected[i].second
                          << "), got (" << res[i].first << "," << res[i].second << ")\n";
                ok = false;
            }
        }
    }

    FreeResult(interp.result);
    return ok;
}

// Entrypoint to run tests
int main() {
    int total = 0;
    int passed = 0;

    std::cout << "Starting unit tests for sqlite3BtreeSharedCacheReport...\n";

    total++; if (Test_EmptyList()) { ++passed; std::cout << "Test_EmptyList PASSED\n"; } else std::cout << "Test_EmptyList FAILED\n";
    total++; if (Test_SingleEntry()) { ++passed; std::cout << "Test_SingleEntry PASSED\n"; } else std::cout << "Test_SingleEntry FAILED\n";
    total++; if (Test_MultipleEntries()) { ++passed; std::cout << "Test_MultipleEntries PASSED\n"; } else std::cout << "Test_MultipleEntries FAILED\n";

    std::cout << "Tests run: " << total << ", Passed: " << passed << ", Failed: " << (total - passed) << "\n";

    return (passed == total) ? 0 : 1;
}