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
//<ID> 553
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_mutex *mtx = NULL;
    void *mem = NULL;
    const char *db_name = NULL;
    sqlite3_uint64 alloc_size = 0;
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_step_create = 0;
    int rc_fin_create = 0;
    int rc_prep_insert = 0;
    int rc_step_insert = 0;
    int rc_fin_insert = 0;
    int rc_blob_open = 0;
    int blob_len = 0;
    int rc_close = 0;

    // step 2: Setup (Initialize DB and statements)
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_prep_create = sqlite3_prepare_v2(db, "CREATE TABLE t1(x BLOB);", -1, &stmt_create, NULL);
    rc_step_create = sqlite3_step(stmt_create);
    rc_fin_create = sqlite3_finalize(stmt_create);
    rc_prep_insert = sqlite3_prepare_v2(db, "INSERT INTO t1(x) VALUES(randomblob(16));", -1, &stmt_insert, NULL);
    rc_step_insert = sqlite3_step(stmt_insert);
    rc_fin_insert = sqlite3_finalize(stmt_insert);

    // step 3: Operate / Validate (open blob, inspect size, allocate memory, query DB name, mutex check)
    rc_blob_open = sqlite3_blob_open(db, "main", "t1", "x", 1, 0, &blob);
    blob_len = sqlite3_blob_bytes(blob);
    mem = sqlite3_realloc(NULL, (int)(blob_len > 0 ? blob_len : 1));
    alloc_size = sqlite3_msize(mem);
    (void)sqlite3_mutex_notheld(mtx);
    db_name = sqlite3_db_name(db, 0);

    // step 4: Cleanup
    sqlite3_blob_close(blob);
    sqlite3_realloc(mem, 0);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_fin_create;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)rc_fin_insert;
    (void)rc_blob_open;
    (void)blob_len;
    (void)alloc_size;
    (void)db_name;
    (void)mtx;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}