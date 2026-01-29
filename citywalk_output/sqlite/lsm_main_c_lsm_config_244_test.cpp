#include <lsmInt.h>
#include <iostream>
#include <cstring>
#include <cstdint>


// Minimal test harness for C++11 (no GTest). Tests call the focal method lsm_config
// and exercise various branches. All tests are self-contained and print results
// to stdout. The tests rely on the real lsm_config implementation and its
// dependent structures provided by the project.

// Note: The static (file-scope) helpers inside the focal file are not directly
// testable, so tests focus on public behavior exposed by lsm_config and the
// lsm_db state mutations it performs.

// Helper macro for test result reporting
#define TEST_PASS(msg) std::cout << "[PASS] " << msg << std::endl
#define TEST_FAIL(msg) std::cerr << "[FAIL] " << msg << std::endl; return false

// Utility to safely zero-initialize lsm_db for each test
static void init_db(lsm_db &db) {
    std::memset(&db, 0, sizeof(db));
}

// Candidate test 1: LSM_CONFIG_AUTOFLUSH within valid KB bounds.
// - Sets nTreeLimit = iVal * 1024 when iVal in [0, 1024*1024].
// - piVal is updated to (nTreeLimit / 1024).
static bool test_lsm_config_autoflush_within_bounds() {
    lsm_db db;
    init_db(db);
    int val = 256; // 256 KB
    int rc = lsm_config(&db, LSM_CONFIG_AUTOFLUSH, &val);
    if (rc != LSM_OK) {
        TEST_FAIL("AutoFlush within bounds: rc != LSM_OK");
    }
    // Expect nTreeLimit = 256 * 1024
    if (db.nTreeLimit != 256 * 1024) {
        TEST_FAIL("AutoFlush within bounds: nTreeLimit not updated correctly");
    }
    // piVal should be nTreeLimit / 1024 => 256
    if (val != 256) {
        TEST_FAIL("AutoFlush within bounds: piVal not updated to nTreeLimit/1024");
    }
    TEST_PASS("AutoFlush within bounds");
    return true;
}

// Candidate test 2: LSM_CONFIG_AUTOFLUSH out of valid KB bounds (negative input).
static bool test_lsm_config_autoflush_out_of_bounds() {
    lsm_db db;
    init_db(db);
    int val = -1; // invalid
    int rc = lsm_config(&db, LSM_CONFIG_AUTOFLUSH, &val);
    if (rc != LSM_OK) {
        TEST_FAIL("AutoFlush out of bounds: rc != LSM_OK");
    }
    // Should not modify nTreeLimit (still 0)
    if (db.nTreeLimit != 0) {
        TEST_FAIL("AutoFlush out of bounds: nTreeLimit should remain unchanged");
    }
    // piVal should reflect current nTreeLimit / 1024 => 0
    if (val != 0) {
        TEST_FAIL("AutoFlush out of bounds: piVal should reflect nTreeLimit/1024 (0)");
    }
    TEST_PASS("AutoFlush out of bounds");
    return true;
}

// Candidate test 3: LSM_CONFIG_AUTOWORK updates when non-negative and reflects back.
static bool test_lsm_config_autowork_updates_and_readback() {
    lsm_db db;
    init_db(db);
    db.bAutowork = 0;
    int val = 1; // enable autowork
    int rc = lsm_config(&db, LSM_CONFIG_AUTOWORK, &val);
    if (rc != LSM_OK) {
        TEST_FAIL("Autowork: rc != LSM_OK");
    }
    if (db.bAutowork != 1) {
        TEST_FAIL("Autowork: bAutowork not updated to 1");
    }
    if (val != 1) {
        TEST_FAIL("Autowork: piVal should reflect bAutowork (1)");
    }
    TEST_PASS("Autowork updates and readback");
    return true;
}

// Candidate test 4: LSM_CONFIG_AUTOCHECKPOINT updates when non-negative and readback in KB units.
static bool test_lsm_config_autocheckpoint_readback() {
    lsm_db db;
    init_db(db);
    int val = 2; // 2 KB checkpoint
    int rc = lsm_config(&db, LSM_CONFIG_AUTOCHECKPOINT, &val);
    if (rc != LSM_OK) {
        TEST_FAIL("Autocheckpoint: rc != LSM_OK");
    }
    // nAutockpt should be 2 * 1024 bytes
    if (db.nAutockpt != (int64_t)2 * 1024) {
        TEST_FAIL("Autocheckpoint: nAutockpt not updated correctly");
    }
    // piVal should be nAutockpt / 1024
    if (val != 2) {
        TEST_FAIL("Autocheckpoint: piVal should reflect nAutockpt/1024");
    }
    TEST_PASS("Autocheckpoint updates and readback");
    return true;
}

// Candidate test 5: LSM_CONFIG_PAGE_SIZE with pDatabase == 0 (read/write default path).
// - When val is a power of two in [256, 65536], nDfltPgsz is updated.
// - piVal remains unchanged in this path.
static bool test_lsm_config_page_size_open_false_power2_path() {
    lsm_db db;
    init_db(db);
    db.pDatabase = 0;  // ensure this is the read/write path
    db.nDfltPgsz = 4096; // initial default

    int val = 512; // power of two within range
    int rc = lsm_config(&db, LSM_CONFIG_PAGE_SIZE, &val);
    if (rc != LSM_OK) {
        TEST_FAIL("PageSize (read/write): rc != LSM_OK");
    }
    // nDfltPgsz should be updated
    if (db.nDfltPgsz != 512) {
        TEST_FAIL("PageSize (read/write): nDfltPgsz not updated to 512");
    }
    // piVal should remain 512 (as per function's behavior when path matches branch)
    if (val != 512) {
        TEST_FAIL("PageSize (read/write): piVal should remain unchanged (512)");
    }
    TEST_PASS("PageSize read/write path with power-of-two");
    return true;
}

// Candidate test 6: LSM_CONFIG_PAGE_SIZE with non-power-of-two within range (read/write path else branch).
static bool test_lsm_config_page_size_open_false_non_pp2_path() {
    lsm_db db;
    init_db(db);
    db.pDatabase = 0;
    db.nDfltPgsz = 4096;

    int val = 300; // not a power of two
    int rc = lsm_config(&db, LSM_CONFIG_PAGE_SIZE, &val);
    if (rc != LSM_OK) {
        TEST_FAIL("PageSize (read/write non-pp2): rc != LSM_OK");
    }
    // piVal should be updated to current nDfltPgsz
    if (val != 4096) {
        TEST_FAIL("PageSize (read/write non-pp2): piVal should be set to nDfltPgsz (4096)");
    }
    // nDfltPgsz should remain unchanged
    if (db.nDfltPgsz != 4096) {
        TEST_FAIL("PageSize (read/write non-pp2): nDfltPgsz should remain 4096");
    }
    TEST_PASS("PageSize non-power-of-two read/write path");
    return true;
}

// Candidate test 7: LSM_CONFIG_BLOCK_SIZE with pDatabase == 0 (read/write path for block size in KB).
static bool test_lsm_config_block_size_open_false_power2_path() {
    lsm_db db;
    init_db(db);
    db.pDatabase = 0;
    db.nDfltBlksz = 64 * 1024; // 64 KB default for blocks

    int val = 256; // power of two in KB range [64, 65536]
    int rc = lsm_config(&db, LSM_CONFIG_BLOCK_SIZE, &val);
    if (rc != LSM_OK) {
        TEST_FAIL("BlockSize (read/write): rc != LSM_OK");
    }
    // nDfltBlksz should be updated to 256*1024
    if (db.nDfltBlksz != 256 * 1024) {
        TEST_FAIL("BlockSize (read/write): nDfltBlksz not updated to 256*1024");
    }
    // piVal should remain unchanged (256)
    if (val != 256) {
        TEST_FAIL("BlockSize (read/write): piVal should remain 256");
    }
    TEST_PASS("BlockSize read/write with power-of-two");
    return true;
}

// Candidate test 8: LSM_CONFIG_SAFETY within range and readback.
// - When input in [0,2], both pDb.eSafety and piVal updated.
static bool test_lsm_config_safety_within_range() {
    lsm_db db;
    init_db(db);
    int val = 2;
    int rc = lsm_config(&db, LSM_CONFIG_SAFETY, &val);
    if (rc != LSM_OK) {
        TEST_FAIL("Safety within range: rc != LSM_OK");
    }
    if (db.eSafety != 2) {
        TEST_FAIL("Safety within range: eSafety not updated to 2");
    }
    if (val != 2) {
        TEST_FAIL("Safety within range: piVal not updated to eSafety (2)");
    }
    TEST_PASS("Safety within range");
    return true;
}

// Candidate test 9: LSM_CONFIG_SAFETY out-of-range leaves existing value unchanged.
static bool test_lsm_config_safety_out_of_range() {
    lsm_db db;
    init_db(db);
    db.eSafety = 1; // initial safe value
    int val = 9;     // out of allowed range
    int rc = lsm_config(&db, LSM_CONFIG_SAFETY, &val);
    if (rc != LSM_OK) {
        TEST_FAIL("Safety out of range: rc != LSM_OK");
    }
    if (db.eSafety != 1) {
        TEST_FAIL("Safety out of range: eSafety should remain 1");
    }
    if (val != 1) {
        TEST_FAIL("Safety out of range: piVal should reflect eSafety (1)");
    }
    TEST_PASS("Safety out of range leaves value unchanged");
    return true;
}

// Candidate test 10: LSM_CONFIG_USE_LOG enables when trans open count is zero and val is 0/1.
static bool test_lsm_config_use_log_toggle() {
    lsm_db db;
    init_db(db);
    db.nTransOpen = 0;
    int val = 1;
    int rc = lsm_config(&db, LSM_CONFIG_USE_LOG, &val);
    if (rc != LSM_OK) {
        TEST_FAIL("UseLog toggle: rc != LSM_OK");
    }
    if (db.bUseLog != 1) {
        TEST_FAIL("UseLog toggle: bUseLog should be set to 1");
    }
    if (val != 1) {
        TEST_FAIL("UseLog toggle: piVal should reflect bUseLog (1)");
    }
    TEST_PASS("UseLog toggle when no open transactions");
    return true;
}

// Candidate test 11: LSM_CONFIG_AUTOMERGE increments when value > 1 and readback.
static bool test_lsm_config_automerge_updates() {
    lsm_db db;
    init_db(db);
    db.nMerge = 1;
    int val = 3; // > 1 triggers update
    int rc = lsm_config(&db, LSM_CONFIG_AUTOMERGE, &val);
    if (rc != LSM_OK) {
        TEST_FAIL("Automerge: rc != LSM_OK");
    }
    if (db.nMerge != 3) {
        TEST_FAIL("Automerge: nMerge not updated to 3");
    }
    if (val != 3) {
        TEST_FAIL("Automerge: piVal should reflect nMerge (3)");
    }
    TEST_PASS("Automerge updates and readback");
    return true;
}

// Candidate test 12: LSM_CONFIG_MAX_FREELIST updates within valid range.
static bool test_lsm_config_max_freelist_within_range() {
    lsm_db db;
    init_db(db);
    db.nMaxFreelist = 0;
    int val = 4; // within [2, LSM_MAX_FREELIST_ENTRIES]
    int rc = lsm_config(&db, LSM_CONFIG_MAX_FREELIST, &val);
    if (rc != LSM_OK) {
        TEST_FAIL("MaxFreelist: rc != LSM_OK");
    }
    if (db.nMaxFreelist != 4) {
        TEST_FAIL("MaxFreelist: nMaxFreelist not updated to 4");
    }
    if (val != 4) {
        TEST_FAIL("MaxFreelist: piVal should reflect nMaxFreelist (4)");
    }
    TEST_PASS("MaxFreelist within range");
    return true;
}

// Candidate test 13: LSM_CONFIG_READONLY when pDatabase == 0 updates readonly flag.
static bool test_lsm_config_readonly_path() {
    lsm_db db;
    init_db(db);
    db.pDatabase = 0;
    int val = 1;
    int rc = lsm_config(&db, LSM_CONFIG_READONLY, &val);
    if (rc != LSM_OK) {
        TEST_FAIL("Readonly path: rc != LSM_OK");
    }
    if (db.bReadonly != 1) {
        TEST_FAIL("Readonly path: bReadonly not updated to 1");
    }
    if (val != 1) {
        TEST_FAIL("Readonly path: piVal should reflect bReadonly (1)");
    }
    TEST_PASS("Readonly path when no database opened");
    return true;
}

// Main function: execute all tests in sequence
int main() {
    std::cout << "Starting lsm_config unit tests (C++11 harness, no GTest)" << std::endl;

    if (!test_lsm_config_autoflush_within_bounds()) return 1;
    if (!test_lsm_config_autoflush_out_of_bounds()) return 1;
    if (!test_lsm_config_autowork_updates_and_readback()) return 1;
    if (!test_lsm_config_autocheckpoint_readback()) return 1;
    if (!test_lsm_config_page_size_open_false_power2_path()) return 1;
    if (!test_lsm_config_page_size_open_false_non_pp2_path()) return 1;
    if (!test_lsm_config_block_size_open_false_power2_path()) return 1;
    if (!test_lsm_config_safety_within_range()) return 1;
    if (!test_lsm_config_safety_out_of_range()) return 1;
    if (!test_lsm_config_use_log_toggle()) return 1;
    if (!test_lsm_config_automerge_updates()) return 1;
    if (!test_lsm_config_max_freelist_within_range()) return 1;
    if (!test_lsm_config_readonly_path()) return 1;

    std::cout << "All tests completed." << std::endl;
    return 0;
}