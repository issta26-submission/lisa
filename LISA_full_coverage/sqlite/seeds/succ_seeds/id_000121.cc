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
//<ID> 121
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
    const char *create_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *insert_sql = "INSERT INTO t(data) VALUES(zeroblob(16));";
    const char *select_sql = "SELECT id, length(data) FROM t;";
    int rc_open = sqlite3_open(":memory:", &db);
    void *prev_trace = sqlite3_trace(db, NULL, NULL);

    // step 2: Configure
    int rc_create = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    int rc_insert = sqlite3_exec(db, insert_sql, NULL, NULL, NULL);

    // step 3: Operate / Validate
    int rc_blob_open = sqlite3_blob_open(db, "main", "t", "data", 1, 0, &blob);
    int rc_get_table = sqlite3_get_table(db, select_sql, &results, &rows, &cols, NULL);
    const char *db_errmsg = sqlite3_errmsg(db);
    int combined_check = rc_open + rc_create + rc_insert + rc_blob_open + rc_get_table + rows + cols;
    (void)combined_check;
    (void)db_errmsg;
    (void)prev_trace;

    // step 4: Cleanup
    int rc_blob_close = sqlite3_blob_close(blob);
    sqlite3_free_table(results);
    int rc_close = sqlite3_close(db);
    (void)rc_open;
    (void)rc_create;
    (void)rc_insert;
    (void)rc_blob_open;
    (void)rc_get_table;
    (void)rows;
    (void)cols;
    (void)rc_blob_close;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}