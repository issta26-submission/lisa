// A self-contained C++11 test suite for the focal C function sqlite3NotPureFunc.
// This test mocks the minimal SQLite context and VDBe structures required by the function.
// It exercises true/false branches and ensures memory management paths are exercised.
// Note: The tests avoid Google Test and rely on simple non-terminating assertions.

#include <cstdio>
#include <cstdarg>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <cstdlib>


// Enable the STAT4 branch to exercise the early return path when pVdbe is NULL.
#define SQLITE_ENABLE_STAT4 1

// Minimal isolated constants to mirror the real code's usage.
#define OP_PureFunc 1
#define NC_IsCheck 1
#define NC_GenCol 2

// Forward declarations of lightweight stubs that the focal function relies on.
struct VdbeOp {
  int opcode;
  int p5;
};

struct Vdbe {
  VdbeOp *aOp;
};

struct FuncDef {
  const char *zName;
};

// Lightweight sqlite3_context substitute
struct sqlite3_context {
  Vdbe *pVdbe;
  int iOp;
  FuncDef *pFunc;
  char *zErr;
};

// Forward declarations of minimal SQLite-like helpers used by the focal function.
extern "C" char *sqlite3_mprintf(const char *zFormat, ...);
extern "C" void sqlite3_result_error(sqlite3_context *pCtx, const char *zMsg, int n);
extern "C" void sqlite3_free(void *p);

// The focal function under test (copied/replicated here with C linkage for the test).
extern "C" int sqlite3NotPureFunc(sqlite3_context *pCtx) {
  const VdbeOp *pOp;
#ifdef SQLITE_ENABLE_STAT4
  if( pCtx->pVdbe==0 ) return 1;
#endif
  pOp = pCtx->pVdbe->aOp + pCtx->iOp;
  if( pOp->opcode==OP_PureFunc ){
    const char *zContext;
    char *zMsg;
    if( pOp->p5 & NC_IsCheck ){
      zContext = "a CHECK constraint";
    }else if( pOp->p5 & NC_GenCol ){
      zContext = "a generated column";
    }else{
      zContext = "an index";
    }
    zMsg = sqlite3_mprintf("non-deterministic use of %s() in %s",
                           pCtx->pFunc->zName, zContext);
    sqlite3_result_error(pCtx, zMsg, -1);
    sqlite3_free(zMsg);
    return 0;
  }
  return 1;
}

// Simple, non-terminating assertion helper
static int g_failures = 0;
#define CHECK(cond, msg) do { if(!(cond)){ \
  printf("ASSERT FAILED: %s\n", (msg)); g_failures++; } } while(0)

// Minimal implementations of the sqlite3 helpers used by sqlite3NotPureFunc

extern "C" char *sqlite3_mprintf(const char *zFormat, ...) {
  va_list args;
  va_start(args, zFormat);
  // Estimate required size
  int size = 128;
  char *buf = (char*)malloc(size);
  if(!buf){
    va_end(args);
    return nullptr;
  }
  int n = vsnprintf(buf, size, zFormat, args);
  if(n >= size){
    // Reallocate to exact size
    size = n + 1;
    char *newbuf = (char*)realloc(buf, size);
    if(newbuf){
      buf = newbuf;
      vsnprintf(buf, size, zFormat, args);
    }
  }
  va_end(args);
  return buf;
}

extern "C" void sqlite3_result_error(sqlite3_context *pCtx, const char *zMsg, int n){
  // Store a copy of the error message in the context for later inspection.
  if(pCtx){
    if(pCtx->zErr){
      free(pCtx->zErr);
      pCtx->zErr = nullptr;
    }
    if(zMsg){
      pCtx->zErr = strdup(zMsg);
    } else {
      pCtx->zErr = nullptr;
    }
  }
}

extern "C" void sqlite3_free(void *p){
  free(p);
}

// Test driver
int main() {
  int testCount = 0;
  int passCount = 0;

  // Helper to print test header
  auto header = [](const char* name){
    printf("=== Test: %s ===\n", name);
  };

  // Shared objects
  FuncDef fNameMyFunc;
  fNameMyFunc.zName = "myFunc";

  // Test 1: When SQLITE_ENABLE_STAT4 is defined and pVdbe is NULL, the function should return 1
  {
    header("Early exit when pVdbe is NULL (STAT4 enabled)");
    testCount++;

    sqlite3_context ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.pVdbe = nullptr; // triggers early return
    ctx.iOp = 0;
    ctx.pFunc = &fNameMyFunc;
    ctx.zErr = nullptr;

    int rc = sqlite3NotPureFunc(&ctx);
    CHECK(rc == 1, "Expected return code 1 when pVdbe is NULL");
    CHECK(ctx.zErr == nullptr, "Expected no error message when early exit");
    if(rc == 1 && ctx.zErr == nullptr) passCount++;
  }

  // Test 2: When opcode is not OP_PureFunc, function should return 1 and not set error
  {
    header("Non-PureFunc opcode should return 1 without error");
    testCount++;

    sqlite3_context ctx;
    memset(&ctx, 0, sizeof(ctx));

    VdbeOp op;
    op.opcode = 999; // not OP_PureFunc
    op.p5 = 0;

    Vdbe vdbe;
    vdbe.aOp = &op;

    ctx.pVdbe = &vdbe;
    ctx.iOp = 0;
    ctx.pFunc = &fNameMyFunc;
    ctx.zErr = nullptr;

    int rc = sqlite3NotPureFunc(&ctx);
    CHECK(rc == 1, "Expected return code 1 for non-PureFunc opcode");
    CHECK(ctx.zErr == nullptr, "Expected no error message for non-PureFunc opcode");
    if(rc == 1 && ctx.zErr == nullptr) passCount++;
  }

  // Test 3: OP_PureFunc with NC_IsCheck should produce a check-constraint error
  {
    header("PureFunc with NC_IsCheck should report a CHECK constraint context");
    testCount++;

    sqlite3_context ctx;
    memset(&ctx, 0, sizeof(ctx));

    VdbeOp op;
    op.opcode = OP_PureFunc;
    op.p5 = NC_IsCheck;

    Vdbe vdbe;
    vdbe.aOp = &op;

    ctx.pVdbe = &vdbe;
    ctx.iOp = 0;
    ctx.pFunc = &fNameMyFunc; // zName = "myFunc"
    ctx.zErr = nullptr;

    int rc = sqlite3NotPureFunc(&ctx);
    CHECK(rc == 0, "Expected return code 0 for pure func with NC_IsCheck");
    CHECK(ctx.zErr != nullptr, "Expected an error message to be set");
    if(rc == 0 && ctx.zErr != nullptr) {
      // Validate that the message contains the function name and the CHECK constraint context
      const char *msg = ctx.zErr;
      bool containsFunc = strstr(msg, "myFunc") != nullptr;
      bool containsCheck = strstr(msg, "CHECK constraint") != nullptr;
      CHECK(containsFunc, "Error message should contain function name");
      CHECK(containsCheck, "Error message should mention CHECK constraint");
      if(containsFunc && containsCheck) passCount++;
    }
  }

  // Test 4: OP_PureFunc with NC_GenCol should report a generated column context
  {
    header("PureFunc with NC_GenCol should report a generated column context");
    testCount++;

    sqlite3_context ctx;
    memset(&ctx, 0, sizeof(ctx));

    VdbeOp op;
    op.opcode = OP_PureFunc;
    op.p5 = NC_GenCol;

    Vdbe vdbe;
    vdbe.aOp = &op;

    ctx.pVdbe = &vdbe;
    ctx.iOp = 0;
    ctx.pFunc = &fNameMyFunc; // zName = "myFunc"
    ctx.zErr = nullptr;

    int rc = sqlite3NotPureFunc(&ctx);
    CHECK(rc == 0, "Expected return code 0 for pure func with NC_GenCol");
    CHECK(ctx.zErr != nullptr, "Expected an error message to be set for GenCol");
    if(rc == 0 && ctx.zErr != nullptr) {
      const char *msg = ctx.zErr;
      bool containsGen = strstr(msg, "generated column") != nullptr;
      bool containsFunc = strstr(msg, "myFunc") != nullptr;
      CHECK(containsGen, "Error message should mention generated column context");
      CHECK(containsFunc, "Error message should mention function name");
      if(containsGen && containsFunc) passCount++;
    }
  }

  // Test 5: OP_PureFunc with no NC_IsCheck or NC_GenCol should report an index context
  {
    header("PureFunc with no NC_IsCheck/NC_GenCol should report an index context");
    testCount++;

    sqlite3_context ctx;
    memset(&ctx, 0, sizeof(ctx));

    VdbeOp op;
    op.opcode = OP_PureFunc;
    op.p5 = 0; // neither NC_IsCheck nor NC_GenCol

    Vdbe vdbe;
    vdbe.aOp = &op;

    ctx.pVdbe = &vdbe;
    ctx.iOp = 0;
    ctx.pFunc = &fNameMyFunc; // zName = "myFunc"
    ctx.zErr = nullptr;

    int rc = sqlite3NotPureFunc(&ctx);
    CHECK(rc == 0, "Expected return code 0 for pure func with no NC_IsCheck/GenCol");
    CHECK(ctx.zErr != nullptr, "Expected an error message to be set for index context");
    if(rc == 0 && ctx.zErr != nullptr) {
      const char *msg = ctx.zErr;
      bool containsIndex = strstr(msg, "an index") != nullptr;
      bool containsFunc  = strstr(msg, "myFunc") != nullptr;
      CHECK(containsIndex, "Error message should mention index context");
      CHECK(containsFunc, "Error message should mention function name");
      if(containsIndex && containsFunc) passCount++;
    }
  }

  // Summary
  printf("Tests run: %d, Passed: %d, Failures: %d\n", testCount, passCount, testCount - passCount);
  return (g_failures > 0) ? 1 : 0;
}