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
//<ID> 124
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    void *prev_trace = NULL;
    char **results = NULL;
    int rows = 0;
    int cols = 0;
    char *errmsg = NULL;
    sqlite3_blob *blob = NULL;
    const char *errstr = NULL;
    sqlite3_int64 target_row = 1;
    int rc_open = 0;
    int rc_create = 0;
    int rc_insert = 0;
    int rc_select = 0;
    int rc_blob_open = 0;
    int rc_blob_close = 0;
    int rc_close = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    prev_trace = sqlite3_trace(db, (void (*)(void *, const char *))NULL, NULL);

    // step 3: Operate / Validate
    rc_create = sqlite3_get_table(db, "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);", &results, &rows, &cols, &errmsg);
    sqlite3_free_table(results);
    results = NULL;
    rows = 0;
    cols = 0;
    errmsg = NULL;

    rc_insert = sqlite3_get_table(db, "INSERT INTO t(data) VALUES(x'414243');", &results, &rows, &cols, &errmsg);
    sqlite3_free_table(results);
    results = NULL;
    rows = 0;
    cols = 0;
    errmsg = NULL;

    rc_select = sqlite3_get_table(db, "SELECT id, data FROM t;", &results, &rows, &cols, &errmsg);
    const char *selected_id_text = NULL;
    const char *selected_data_text = NULL;
    selected_id_text = (results ? results[cols + 0] : NULL);
    selected_data_text = (results ? results[cols + 1] : NULL);

    rc_blob_open = sqlite3_blob_open(db, "main", "t", "data", target_row, 0, &blob);
    rc_blob_close = sqlite3_blob_close(blob);
    blob = NULL;

    errstr = sqlite3_errmsg(db);

    // step 4: Cleanup
    sqlite3_free_table(results);
    results = NULL;
    rc_close = sqlite3_close(db);
    db = NULL;

    (void)prev_trace;
    (void)rows;
    (void)cols;
    (void)errmsg;
    (void)errstr;
    (void)selected_id_text;
    (void)selected_data_text;
    (void)rc_open;
    (void)rc_create;
    (void)rc_insert;
    (void)rc_select;
    (void)rc_blob_open;
    (void)rc_blob_close;
    (void)rc_close;
    (void)target_row;

    // API sequence test completed successfully
    return 66;
}