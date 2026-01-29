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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = 0;
    sqlite3_stmt *stmt = 0;
    sqlite3_value *val = 0;
    void *mem = 0;
    int ver = 0;
    int cmp = 0;
    int opt = 0;
    sqlite3_int64 used1 = 0;
    sqlite3_int64 used2 = 0;
    sqlite3_int64 prev_limit = 0;

    // step 2: Initialize / Configure (use only APIs that do not require a valid sqlite3*)
    ver = sqlite3_libversion_number();
    used1 = sqlite3_memory_used();
    prev_limit = sqlite3_hard_heap_limit64((sqlite3_int64)1024);
    sqlite3_status64(0, &used1, &used2, 0);

    // step 3: Operate / Validate (allocate and free memory via sqlite3 APIs; string and option checks)
    mem = sqlite3_malloc(128);
    sqlite3_free(mem);
    cmp = sqlite3_strnicmp("abc", "Abc", 2);
    opt = sqlite3_compileoption_used("ENABLE_FOO");
    sqlite3_reset_auto_extension();
    sqlite3_shutdown();

    // step 4: Cleanup (no DB opened, nothing to close)
    (void)db;
    (void)stmt;
    (void)val;
    (void)ver;
    (void)cmp;
    (void)opt;
    (void)used1;
    (void)used2;
    (void)prev_limit;

    return 66;
}