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
//<ID> 122
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
    int nrow = 0;
    int ncol = 0;
    void *prev_trace = NULL;
    const char *errmsg_str = NULL;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, val BLOB);";
    const char *sql_insert = "INSERT INTO t(val) VALUES(x'000102');";
    const char *sql_select = "SELECT val FROM t WHERE id=1;";
    int rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 2: Configure
    prev_trace = sqlite3_trace(db, NULL, (void *)0x1);
    int rc_create = sqlite3_get_table(db, sql_create, &results, &nrow, &ncol, NULL);
    sqlite3_free_table(results);
    results = NULL;
    int rc_insert = sqlite3_get_table(db, sql_insert, &results, &nrow, &ncol, NULL);
    sqlite3_free_table(results);
    results = NULL;

    // step 3: Operate / Validate
    int rc_select = sqlite3_get_table(db, sql_select, &results, &nrow, &ncol, NULL);
    int rc_blob_open = sqlite3_blob_open(db, "main", "t", "val", (sqlite3_int64)1, 0, &blob);
    int rc_blob_close = sqlite3_blob_close(blob);
    errmsg_str = sqlite3_errmsg(db);

    // step 4: Cleanup
    sqlite3_free_table(results);
    results = NULL;
    sqlite3_trace(db, NULL, prev_trace);
    int rc_close = sqlite3_close_v2(db);
    (void)rc_open;
    (void)rc_create;
    (void)rc_insert;
    (void)rc_select;
    (void)rc_blob_open;
    (void)rc_blob_close;
    (void)rc_close;
    (void)nrow;
    (void)ncol;
    (void)errmsg_str;
    (void)blob;
    (void)prev_trace;
    // API sequence test completed successfully
    return 66;
}