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
//<ID> 589
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_str *builder = NULL;
    char *sql = NULL;
    void *mem = NULL;
    sqlite3_context *ctx = NULL;
    int rc_open = 0;
    int complete = 0;
    int threadsafe = 0;
    int rc_close = 0;
    const char *errmsg = NULL;

    rc_open = sqlite3_open(":memory:", &db);

    // step 2: Configure
    builder = sqlite3_str_new(db);
    sqlite3_str_appendall(builder, "CREATE TABLE t(id INTEGER);");
    sql = sqlite3_str_finish(builder);
    builder = NULL;

    // step 3: Operate / Validate
    complete = sqlite3_complete(sql);
    threadsafe = sqlite3_threadsafe();
    mem = sqlite3_realloc(NULL, 128);
    sqlite3_result_null(ctx);
    errmsg = sqlite3_errmsg(db);

    // step 4: Cleanup
    sqlite3_realloc(mem, 0);
    sqlite3_realloc(sql, 0);
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)complete;
    (void)threadsafe;
    (void)errmsg;
    (void)rc_close;
    (void)ctx;
    (void)builder;

    // API sequence test completed successfully
    return 66;
}