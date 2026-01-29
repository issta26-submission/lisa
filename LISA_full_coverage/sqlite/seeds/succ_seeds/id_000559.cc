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
//<ID> 559
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    char **results = NULL;
    char *errmsg = NULL;
    int nRow = 0;
    int nCol = 0;
    sqlite3_blob *blob = NULL;
    void *buffer = NULL;
    sqlite3_uint64 buffer_size = 0;
    int blob_sz = 0;
    const char *database_name = NULL;
    int rc_open = 0;
    int rc_gt_create = 0;
    int rc_gt_insert = 0;
    int rc_gt_select = 0;
    int rc_free = 0;
    int rc_close = 0;
    int mutex_not_held = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_gt_create = sqlite3_get_table(db, "CREATE TABLE t1(id INTEGER PRIMARY KEY, data BLOB);", &results, &nRow, &nCol, &errmsg);
    rc_free = 0;
    sqlite3_free_table(results);
    rc_gt_insert = sqlite3_get_table(db, "INSERT INTO t1(data) VALUES(x'01020304');", &results, &nRow, &nCol, &errmsg);
    sqlite3_free_table(results);
    rc_gt_select = sqlite3_get_table(db, "SELECT id, data FROM t1;", &results, &nRow, &nCol, &errmsg);

    // step 3: Operate / Validate
    database_name = sqlite3_db_name(db, 0);
    buffer = malloc(64);
    memset(buffer, 0, 64);
    buffer_size = sqlite3_msize(buffer);
    blob_sz = sqlite3_blob_bytes(blob);
    mutex_not_held = sqlite3_mutex_notheld((sqlite3_mutex *)NULL);

    // step 4: Cleanup
    sqlite3_free_table(results);
    free(buffer);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_gt_create;
    (void)rc_gt_insert;
    (void)rc_gt_select;
    (void)rc_free;
    (void)rc_close;
    (void)errmsg;
    (void)nRow;
    (void)nCol;
    (void)blob;
    (void)buffer_size;
    (void)blob_sz;
    (void)database_name;
    (void)mutex_not_held;

    // API sequence test completed successfully
    return 66;
}