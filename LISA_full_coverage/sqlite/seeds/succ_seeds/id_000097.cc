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
//<ID> 97
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *pzTail = NULL;
    int rc_open = 0;
    int rc_create_func = 0;
    int rc_exec_create = 0;
    int rc_exec_insert = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    sqlite3_int64 extracted_value = 0;
    const char *errstr = NULL;

    // user-defined scalar function (captureless lambda convertible to function pointer)
    auto udf = [] (sqlite3_context *ctx, int argc, sqlite3_value **argv) -> void {
        sqlite3_int64 v = sqlite3_value_int64(argv[0]);
        sqlite3_result_int(ctx, (int)(v & 0x7FFFFFFF));
    };
    void (*udf_ptr)(sqlite3_context *, int, sqlite3_value **) = udf;

    // progress handler (captureless lambda)
    auto prog = [] (void *p) -> int {
        return 0;
    };
    int (*prog_ptr)(void *) = prog;

    // step 2: Configure
    rc_open = sqlite3_open(":memory:", &db);
    rc_create_func = sqlite3_create_function(db, "get_low", 1, 1, NULL, udf_ptr, NULL, NULL);
    sqlite3_progress_handler(db, 1000, prog_ptr, NULL);

    // step 3: Operate / Validate
    rc_exec_create = sqlite3_exec(db, "CREATE TABLE t(x INTEGER);", NULL, NULL, NULL);
    rc_exec_insert = sqlite3_exec(db, "INSERT INTO t VALUES(9223372036854775807);", NULL, NULL, NULL);
    rc_prep = sqlite3_prepare_v3(db, "SELECT get_low(x) FROM t;", -1, 0, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    extracted_value = sqlite3_column_int64(stmt, 0);
    errstr = sqlite3_errstr(rc_step);

    // step 4: Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    sqlite3_progress_handler(db, 0, NULL, NULL);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_create_func;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)rc_prep;
    (void)rc_step;
    (void)extracted_value;
    (void)errstr;
    (void)rc_finalize;
    (void)rc_close;
    (void)pzTail;
    (void)udf_ptr;
    (void)prog_ptr;
    (void)stmt;
    (void)db;

    // API sequence test completed successfully
    return 66;
}