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
//<ID> 125
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_blob *blob = NULL;
    char **results = NULL;
    char *zErrMsg = NULL;
    int nRow = 0;
    int nCol = 0;
    void *prev_trace = NULL;
    const char *value = NULL;
    const char *errmsg = NULL;
    int rc_open = 0;
    int rc_create = 0;
    int rc_insert = 0;
    int rc_select = 0;
    int rc_blob_open = 0;
    int rc_blob_close = 0;
    int rc_close = 0;

    // step 2: Configure / Open DB and set trace
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    prev_trace = sqlite3_trace(db, (void (*)(void *, const char *))NULL, NULL);

    // step 3: Operate / Validate (use get_table for DDL, DML, and query; open blob; inspect errmsg)
    rc_create = sqlite3_get_table(db, "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT);", &results, &nRow, &nCol, &zErrMsg);
    sqlite3_free_table(results);
    results = NULL;
    rc_insert = sqlite3_get_table(db, "INSERT INTO t(val) VALUES('hello');", &results, &nRow, &nCol, &zErrMsg);
    sqlite3_free_table(results);
    results = NULL;
    rc_select = sqlite3_get_table(db, "SELECT val FROM t WHERE id=1;", &results, &nRow, &nCol, &zErrMsg);
    value = results ? results[1] : NULL;
    rc_blob_open = sqlite3_blob_open(db, "main", "t", "val", (sqlite3_int64)1, 0, &blob);
    errmsg = sqlite3_errmsg(db);
    rc_blob_close = sqlite3_blob_close(blob);

    // step 4: Cleanup
    sqlite3_free_table(results);
    results = NULL;
    rc_close = sqlite3_close(db);

    (void)prev_trace;
    (void)value;
    (void)errmsg;
    (void)rc_open;
    (void)rc_create;
    (void)rc_insert;
    (void)rc_select;
    (void)rc_blob_open;
    (void)rc_blob_close;
    (void)rc_close;
    (void)zErrMsg;
    (void)nRow;
    (void)nCol;
    (void)blob;
    (void)db;

    // API sequence test completed successfully
    return 66;
}