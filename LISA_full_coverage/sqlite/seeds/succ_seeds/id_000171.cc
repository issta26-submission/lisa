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
//<ID> 171
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    int rc_init = sqlite3_initialize();
    int rc_shared_on = sqlite3_enable_shared_cache(1);

    // step 2: Setup - open an in-memory database and enable load_extension
    int rc_open = sqlite3_open(":memory:", &db);
    int rc_enable_ext_on = sqlite3_enable_load_extension(db, 1);

    // step 3: Operate - query column metadata for a sample table/column and inspect error state
    const char *pzDataType = NULL;
    const char *pzCollSeq = NULL;
    int pNotNull = 0;
    int pPrimaryKey = 0;
    int pAutoinc = 0;
    int rc_meta = sqlite3_table_column_metadata(db, "main", "t", "id", &pzDataType, &pzCollSeq, &pNotNull, &pPrimaryKey, &pAutoinc);
    int db_errcode = sqlite3_errcode(db);
    const char *db_errmsg = sqlite3_errmsg(db);

    // step 4: Cleanup / Validate - disable extension loading, disable shared cache and release memory
    int rc_enable_ext_off = sqlite3_enable_load_extension(db, 0);
    int rc_shared_off = sqlite3_enable_shared_cache(0);
    int rc_release_mem = sqlite3_db_release_memory(db);

    (void)rc_init;
    (void)rc_shared_on;
    (void)rc_open;
    (void)rc_enable_ext_on;
    (void)rc_meta;
    (void)pzDataType;
    (void)pzCollSeq;
    (void)pNotNull;
    (void)pPrimaryKey;
    (void)pAutoinc;
    (void)db_errcode;
    (void)db_errmsg;
    (void)rc_enable_ext_off;
    (void)rc_shared_off;
    (void)rc_release_mem;

    // API sequence test completed successfully
    return 66;
}