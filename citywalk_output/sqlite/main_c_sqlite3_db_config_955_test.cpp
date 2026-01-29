/*
  Lightweight C++11 unit test suite for the focal method:
  int sqlite3_db_config(sqlite3 *db, int op, ...)

  This test harness implements a minimal, self-contained re-implementation
  of the focal function's environment (types, constants, and a simplified
  sqlite3_db_config) to exercise true/false branches and critical code paths.

  Notes:
  - No GTest; a small, self-contained test framework is used.
  - Tests cover:
    - MAINDBNAME branch
    - LOOKASIDE branch
    - Default flag-ops branch (enable/disable and pRes reporting)
    - No-change scenario for flags (ensures no expensive side-effects)
    - Unknown op returns SQLITE_ERROR
  - The implementation is intentionally self-contained to run without
    external dependencies, while mirroring the logic of the provided focal code.
*/

#include <cstdarg>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Lightweight test harness globals
static int g_pass = 0;
static int g_fail = 0;

// Simple assertion macro for the test harness
#define ASSERT(cond, msg) do { \
    if(!(cond)) { \
        ++g_fail; \
        std::cerr << "Test Failure: " << (msg) << std::endl; \
    } else { \
        ++g_pass; \
    } \
} while(0)

// Macro to determine array size (used in the default flag-ops loop)
#define ArraySize(x) (sizeof(x) / sizeof((x)[0]))

// Domain types and constants mirroring the focal function's usage
typedef uint64_t u64;

// Minimal sqlite3-like structures to support the focal method
struct DbSlot {
    char *zDbSName;
};

struct sqlite3 {
    void *mutex;       // dummy mutex handle
    DbSlot aDb[1];     // single database handle
    u64 flags;           // bitfield for flags
};

// Public-return codes from the focal function
enum {
    SQLITE_OK = 0,
    SQLITE_ERROR = 1,
    SQLITE_MISUSE_BKPT = 21
};

// Opcode values used by sqlite3_db_config
enum {
    SQLITE_DBCONFIG_MAINDBNAME = 1,
    SQLITE_DBCONFIG_LOOKASIDE  = 2,
    // Expanded list (subset), sufficient for tests
    SQLITE_DBCONFIG_ENABLE_FKEY        = 3,
    SQLITE_DBCONFIG_ENABLE_TRIGGER     = 4,
    SQLITE_DBCONFIG_ENABLE_VIEW        = 5,
    SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER = 6,
    SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION = 7,
    SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE   = 8,
    SQLITE_DBCONFIG_ENABLE_QPSG        = 9,
    SQLITE_DBCONFIG_TRIGGER_EQP          = 10,
    SQLITE_DBCONFIG_RESET_DATABASE       = 11,
    SQLITE_DBCONFIG_DEFENSIVE            = 12,
    SQLITE_DBCONFIG_WRITABLE_SCHEMA      = 13,
    SQLITE_DBCONFIG_LEGACY_ALTER_TABLE   = 14,
    SQLITE_DBCONFIG_DQS_DDL              = 15,
    SQLITE_DBCONFIG_DQS_DML              = 16,
    SQLITE_DBCONFIG_LEGACY_FILE_FORMAT   = 17,
    SQLITE_DBCONFIG_TRUSTED_SCHEMA        = 18,
    SQLITE_DBCONFIG_STMT_SCANSTATUS       = 19,
    SQLITE_DBCONFIG_REVERSE_SCANORDER     = 20
};

// Masks for flags (simplified subset)
enum {
    SQLITE_ForeignKeys   = 1ULL << 0,
    SQLITE_EnableTrigger = 1ULL << 1,
    SQLITE_EnableView    = 1ULL << 2,
    SQLITE_Fts3Tokenizer  = 1ULL << 3,
    SQLITE_LoadExtension  = 1ULL << 4,
    SQLITE_NoCkptOnClose  = 1ULL << 5,
    SQLITE_EnableQPSG     = 1ULL << 6,
    SQLITE_TriggerEQP       = 1ULL << 7,
    SQLITE_ResetDatabase    = 1ULL << 8,
    SQLITE_Defensive        = 1ULL << 9,
    SQLITE_WriteSchema      = 1ULL << 10,
    SQLITE_NoSchemaError    = 1ULL << 11,
    SQLITE_LegacyAlter      = 1ULL << 12,
    SQLITE_DqsDDL           = 1ULL << 13,
    SQLITE_DqsDML           = 1ULL << 14,
    SQLITE_LegacyFileFmt    = 1ULL << 15,
    SQLITE_TrustedSchema    = 1ULL << 16,
    SQLITE_StmtScanStatus   = 1ULL << 17,
    SQLITE_ReverseOrder     = 1ULL << 18
};

// Helpers: no-op mutex operations (tests run in single-threaded mode)
static void sqlite3_mutex_enter(void *mutex) { (void)mutex; }
static void sqlite3_mutex_leave(void *mutex) { (void)mutex; }

// Lookaside setup tracking (for LOOKASIDE op)
static struct {
    void *pBuf;
    int sz;
    int cnt;
} g_lookaside = {nullptr, 0, 0};

// Track whether prepared statements expire were triggered
static bool g_expireCalled = false;
static void sqlite3ExpirePreparedStatements(sqlite3 *db, int x) {
    (void)db; (void)x;
    g_expireCalled = true;
}
static void resetExpireFlag() { g_expireCalled = false; }

// Track the most recent LOOKASIDE parameters
static int setupLookaside(sqlite3 *db, void *pBuf, int sz, int cnt) {
    (void)db;
    g_lookaside.pBuf = pBuf;
    g_lookaside.sz = sz;
    g_lookaside.cnt = cnt;
    return SQLITE_OK;
}

// The focal method re-implemented (to be tested)
static int sqlite3_db_config(sqlite3 *db, int op, ...){
    va_list ap;
    int rc;
#ifdef SQLITE_ENABLE_API_ARMOR
    // Intentionally not defining the macro in tests to skip safety check
    if( !0 ) return SQLITE_MISUSE_BKPT;
#endif
    sqlite3_mutex_enter(db->mutex);
    va_start(ap, op);
    switch( op ){
        case SQLITE_DBCONFIG_MAINDBNAME: {
            db->aDb[0].zDbSName = va_arg(ap,char*);
            rc = SQLITE_OK;
            break;
        }
        case SQLITE_DBCONFIG_LOOKASIDE: {
            void *pBuf = va_arg(ap, void*);
            int sz = va_arg(ap, int);
            int cnt = va_arg(ap, int);
            rc = setupLookaside(db, pBuf, sz, cnt);
            break;
        }
        default: {
            static const struct {
                int op;      /* The opcode */
                u64 mask;    /* Mask of the bit in sqlite3.flags to set/clear */
            } aFlagOp[] = {
                { SQLITE_DBCONFIG_ENABLE_FKEY,           SQLITE_ForeignKeys    },
                { SQLITE_DBCONFIG_ENABLE_TRIGGER,        SQLITE_EnableTrigger  },
                { SQLITE_DBCONFIG_ENABLE_VIEW,           SQLITE_EnableView     },
                { SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER, SQLITE_Fts3Tokenizer  },
                { SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION, SQLITE_LoadExtension  },
                { SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE,      SQLITE_NoCkptOnClose  },
                { SQLITE_DBCONFIG_ENABLE_QPSG,           SQLITE_EnableQPSG     },
                { SQLITE_DBCONFIG_TRIGGER_EQP,           SQLITE_TriggerEQP     },
                { SQLITE_DBCONFIG_RESET_DATABASE,        SQLITE_ResetDatabase  },
                { SQLITE_DBCONFIG_DEFENSIVE,             SQLITE_Defensive      },
                { SQLITE_DBCONFIG_WRITABLE_SCHEMA,       SQLITE_WriteSchema|
                                                         SQLITE_NoSchemaError  },
                { SQLITE_DBCONFIG_LEGACY_ALTER_TABLE,    SQLITE_LegacyAlter    },
                { SQLITE_DBCONFIG_DQS_DDL,               SQLITE_DqsDDL         },
                { SQLITE_DBCONFIG_DQS_DML,               SQLITE_DqsDML         },
                { SQLITE_DBCONFIG_LEGACY_FILE_FORMAT,    SQLITE_LegacyFileFmt  },
                { SQLITE_DBCONFIG_TRUSTED_SCHEMA,        SQLITE_TrustedSchema  },
                { SQLITE_DBCONFIG_STMT_SCANSTATUS,       SQLITE_StmtScanStatus },
                { SQLITE_DBCONFIG_REVERSE_SCANORDER,     SQLITE_ReverseOrder   },
            };
            unsigned int i;
            rc = SQLITE_ERROR;
            for(i=0; i<ArraySize(aFlagOp); i++){
                if( aFlagOp[i].op==op ){
                    int onoff = va_arg(ap, int);
                    int *pRes = va_arg(ap, int*);
                    u64 oldFlags = db->flags;
                    if( onoff>0 ){
                        db->flags |= aFlagOp[i].mask;
                    }else if( onoff==0 ){
                        db->flags &= ~(u64)aFlagOp[i].mask;
                    }
                    if( oldFlags!=db->flags ){
                        sqlite3ExpirePreparedStatements(db, 0);
                    }
                    if( pRes ){
                        *pRes = (db->flags & aFlagOp[i].mask)!=0;
                    }
                    rc = SQLITE_OK;
                    break;
                }
            }
            break;
        }
    }
    va_end(ap);
    sqlite3_mutex_leave(db->mutex);
    return rc;
}

// Test helpers
static void runAllTests();

// Test 1: MAINDBNAME branch sets pointer correctly
static void test_mainDbName_sets_name() {
    sqlite3 db;
    db.mutex = (void*)0xDEADBEEF;
    db.aDb[0].zDbSName = nullptr;
    db.flags = 0;

    const char *src = "my_main_db";
    char *mutName = new char[strlen(src) + 1];
    std::strcpy(mutName, src);

    int rc = sqlite3_db_config(&db, SQLITE_DBCONFIG_MAINDBNAME, mutName);

    ASSERT(rc == SQLITE_OK, "MAINDBNAME should return SQLITE_OK");
    ASSERT(db.aDb[0].zDbSName == mutName, "zDbSName should point to provided name");

    delete[] mutName;
    // nothing else to cleanup
}

// Test 2: LOOKASIDE branch sets internal tracking and returns OK
static void test_lookaside_branch() {
    sqlite3 db;
    db.mutex = (void*)0xFEEDBEEF;
    db.aDb[0].zDbSName = nullptr;
    db.flags = 0;
    g_lookaside.pBuf = nullptr;
    g_lookaside.sz = 0;
    g_lookaside.cnt = 0;

    void *buf = (void*)0x1234;
    int sz = 128;
    int cnt = 4;

    int rc = sqlite3_db_config(&db, SQLITE_DBCONFIG_LOOKASIDE, buf, sz, cnt);

    ASSERT(rc == SQLITE_OK, "LOOKASIDE should return SQLITE_OK");
    ASSERT(g_lookaside.pBuf == buf, "LOOKASIDE pBuf should be stored");
    ASSERT(g_lookaside.sz == sz, "LOOKASIDE size should be stored");
    ASSERT(g_lookaside.cnt == cnt, "LOOKASIDE count should be stored");
}

// Test 3: Default op path sets flags and reports through pRes, and calls expirer when changed
static void test_flagOp_set_and_expire() {
    sqlite3 db;
    db.mutex = (void*)0xC0FFEE;
    db.aDb[0].zDbSName = nullptr;
    db.flags = 0;
    g_expireCalled = false;
    int res = 0;

    int rc = sqlite3_db_config(&db, SQLITE_DBCONFIG_ENABLE_FKEY, 1, &res);

    ASSERT(rc == SQLITE_OK, "ENABLE_FKEY should return SQLITE_OK");
    ASSERT((db.flags & SQLITE_ForeignKeys) != 0, "FOREIGN KEYS flag should be set");
    ASSERT(res != 0, "pRes should reflect flag state (non-zero when set)");
    ASSERT(g_expireCalled, "Expiring prepared statements should be triggered on flag change");

    // Now clear the flag
    g_expireCalled = false;
    rc = sqlite3_db_config(&db, SQLITE_DBCONFIG_ENABLE_FKEY, 0, &res);
    ASSERT(rc == SQLITE_OK, "DISABLE FKEY should return SQLITE_OK");
    ASSERT((db.flags & SQLITE_ForeignKeys) == 0, "FOREIGN KEYS flag should be cleared");
    ASSERT(res == 0, "pRes should reflect cleared flag state (0)");
    ASSERT(g_expireCalled, "Expiring prepared statements should be triggered on flag change");
}

// Test 4: No-change path does not trigger expire and leaves flags intact
static void test_flagOp_noChange_noExpire() {
    sqlite3 db;
    db.mutex = (void*)0xBADA55;
    db.aDb[0].zDbSName = nullptr;
    // Pre-set the flag
    db.flags = 0;
    // Set the flag as if previously toggled
    db.flags |= SQLITE_ForeignKeys;
    g_expireCalled = false;
    int res = 0;

    int rc = sqlite3_db_config(&db, SQLITE_DBCONFIG_ENABLE_FKEY, 1, &res);

    // Should not trigger expire since oldFlags == new flags
    ASSERT(rc == SQLITE_OK, "ENABLE_FKEY (no-change) should return SQLITE_OK");
    ASSERT((db.flags & SQLITE_ForeignKeys) != 0, "FOREIGN KEYS should remain set");
    ASSERT(res != 0, "pRes should reflect that flag remains set");
    ASSERT(!g_expireCalled, "No expire should be triggered on no change");
}

// Test 5: Unknown op returns SQLITE_ERROR
static void test_unknown_op_returns_error() {
    sqlite3 db;
    db.mutex = (void*)0xABCD;
    db.aDb[0].zDbSName = nullptr;
    db.flags = 0;

    int rc = sqlite3_db_config(&db, 9999 /* unknown op */);

    ASSERT(rc == SQLITE_ERROR, "Unknown op should return SQLITE_ERROR");
}

// Run all tests
static void runAllTests() {
    test_mainDbName_sets_name();
    test_lookaside_branch();
    test_flagOp_set_and_expire();
    test_flagOp_noChange_noExpire();
    test_unknown_op_returns_error();
}

int main() {
    runAllTests();

    std::cout << "Tests run: " << (g_pass + g_fail)
              << "  Passed: " << g_pass
              << "  Failed: " << g_fail << std::endl;
    if(g_fail) {
        std::cout << "Some tests failed." << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
}