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
//<ID> 572
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
    const char *sql_create = "CREATE TABLE t1(id INTEGER PRIMARY KEY, value TEXT);";
    const char *sql_insert = "INSERT INTO t1(value) VALUES('hello');";
    const char *sql_select = "SELECT value FROM t1 WHERE id=1;";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int col_type = 0;
    int col_bytes = 0;
    const void *col_text16 = NULL;
    int glob_match = 0;
    int errcode = 0;
    unsigned char *serialized = NULL;
    sqlite3_int64 serialized_size = 0;
    char tmpbuf[16];
    memset(tmpbuf, 0, sizeof(tmpbuf));

    // step 2: Setup / Configure - open and create table, insert a row
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_prepare = sqlite3_prepare_v3(db, sql_create, -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    rc_prepare = sqlite3_prepare_v3(db, sql_insert, -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);

    // step 3: Operate / Validate - select the row, inspect column, use glob, errcode, serialize
    rc_prepare = sqlite3_prepare_v3(db, sql_select, -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    col_type = sqlite3_column_type(stmt, 0);
    col_bytes = sqlite3_column_bytes(stmt, 0);
    col_text16 = sqlite3_column_text16(stmt, 0);
    glob_match = sqlite3_strglob("m*", "main");
    errcode = sqlite3_errcode(db);
    serialized = sqlite3_serialize(db, "main", &serialized_size, 0);

    // step 4: Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close_v2(db);
    free(serialized);
    memset(tmpbuf, 0, sizeof(tmpbuf));

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)col_type;
    (void)col_bytes;
    (void)col_text16;
    (void)glob_match;
    (void)errcode;
    (void)serialized_size;
    (void)tmpbuf;

    // API sequence test completed successfully
    return 66;
}