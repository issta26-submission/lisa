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
//<ID> 493
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_str *str_builder = NULL;
    sqlite3_context *ctx = NULL;
    const char *src = NULL;
    const char *opt0 = NULL;
    char *msg = NULL;
    size_t msg_size = 0;
    int kw_count = 0;
    int rc_os_init = 0;
    int rc_global = 0;

    src = sqlite3_sourceid();
    opt0 = sqlite3_compileoption_get(0);
    kw_count = sqlite3_keyword_count();
    msg = sqlite3_mprintf("sourceid=%s opt0=%s keywords=%d", src ? src : "NULL", opt0 ? opt0 : "NULL", kw_count);
    msg_size = (size_t)sqlite3_msize((void *)msg);
    sqlite3_free(msg);
    sqlite3_thread_cleanup();
    rc_os_init = sqlite3_os_init();
    rc_global = sqlite3_global_recover();

    (void)db;
    (void)stmt;
    (void)str_builder;
    (void)ctx;
    (void)msg_size;
    (void)rc_os_init;
    (void)rc_global;
    (void)src;
    (void)opt0;
    (void)kw_count;

    return 66;
}