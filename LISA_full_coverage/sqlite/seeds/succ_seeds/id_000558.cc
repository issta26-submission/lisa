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
//<ID> 558
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
    sqlite3_stmt *stmt = NULL;
    sqlite3_mutex *mtx = NULL;
    void *mem = NULL;
    char **gettable_result = NULL;
    char *errmsg = NULL;
    const char *dbname = NULL;
    const char *sql_create = "CREATE TABLE t1(id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_insert = "INSERT INTO t1(data) VALUES(x'010203');";
    int rc_open = 0;
    int rc_get1 = 0;
    int rc_get2 = 0;
    int rc_blob_open = 0;
    sqlite3_uint64 usable = 0;
    int blob_nbytes = 0;

    // step 2: Setup / Configure (open DB and create a table + insert a blob)
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_get1 = sqlite3_get_table(db, sql_create, &gettable_result, NULL, NULL, &errmsg);
    sqlite3_free_table(gettable_result);
    gettable_result = NULL;
    rc_get2 = sqlite3_get_table(db, sql_insert, &gettable_result, NULL, NULL, &errmsg);
    sqlite3_free_table(gettable_result);
    gettable_result = NULL;

    // step 3: Operate / Validate
    dbname = sqlite3_db_name(db, 0);
    mem = sqlite3_realloc(NULL, 128);
    usable = sqlite3_msize(mem);
    rc_blob_open = sqlite3_blob_open(db, dbname, "t1", "data", (sqlite3_int64)1, 0, &blob);
    blob_nbytes = sqlite3_blob_bytes(blob);
    (void)sqlite3_mutex_notheld(mtx);

    // step 4: Cleanup
    (void)rc_open;
    (void)rc_get1;
    (void)rc_get2;
    (void)rc_blob_open;
    (void)sql_create;
    (void)sql_insert;
    (void)errmsg;
    (void)stmt;
    (void)dbname;
    (void)usable;
    (void)blob_nbytes;
    if (blob) { sqlite3_blob_close(blob); }
    mem = sqlite3_realloc(mem, 0);
    sqlite3_close(db);

    // API sequence test completed successfully
    return 66;
}