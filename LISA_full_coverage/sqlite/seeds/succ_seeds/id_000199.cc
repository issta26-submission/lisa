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
//<ID> 199
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // Step 1: Initialize library and basic queries
    sqlite3 *db = NULL;
    int rc_init = sqlite3_initialize();
    int kw_count = sqlite3_keyword_count();
    int like_res = sqlite3_strlike("a%", "abc", 0);
    int glob_res = sqlite3_strglob("*.c", "file.c");

    // Step 2: Mutex operations (allocation, enter/leave/try/notheld)
    sqlite3_mutex *mtx = sqlite3_mutex_alloc(0);
    sqlite3_mutex_enter(mtx);
    int mtx_notheld = sqlite3_mutex_notheld(mtx);
    sqlite3_mutex_leave(mtx);
    int mtx_try = sqlite3_mutex_try(mtx);

    // Step 3: Memory allocation and size query
    void *mem = sqlite3_malloc(128);
    sqlite3_uint64 mem_size = sqlite3_msize(mem);

    // Step 4: Misc utilities
    int compile_used = sqlite3_compileoption_used("THREADSAFE");
    const char *errstr = sqlite3_errstr(1);
    int global_recover = sqlite3_global_recover();
    int enable_shared = sqlite3_enable_shared_cache(0);

    // Step 5: Shutdown and close (db is NULL as no open API used here)
    int rc_shutdown = sqlite3_shutdown();
    int rc_close = sqlite3_close(db);

    // Silence unused-variable warnings
    (void)rc_init;
    (void)kw_count;
    (void)like_res;
    (void)glob_res;
    (void)mtx;
    (void)mtx_notheld;
    (void)mtx_try;
    (void)mem;
    (void)mem_size;
    (void)compile_used;
    (void)errstr;
    (void)global_recover;
    (void)enable_shared;
    (void)rc_shutdown;
    (void)rc_close;

    return 66;
}