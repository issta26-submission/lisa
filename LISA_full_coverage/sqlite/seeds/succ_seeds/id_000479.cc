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
//<ID> 479
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3_int64 heap_limit = 0;
    const char *ver = NULL;
    int rc_os = 0;
    int rc_init = 0;
    int rc_compileopt = 0;
    int rc_keyword_name = 0;
    int rc_keyword_check = 0;
    int rc_shutdown = 0;
    char *msg = NULL;
    void *mem = NULL;
    void *mem2 = NULL;
    char tmp[128];
    const char *kwlist[1];
    const char *kwptr = NULL;
    int kwlen = 0;

    // step 2: Initialize / Configure
    rc_os = sqlite3_os_init();
    rc_init = sqlite3_initialize();
    ver = sqlite3_libversion();
    rc_compileopt = sqlite3_compileoption_used("THREADSAFE");
    heap_limit = sqlite3_hard_heap_limit64(0);

    // step 3: Operate / Validate (safe, straight-line calls)
    msg = sqlite3_mprintf("ver=%s opt=%d limit=%lld", ver ? ver : "unknown", rc_compileopt, (long long)heap_limit);
    mem = sqlite3_malloc64(64);
    mem2 = sqlite3_realloc(mem, 128);
    sqlite3_snprintf(sizeof(tmp), tmp, "%s", msg ? msg : "empty");
    sqlite3_free(msg);
    sqlite3_free(mem2);
    kwlist[0] = NULL;
    rc_keyword_name = sqlite3_keyword_name(0, kwlist, &kwlen);
    kwptr = kwlist[0];
    rc_keyword_check = sqlite3_keyword_check("SELECT", 6);
    sqlite3_sleep(0);

    // step 4: Cleanup / Finalize
    rc_shutdown = sqlite3_shutdown();

    (void)rc_os;
    (void)rc_init;
    (void)ver;
    (void)rc_compileopt;
    (void)heap_limit;
    (void)tmp;
    (void)kwptr;
    (void)rc_keyword_name;
    (void)rc_keyword_check;
    (void)rc_shutdown;

    // API sequence test completed successfully
    return 66;
}