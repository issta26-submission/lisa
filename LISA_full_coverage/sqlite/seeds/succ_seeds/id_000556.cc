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
//<ID> 556
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
    void *mem = NULL;
    sqlite3_uint64 mem_size = 0;
    char **table = NULL;
    char *errmsg = NULL;
    const char *dbname0 = NULL;
    int rc_open = 0;
    int rc_get = 0;
    int nrow = 0;
    int ncol = 0;
    int blob_b = 0;
    int mutex_notheld = 0;

    // step 2: Setup / Initialize
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_get = sqlite3_get_table(db, "SELECT 42 AS answer;", &table, &nrow, &ncol, &errmsg);

    // step 3: Operate / Validate
    mem = sqlite3_realloc(NULL, 128);
    mem_size = sqlite3_msize(mem);
    blob_b = sqlite3_blob_bytes(blob);
    mutex_notheld = sqlite3_mutex_notheld(mtx);
    dbname0 = sqlite3_db_name(db, 0);

    // step 4: Cleanup
    mem = sqlite3_realloc(mem, 0);
    sqlite3_free_table(table);
    (void)errmsg;
    (void)rc_open;
    (void)rc_get;
    (void)nrow;
    (void)ncol;
    (void)blob_b;
    (void)mutex_notheld;
    (void)mem_size;
    (void)dbname0;
    (void)blob;
    (void)mtx;
    (void)db;

    // API sequence test completed successfully
    return 66;
}