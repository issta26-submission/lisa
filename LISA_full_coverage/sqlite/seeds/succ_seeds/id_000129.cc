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
//<ID> 129
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    char **results = NULL;
    int nrow = 0;
    int ncol = 0;
    char *errmsg = NULL;
    sqlite3_blob *pBlob = NULL;
    void *trace_cookie = NULL;
    const char *last_errmsg = NULL;
    int rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    sqlite3_trace(db, (void (*)(void *, const char *))NULL, NULL);

    // step 2: Configure - create table and insert a blob using sqlite3_get_table
    const char *sql_setup = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB); INSERT INTO t(data) VALUES(x'010203');";
    int rc_get_setup = sqlite3_get_table(db, sql_setup, &results, &nrow, &ncol, &errmsg);
    sqlite3_free_table(results);
    results = NULL;
    nrow = 0;
    ncol = 0;

    // step 3: Operate / Validate - open a blob handle on rowid 1 and get error text
    int rc_blob_open = sqlite3_blob_open(db, "main", "t", "data", (sqlite3_int64)1, 0, &pBlob);
    last_errmsg = sqlite3_errmsg(db);

    // step 4: Cleanup
    int rc_blob_close = sqlite3_blob_close(pBlob);
    pBlob = NULL;
    int rc_close = sqlite3_close_v2(db);
    (void)trace_cookie;
    (void)errmsg;
    (void)last_errmsg;
    (void)rc_open;
    (void)rc_get_setup;
    (void)rc_blob_open;
    (void)rc_blob_close;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}