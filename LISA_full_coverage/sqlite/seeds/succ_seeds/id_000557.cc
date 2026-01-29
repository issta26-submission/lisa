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
//<ID> 557
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    char **table_result = NULL;
    int nrow = 0;
    int ncol = 0;
    char *errmsg = NULL;
    int rc_open = 0;
    int rc_get = 0;
    int rc_blob_open = 0;
    sqlite3_blob *blob = NULL;
    const char *dbname = NULL;
    int blob_size = 0;
    sqlite3_uint64 alloc_size = 0;
    sqlite3_mutex *mtx = NULL;
    int mutex_notheld = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_get = sqlite3_get_table(db,
        "CREATE TABLE t1(id INTEGER PRIMARY KEY, data BLOB);"
        "INSERT INTO t1(data) VALUES(x'010203');"
        "SELECT id, hex(data) FROM t1;",
        &table_result, &nrow, &ncol, &errmsg);
    dbname = sqlite3_db_name(db, 0);
    rc_blob_open = sqlite3_blob_open(db, "main", "t1", "data", 1, 0, &blob);
    blob_size = sqlite3_blob_bytes(blob);
    alloc_size = sqlite3_msize((void*)table_result);
    mutex_notheld = sqlite3_mutex_notheld(mtx);

    // step 3: Operate / Validate
    (void)rc_open;
    (void)rc_get;
    (void)rc_blob_open;
    (void)dbname;
    (void)blob_size;
    (void)alloc_size;
    (void)mutex_notheld;
    (void)nrow;
    (void)ncol;
    (void)errmsg;
    (void)table_result;
    (void)blob;

    // step 4: Cleanup
    sqlite3_free_table(table_result);
    sqlite3_close(db);

    // API sequence test completed successfully
    return 66;
}