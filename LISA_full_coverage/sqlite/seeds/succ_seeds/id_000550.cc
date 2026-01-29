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
//<ID> 550
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
    sqlite3_mutex *mtx = NULL;
    char **result = NULL;
    char *errmsg = NULL;
    const char *db_name = NULL;
    const char *sql_create = "CREATE TABLE t1(id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_insert = "INSERT INTO t1(data) VALUES(zeroblob(16));";
    void *mem = NULL;
    sqlite3_uint64 allocated_size = 0;
    int nRow = 0;
    int nCol = 0;
    int rc_open = 0;
    int rc_get_create = 0;
    int rc_get_insert = 0;
    int rc_blob_open = 0;
    int blob_size = 0;
    int mutex_notheld = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_get_create = sqlite3_get_table(db, sql_create, &result, &nRow, &nCol, &errmsg);
    sqlite3_free_table(result);
    result = NULL;
    rc_get_insert = sqlite3_get_table(db, sql_insert, &result, &nRow, &nCol, &errmsg);
    sqlite3_free_table(result);
    result = NULL;

    // step 3: Operate / Validate
    db_name = sqlite3_db_name(db, 0);
    mem = sqlite3_realloc(NULL, 64);
    allocated_size = sqlite3_msize(mem);
    rc_blob_open = sqlite3_blob_open(db, "main", "t1", "data", 1, 0, &blob);
    blob_size = sqlite3_blob_bytes(blob);
    mtx = sqlite3_mutex_alloc(0);
    mutex_notheld = sqlite3_mutex_notheld(mtx);

    // step 4: Cleanup
    (void)rc_get_create;
    (void)rc_get_insert;
    (void)rc_blob_open;
    (void)rc_open;
    (void)db_name;
    (void)errmsg;
    (void)nRow;
    (void)nCol;
    (void)mutex_notheld;
    (void)allocated_size;
    (void)blob_size;
    sqlite3_blob_close(blob);
    sqlite3_realloc(mem, 0);
    sqlite3_mutex_free(mtx);
    sqlite3_close(db);

    // API sequence test completed successfully
    return 66;
}