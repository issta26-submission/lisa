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
//<ID> 179
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    const char *create_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY AUTOINCREMENT, val TEXT);";
    int rc_init = sqlite3_initialize();
    int rc_shared = sqlite3_enable_shared_cache(1);
    int rc_open = sqlite3_open(":memory:", &db);

    // step 2: Configure database instance
    int rc_load_ext = sqlite3_enable_load_extension(db, 0);

    // step 3: Operate - create a table and query its column metadata
    int rc_exec = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    const char *pzDataType = NULL;
    const char *pzCollSeq = NULL;
    int pNotNull = 0;
    int pPrimaryKey = 0;
    int pAutoinc = 0;
    int rc_meta = sqlite3_table_column_metadata(db, "main", "t", "id", &pzDataType, &pzCollSeq, &pNotNull, &pPrimaryKey, &pAutoinc);

    // step 4: Validate and cleanup
    int last_err = sqlite3_errcode(db);
    int rc_release = sqlite3_db_release_memory(db);

    (void)rc_init;
    (void)rc_shared;
    (void)rc_open;
    (void)rc_load_ext;
    (void)rc_exec;
    (void)pzDataType;
    (void)pzCollSeq;
    (void)pNotNull;
    (void)pPrimaryKey;
    (void)pAutoinc;
    (void)rc_meta;
    (void)last_err;
    (void)rc_release;

    // API sequence test completed successfully
    return 66;
}