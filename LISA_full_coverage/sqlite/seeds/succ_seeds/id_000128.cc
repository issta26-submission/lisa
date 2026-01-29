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
//<ID> 128
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
    void *prev_trace = NULL;
    const char *errstr = NULL;
    int rc_open = 0;
    int rc_get = 0;
    int rc_blob_open = 0;
    int rc_blob_close = 0;
    int rc_close = 0;

    // step 2: Configure / Setup
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    prev_trace = sqlite3_trace(db, NULL, NULL);
    const char *sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);"
                      "INSERT INTO t(data) VALUES(x'010203');"
                      "SELECT id, hex(data) FROM t;";
    rc_get = sqlite3_get_table(db, sql, &results, &rows, &cols, &errmsg);
    errstr = sqlite3_errmsg(db);

    // step 3: Operate / Validate
    rc_blob_open = sqlite3_blob_open(db, "main", "t", "data", (sqlite3_int64)1, 0, &blob);
    rc_blob_close = sqlite3_blob_close(blob);

    // step 4: Cleanup
    sqlite3_free_table(results);
    rc_close = sqlite3_close_v2(db);
    (void)prev_trace;
    (void)errstr;
    (void)errmsg;
    (void)rows;
    (void)cols;
    (void)rc_open;
    (void)rc_get;
    (void)rc_blob_open;
    (void)rc_blob_close;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}