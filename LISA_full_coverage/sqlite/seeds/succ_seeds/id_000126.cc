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
//<ID> 126
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
    char **result = NULL;
    int rows = 0;
    int cols = 0;
    char *errmsg = NULL;
    const char *errstr = NULL;
    void *prev_trace = NULL;
    int rc_open = 0;
    int rc_gt_create = 0;
    int rc_gt_insert = 0;
    int rc_blob_open = 0;
    int rc_blob_close = 0;
    int rc_gt_select = 0;
    int rc_close = 0;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_insert = "INSERT INTO t(data) VALUES(zeroblob(10));";
    const char *sql_select = "SELECT rowid, data FROM t LIMIT 1;";

    // step 2: Setup / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    prev_trace = sqlite3_trace(db, NULL, NULL);

    // step 3: Operate / Validate
    rc_gt_create = sqlite3_get_table(db, sql_create, &result, &rows, &cols, NULL);
    rc_gt_insert = sqlite3_get_table(db, sql_insert, &result, &rows, &cols, NULL);
    rc_blob_open = sqlite3_blob_open(db, "main", "t", "data", (sqlite3_int64)1, 1, &blob);
    rc_blob_close = sqlite3_blob_close(blob);
    errstr = sqlite3_errmsg(db);
    rc_gt_select = sqlite3_get_table(db, sql_select, &result, &rows, &cols, NULL);

    // step 4: Cleanup
    sqlite3_free_table(result);
    rc_close = sqlite3_close_v2(db);
    (void)rc_open;
    (void)prev_trace;
    (void)rc_gt_create;
    (void)rc_gt_insert;
    (void)rc_blob_open;
    (void)rc_blob_close;
    (void)errstr;
    (void)rc_gt_select;
    (void)rc_close;
    (void)errmsg;
    (void)rows;
    (void)cols;
    (void)blob;
    (void)result;
    // API sequence test completed successfully
    return 66;
}