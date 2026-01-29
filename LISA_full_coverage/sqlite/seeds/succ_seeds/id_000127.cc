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
//<ID> 127
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
    int nRow = 0;
    int nCol = 0;
    char *errmsg = NULL;
    void *prev_trace = NULL;
    const char *err_msg = NULL;
    int rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    prev_trace = sqlite3_trace(db, (void (*)(void *, const char *))NULL, NULL);

    // step 2: Configure (create table and insert a small blob) using sqlite3_get_table
    int rc_gt_create_insert = sqlite3_get_table(
        db,
        "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB); INSERT INTO t(data) VALUES(x'010203');",
        &results,
        &nRow,
        &nCol,
        &errmsg
    );
    sqlite3_free_table(results);
    results = NULL;

    // step 3: Operate / Validate (open blob for the inserted row, retrieve error message)
    int rc_blob_open = sqlite3_blob_open(db, "main", "t", "data", (sqlite3_int64)1, 0, &blob);
    err_msg = sqlite3_errmsg(db);

    // step 4: Cleanup
    int rc_blob_close = sqlite3_blob_close(blob);
    int rc_close = sqlite3_close_v2(db);
    (void)rc_open;
    (void)prev_trace;
    (void)rc_gt_create_insert;
    (void)nRow;
    (void)nCol;
    (void)errmsg;
    (void)rc_blob_open;
    (void)err_msg;
    (void)rc_blob_close;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}