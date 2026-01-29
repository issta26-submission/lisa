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
//<ID> 551
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    char **results1 = NULL;
    char **results2 = NULL;
    int nrow1 = 0;
    int ncol1 = 0;
    int nrow2 = 0;
    int ncol2 = 0;
    char *errmsg1 = NULL;
    char *errmsg2 = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_mutex *mtx = NULL;
    void *buf = NULL;
    sqlite3_uint64 msize_res = 0;
    const char *dbname = NULL;
    int blob_size = 0;
    int rc_open = 0;
    int rc_get1 = 0;
    int rc_get2 = 0;
    int rc_blob_open = 0;
    int rc_mutex_notheld = 0;
    int rc_close = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_get1 = sqlite3_get_table(db, "CREATE TABLE t1(id INTEGER PRIMARY KEY, b BLOB);", &results1, &nrow1, &ncol1, &errmsg1);
    sqlite3_free_table(results1);
    rc_get2 = sqlite3_get_table(db, "INSERT INTO t1(b) VALUES(X'01020304');", &results2, &nrow2, &ncol2, &errmsg2);
    sqlite3_free_table(results2);

    // step 3: Operate / Validate
    rc_blob_open = sqlite3_blob_open(db, "main", "t1", "b", 1, 0, &blob);
    blob_size = sqlite3_blob_bytes(blob);
    buf = malloc((size_t)(blob_size > 0 ? blob_size : 1));
    msize_res = sqlite3_msize(buf);
    rc_mutex_notheld = sqlite3_mutex_notheld(mtx);
    dbname = sqlite3_db_name(db, 0);

    // step 4: Cleanup
    free(buf);
    if (blob) {
        /* sqlite3_blob_close may be available in the linked sqlite3 library */
        sqlite3_blob_close(blob);
    }
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_get1;
    (void)errmsg1;
    (void)rc_get2;
    (void)errmsg2;
    (void)rc_blob_open;
    (void)blob_size;
    (void)msize_res;
    (void)rc_mutex_notheld;
    (void)dbname;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}