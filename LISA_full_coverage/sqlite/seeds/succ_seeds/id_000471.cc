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
//<ID> 471
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3_mutex *mtx = NULL;
    sqlite3_vfs *vfs = NULL;
    sqlite3_filename fname = (sqlite3_filename)0;
    const char *uri_key = NULL;
    char buf[64];
    char *msg = NULL;
    sqlite3_int64 used_before = 0;
    sqlite3_int64 highwater = 0;
    sqlite3_int64 soft_limit_res = 0;
    int opt_used = 0;

    /* Query memory usage and highwater marks */
    used_before = sqlite3_memory_used();
    highwater = sqlite3_memory_highwater(0);

    /* Adjust soft heap limit relative to current usage */
    soft_limit_res = sqlite3_soft_heap_limit64(used_before + 1024);

    /* Check a compile-time option (string literal is safe) */
    opt_used = sqlite3_compileoption_used("ENABLE_FTS3");

    /* Lookup the default VFS */
    vfs = sqlite3_vfs_find((const char *)0);

    /* Create a filename object and query a URI key from it */
    fname = sqlite3_create_filename("test.db", "-journal", "-wal", 0, (const char **)0);
    uri_key = sqlite3_uri_key(fname, 0);

    /* Produce a small formatted string (use simple constant per provided signature) */
    msg = sqlite3_mprintf("ok");

    /* Use the snprintf variant from the API list to write into a buffer */
    sqlite3_snprintf((int)sizeof(buf), buf, "ok");

    /* Allocate and enter a mutex */
    mtx = sqlite3_mutex_alloc(0);
    sqlite3_mutex_enter(mtx);

    /* Silence unused-variable warnings for the harness */
    (void)mtx;
    (void)vfs;
    (void)fname;
    (void)uri_key;
    (void)msg;
    (void)used_before;
    (void)highwater;
    (void)soft_limit_res;
    (void)opt_used;
    (void)buf;

    return 66;
}