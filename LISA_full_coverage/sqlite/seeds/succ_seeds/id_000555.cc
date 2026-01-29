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
//<ID> 555
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_mutex *mtx = NULL;
    const char *sql_create = "CREATE TABLE t1(id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_insert = "INSERT INTO t1(data) VALUES(?);";
    void *mem = malloc(64);
    sqlite3_uint64 mem_size = 0;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_fin = 0;
    int rc_close = 0;
    int blob_bytes = 0;
    int mutex_notheld = 0;
    const char *db_name = NULL;

    // step 2: Setup / Initialize database and table
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_prep = sqlite3_prepare_v2(db, sql_create, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_fin = sqlite3_finalize(stmt);
    rc_prep = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    rc_step = sqlite3_bind_blob64(stmt, 1, mem, 64, NULL);
    rc_step = sqlite3_step(stmt);
    rc_fin = sqlite3_finalize(stmt);

    // step 3: Operate / Validate (use required APIs)
    mem_size = sqlite3_msize(mem);
    blob_bytes = sqlite3_blob_bytes(blob);
    db_name = sqlite3_db_name(db, 0);
    mutex_notheld = sqlite3_mutex_notheld(mtx);

    // step 4: Cleanup
    free(mem);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_fin;
    (void)rc_close;
    (void)mem_size;
    (void)blob_bytes;
    (void)db_name;
    (void)mutex_notheld;
    (void)sql_create;
    (void)sql_insert;
    (void)stmt;
    (void)blob;
    (void)mtx;

    // API sequence test completed successfully
    return 66;
}