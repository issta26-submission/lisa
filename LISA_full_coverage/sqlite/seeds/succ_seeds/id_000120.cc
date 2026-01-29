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
//<ID> 120
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
    const char *errmsg_text = NULL;
    void *prev_trace = NULL;
    int rc_open = 0;
    int rc_get = 0;
    int rc_blob_open = 0;
    int rc_blob_close = 0;
    int rc_close = 0;

    // step 2: Setup / Open database and set trace (no-op trace)
    rc_open = sqlite3_open_v2(":memory:", &db, 6, NULL);
    prev_trace = sqlite3_trace(db, (void (*)(void *, const char *))NULL, (void *)0);

    // step 3: Operate / Create table, insert blob data, fetch with get_table, open blob, and retrieve error text
    const char *sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB); INSERT INTO t(data) VALUES(X'414243'); SELECT id, data FROM t;";
    rc_get = sqlite3_get_table(db, sql, &results, &nRow, &nCol, &errmsg);
    rc_blob_open = sqlite3_blob_open(db, "main", "t", "data", (sqlite3_int64)1, 0, &blob);
    rc_blob_close = sqlite3_blob_close(blob);
    errmsg_text = sqlite3_errmsg(db);
    prev_trace = sqlite3_trace(db, (void (*)(void *, const char *))NULL, (void *)errmsg_text);

    // step 4: Cleanup
    sqlite3_free_table(results);
    rc_close = sqlite3_close_v2(db);
    (void)rc_open;
    (void)prev_trace;
    (void)rc_get;
    (void)rc_blob_open;
    (void)rc_blob_close;
    (void)errmsg;
    (void)nRow;
    (void)nCol;
    (void)errmsg_text;
    (void)rc_close;
    (void)blob;
    // API sequence test completed successfully
    return 66;
}