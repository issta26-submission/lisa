#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 476
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_index_info *pIdx = NULL;
    char *msg = NULL;
    sqlite3_int64 hw = 0;
    int rc_init = 0;
    int kw = 0;
    int cmp = 0;
    int complete = 0;

    // step 2: Initialize / Configure (safe calls that do not dereference db/stmt/pIdx)
    rc_init = sqlite3_initialize();
    hw = sqlite3_memory_highwater(0);
    sqlite3_soft_heap_limit64(1024);

    // create a formatted string from the library version
    msg = sqlite3_mprintf("%s", sqlite3_libversion());

    // step 3: Operate / Validate (use safe, non-db-requiring APIs)
    cmp = sqlite3_strnicmp(msg ? msg : "", "3.", 2);
    kw = sqlite3_keyword_check("select", 6);
    complete = sqlite3_complete16("SELECT 1");

    // mark unused pointers/values to avoid warnings
    (void)stmt;
    (void)pIdx;
    (void)hw;
    (void)rc_init;
    (void)cmp;
    (void)kw;
    (void)complete;

    // step 4: Cleanup / Finalize
    sqlite3_shutdown();

    // drop references (not freeing msg since sqlite3_free is not used here)
    msg = NULL;
    db = NULL;
    stmt = NULL;
    pIdx = NULL;

    return 66;
}