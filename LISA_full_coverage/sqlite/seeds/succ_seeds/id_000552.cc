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
//<ID> 552
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_mutex *mtx = NULL;
    char *mem = NULL;
    char **result = NULL;
    char *errmsg = NULL;
    const char *dbname = NULL;
    sqlite3_uint64 mem_size = 0;
    int blob_size = 0;
    int rc_open = 0;
    int rc_create = 0;
    int rc_insert = 0;
    int rc_blob = 0;
    int rows = 0;
    int cols = 0;
    int mutex_notheld = 0;

    // step 2: Setup / Initialize DB and table
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_create = sqlite3_get_table(db, "CREATE TABLE t1(id INTEGER PRIMARY KEY, data BLOB);", &result, &rows, &cols, &errmsg);
    sqlite3_free_table(result);
    result = NULL;
    rc_insert = sqlite3_get_table(db, "INSERT INTO t1(data) VALUES(x'010203');", &result, &rows, &cols, &errmsg);
    sqlite3_free_table(result);
    result = NULL;
    dbname = sqlite3_db_name(db, 0);

    // step 3: Operate / Validate: open blob, measure memory, query mutex state
    rc_blob = sqlite3_blob_open(db, dbname, "t1", "data", 1, 0, &blob);
    blob_size = sqlite3_blob_bytes(blob);
    mem = (char*)malloc(32);
    mem_size = sqlite3_msize((void*)mem);
    mutex_notheld = sqlite3_mutex_notheld(mtx);

    // step 4: Cleanup
    sqlite3_blob_close(blob);
    free(mem);
    sqlite3_free_table(NULL); /* safe no-op cleanup call */
    sqlite3_close(db);

    (void)rc_open;
    (void)rc_create;
    (void)rc_insert;
    (void)rc_blob;
    (void)rows;
    (void)cols;
    (void)errmsg;
    (void)dbname;
    (void)mem_size;
    (void)blob_size;
    (void)mutex_notheld;
    (void)mtx;
    (void)result;

    // API sequence test completed successfully
    return 66;
}