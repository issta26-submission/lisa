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
//<ID> 173
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Initialize
    sqlite3 *db = NULL;
    int rc_enable_shared = sqlite3_enable_shared_cache(1);
    int rc_open = sqlite3_open(":memory:", &db);

    // step 2: Configure
    int rc_enable_ext_on = sqlite3_enable_load_extension(db, 1);

    // step 3: Operate
    const char *create_sql = "CREATE TABLE IF NOT EXISTS t(id INTEGER PRIMARY KEY, val TEXT);";
    char *errmsg = NULL;
    int rc_exec = sqlite3_exec(db, create_sql, NULL, NULL, &errmsg);
    const char *pDataType = NULL;
    const char *pCollSeq = NULL;
    int pNotNull = 0;
    int pPrimaryKey = 0;
    int pAutoinc = 0;
    int rc_meta = sqlite3_table_column_metadata(db, "main", "t", "val", &pDataType, &pCollSeq, &pNotNull, &pPrimaryKey, &pAutoinc);

    // step 4: Validate and Cleanup
    int rc_errcode = sqlite3_errcode(db);
    const char *errmsg_now = sqlite3_errmsg(db);
    int rc_disable_ext = sqlite3_enable_load_extension(db, 0);
    int rc_disable_shared = sqlite3_enable_shared_cache(0);
    int rc_release = sqlite3_db_release_memory(db);

    (void)rc_enable_shared;
    (void)rc_open;
    (void)rc_enable_ext_on;
    (void)rc_exec;
    (void)pDataType;
    (void)pCollSeq;
    (void)pNotNull;
    (void)pPrimaryKey;
    (void)pAutoinc;
    (void)rc_meta;
    (void)rc_errcode;
    (void)errmsg;
    (void)errmsg_now;
    (void)rc_disable_ext;
    (void)rc_disable_shared;
    (void)rc_release;
    // API sequence test completed successfully
    return 66;
}