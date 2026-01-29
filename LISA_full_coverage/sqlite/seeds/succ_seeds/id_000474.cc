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
//<ID> 474
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char **result = NULL;
    char *errmsg = NULL;
    const void *filename = (const void *)L":memory:";
    const char *sql = "EXPLAIN SELECT 1;";
    unsigned char *serialized = NULL;
    sqlite3_int64 serialized_size = 0;
    sqlite3_value *val = NULL;
    sqlite3_value *dup_val = NULL;
    int rows = 0;
    int columns = 0;
    int rc_open = 0;
    int rc_get = 0;
    int rc_prepare = 0;
    int colcount = 0;
    int datacount = 0;
    int errcode = 0;
    int rc_close = 0;

    // API sequence (straight-line, no branches or loops)
    rc_open = sqlite3_open16(filename, &db);
    rc_get = sqlite3_get_table(db, sql, &result, &rows, &columns, &errmsg);
    serialized = sqlite3_serialize(db, "main", &serialized_size, 0);
    rc_prepare = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    colcount = sqlite3_column_count(stmt);
    datacount = sqlite3_data_count(stmt);
    val = sqlite3_column_value(stmt, 0);
    dup_val = sqlite3_value_dup(val);
    errcode = sqlite3_errcode(db);
    sqlite3_value_free(dup_val);
    sqlite3_clear_bindings(stmt);
    sqlite3_reset(stmt);
    sqlite3_free_table(result);
    rc_close = sqlite3_close_v2(db);

    // Nullify local references
    db = NULL;
    stmt = NULL;
    result = NULL;
    errmsg = NULL;
    serialized = NULL;
    val = NULL;
    dup_val = NULL;

    // Silence unused variable warnings
    (void)rows;
    (void)columns;
    (void)serialized_size;
    (void)colcount;
    (void)datacount;
    (void)rc_open;
    (void)rc_get;
    (void)rc_prepare;
    (void)errcode;
    (void)rc_close;
    (void)filename;
    (void)sql;

    return 66;
}