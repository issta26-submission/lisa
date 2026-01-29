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
//<ID> 123
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
    int rows = 0;
    int cols = 0;
    char *errmsg = NULL;
    void *trace_prev = NULL;
    const char *errstr = NULL;
    int rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 2: Configure
    trace_prev = sqlite3_trace(db, (void (*)(void *, const char *))NULL, NULL);
    int rc_get = sqlite3_get_table(
        db,
        "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);"
        "INSERT INTO t(data) VALUES(zeroblob(8));"
        "SELECT data FROM t;",
        &results, &rows, &cols, &errmsg);

    // step 3: Operate / Validate
    int rc_blob_open = sqlite3_blob_open(db, "main", "t", "data", (sqlite3_int64)1, 0, &blob);
    int rc_blob_close = sqlite3_blob_close(blob);
    errstr = sqlite3_errmsg(db);

    // step 4: Cleanup
    sqlite3_free_table(results);
    int rc_close = sqlite3_close_v2(db);
    (void)trace_prev;
    (void)rc_open;
    (void)rc_get;
    (void)rows;
    (void)cols;
    (void)errmsg;
    (void)rc_blob_open;
    (void)rc_blob_close;
    (void)errstr;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}