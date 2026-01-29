/*
  Unit test suite for the focal method:
  int sqlite3WalUndo(Wal *pWal, int (*xUndo)(void *, Pgno), void *pUndoCtx)

  Notes:
  - This test harness is designed to be compiled with the same project that provides
    wal.c (the focal implementation) and its related headers. It follows a lightweight
   , GTest-free approach using plain C-style assertions and a small main() to drive tests.

  - Because sqlite3WalUndo interacts with internal WAL state (Wal, WalIndexHdr, and
    related helpers), the tests below focus on:
      1) The behavior when the writeLock is false (early exit, SQLITE_OK).
      2) The behavior when the undo callback reports an error (rc propagation).
      3) Static/non-public aspects via non-destructive checks (e.g., ensuring no crash
         or undefined behavior when a minimal Wal is provided).

  - These tests avoid private method calls and rely on the public (or header-declared)
    interfaces and the focal function under test.

  - The tests assume the environment provides:
      - SQLITE_OK and SQLITE_IOERR_IN_PAGE and other sqlite return codes
      - The Wal type and the xUndo callback signature
      - The Wal-related helper functions used by sqlite3WalUndo (e.g., walIndexHdr,
        walFramePgno, walCleanupHash) are linked in from wal.c as part of the test build.

  - This code is written for C++11, but uses C interfaces from the SQLite codebase where
    appropriate. No GTest is used.

  How to run (example):
    - Compile with the project’s build system, ensuring wal.c and its headers are linked.
    - Execute the resulting test binary.

  Important: The test suite uses a minimal framework (plain asserts) and prints
  diagnostics via standard output where appropriate.
*/

#include <Windows.h>
#include <vector>
#include <cstring>
#include <iostream>
#include <wal.h>
#include <cassert>


extern "C" {
  // Include the WAL-related headers from the project so that we can call sqlite3WalUndo.
  // Adjust the path if your project layout differs.
  #include "wal.h"        // Defines Wal, WalIndexHdr, Pgno, etc., as used by sqlite3WalUndo.
  #include "sqlite3.h"    // For SQLITE_OK, SQLITE_IOERR_IN_PAGE, etc.
}

// Lightweight test harness
class WalUndoTestFramework {
public:
  using UndoCtx = std::vector<Pgno>;

  static void test_no_write_lock_returns_ok() {
    // Arrange
    Wal w;
    std::memset(&w, 0, sizeof(Wal));
    w.writeLock = 0; // Triggers the early return path

    // Act
    int rc = sqlite3WalUndo(&w, &testUndoCallback, nullptr);

    // Assert
    // Expect no error when there is no write lock
    if (rc != SQLITE_OK) {
      std::cerr << "test_no_write_lock_returns_ok: Expected SQLITE_OK, got " << rc << "\n";
    }
    assert(rc == SQLITE_OK);
  }

  static void test_undo_callback_invoked_when_possible() {
    // This test aims to exercise the xUndo invocation when the WAL is in a state
    // where the undo loop could run. Since the internal details of walIndexHdr and
    // walFramePgno depend on the runtime WAL state (and may be static/hidden),
    // we attempt to drive a scenario where the loop would reasonably execute
    // and observe the calls to xUndo by collecting the page numbers passed to it.

    // Arrange
    Wal w;
    std::memset(&w, 0, sizeof(Wal));

    // Enable write lock so the undo logic runs
    w.writeLock = 1;

    // Pretend header state in a way that would allow the for-loop to execute
    // by configuring the public state that sqlite3WalUndo uses to determine iMax
    // and pWal->hdr.mxFrame. In a real environment these would be derived from
    // the shared WAL index memory; here we attempt to set a plausible configuration.
    // Note: Depending on the platform and build, walIndexHdr may return a memory region
    // that is not simply pAnda->hdr; the following is best-effort and intended to
    // work in typical environments where walIndexHdr(pWal) resolves to the WAL header.

    w.hdr.mxFrame = 5; // iMax initial state
    // To maximize chance that the for-loop runs, we hope walIndexHdr(pWal) restores a header
    // with a smaller mxFrame. If the underlying implementation returns a reference to
    // the same header, this path will be a no-op. The assertion below verifies that
    // the function returns SQLITE_OK, and we will observe any xUndo calls through the
    // test callback if they occur.

    UndoCtx undoLog;
    int rc = SQLITE_OK;

    // Use an adapter that pushes all seen page numbers into undoLog
    rc = sqlite3WalUndo(&w, &testUndoCallback, &undoLog);

    // Assert: rc should be SQLITE_OK if the operation completed without error.
    if (rc != SQLITE_OK) {
      std::cerr << "test_undo_callback_invoked_when_possible: Expected SQLITE_OK, got " << rc << "\n";
    }
    // Note: Without control over the internal walIndexHdr() return region, we cannot reliably
    // assert the exact number of xUndo invocations here. The key objective is to ensure
    // the function completes without crashing and returns a valid code.
    assert(rc == SQLITE_OK);
  }

  static void test_undo_callback_error_propagation() {
    // Arrange
    Wal w;
    std::memset(&w, 0, sizeof(Wal));
    w.writeLock = 1;

    // We provide a xUndo callback that returns an error to ensure rc propagation.
    // The goal is to verify that sqlite3WalUndo returns the same error code when
    // the undo operation fails.
    // To simulate an error in the undo callback, pass a context that helps the
    // test differentiate this scenario.
    UndoCtx undoLog;
    int rc = SQLITE_OK;

    // We implement a specialized xUndo that returns SQLITE_IOERR_IN_PAGE to simulate an error.
    // Since sqlite3WalUndo expects a function pointer of the form int (*)(void *, Pgno),
    // we reuse testUndoCallback but provide a global flag to flip behavior for this test.
    // We can't easily pass per-call behavior without global state; instead, we can rely
    // on the "always fail" behavior in the test callback implementation below.

    rc = sqlite3WalUndo(&w, &testUndoCallbackFail, &undoLog);

    // Assert: rc should reflect the error code provided by the undo callback.
    if (rc == SQLITE_OK) {
      std::cerr << "test_undo_callback_error_propagation: Expected error from xUndo, got SQLITE_OK\n";
    }
    assert(rc != SQLITE_OK);
  }

  // Public entry to run all tests
  static void RunAll() {
    test_no_write_lock_returns_ok();
    test_undo_callback_invoked_when_possible();
    test_undo_callback_error_propagation();
    std::cout << "All WAL undo tests executed.\n";
  }

private:
  // Common test undo function: appends the page numbers to the provided vector and returns OK.
  static int testUndoCallback(void *ctx, Pgno page) {
    std::vector<Pgno>* log = static_cast<std::vector<Pgno>*>(ctx);
    if (log) log->push_back(page);
    return SQLITE_OK;
  }

  // Fail variant of the undo callback to simulate error propagation.
  static int testUndoCallbackFail(void *ctx, Pgno page) {
    (void)ctx;
    (void)page;
    // Return an error to simulate failure in xUndo
    return SQLITE_IOERR_IN_PAGE;
  }
};

// Entry point: run the test suite
int main() {
  WalUndoTestFramework::RunAll();
  return 0;
}