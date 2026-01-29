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
//<ID> 554
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_mutex *mtx = NULL;
    sqlite3_blob *blob = NULL;
    void *mem = NULL;
    const char *db_name0 = NULL;
    int rc_open = 0;
    int mtx_notheld_res = 0;
    sqlite3_uint64 mem_size = 0;
    int blob_bytes = 0;

    // step 2: Setup / Initialize resources
    mem = malloc(64);
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 3: Operate / Validate (meaningful data flow between calls)
    db_name0 = sqlite3_db_name(db, 0);
    mtx_notheld_res = sqlite3_mutex_notheld(mtx);
    mem_size = sqlite3_msize(mem);
    blob_bytes = sqlite3_blob_bytes(blob);

    // step 4: Cleanup
    free(mem);
    if (db) {
        sqlite3_close_v2(db);
    }

    (void)db_name0;
    (void)rc_open;
    (void)mtx_notheld_res;
    (void)mem_size;
    (void)blob_bytes;
    (void)mtx;
    (void)blob;
    (void)db;

    // API sequence test completed successfully
    return 66;
}