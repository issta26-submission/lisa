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
//<ID> 514
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
    const char *sql = "SELECT 1";
    char *expanded = NULL;
    void *mem = NULL;
    char *memc = NULL;
    int rc_init = 0;
    sqlite3_int64 mem_used = 0;
    sqlite3_int64 mem_high = 0;

    // step 2: Initialize and allocate memory
    rc_init = sqlite3_initialize();
    mem = sqlite3_malloc64((sqlite3_uint64)16);
    memc = (char *)mem;
    memc[0] = sql[0];

    // step 3: Query memory metrics and allocate another buffer
    mem_used = sqlite3_memory_used();
    mem_high = sqlite3_memory_highwater(0);
    expanded = (char *)sqlite3_malloc64((sqlite3_uint64)8);
    expanded[0] = sql[1];

    // step 4: Reallocate to size 0 (free), hint the library to release memory, shutdown
    mem = sqlite3_realloc64(mem, (sqlite3_uint64)0);
    expanded = (char *)sqlite3_realloc64(expanded, (sqlite3_uint64)0);
    sqlite3_release_memory(0);
    sqlite3_shutdown();

    // silence unused-variable warnings
    (void)db;
    (void)stmt;
    (void)rc_init;
    (void)mem_used;
    (void)mem_high;
    (void)memc;
    (void)expanded;
    (void)sql;

    // API sequence test completed successfully
    return 66;
}