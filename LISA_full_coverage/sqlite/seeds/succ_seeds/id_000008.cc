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
//<ID> 8
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
static int my_busy_handler(void *p, int retry){ (void)p; (void)retry; return 0; }

int test_sqlite3_api_sequence() {
    sqlite3 *db = 0;
    sqlite3_stmt *stmt = 0;
    sqlite3_str *str = 0;
    char *expanded = 0;
    int rc_open = 0;
    int rc_busy = 0;
    int rc_prep = 0;
    int data_count = 0;
    int str_err = 0;

    rc_open = sqlite3_open_v2(":memory:", &db, 0, 0);
    rc_busy = sqlite3_busy_handler(db, my_busy_handler, 0);
    rc_prep = sqlite3_prepare16(db, (const void*)u"SELECT 1", -1, &stmt, 0);
    expanded = sqlite3_expanded_sql(stmt);
    data_count = sqlite3_data_count(stmt);
    str = sqlite3_str_new(db);
    sqlite3_str_appendall(str, expanded);
    sqlite3_str_append(str, "!", 1);
    str_err = sqlite3_str_errcode(str);
    sqlite3_str_reset(str);
    sqlite3_free(expanded);
    sqlite3_reset(stmt);
    sqlite3_interrupt(db);
    sqlite3_thread_cleanup();
    return 66;
}