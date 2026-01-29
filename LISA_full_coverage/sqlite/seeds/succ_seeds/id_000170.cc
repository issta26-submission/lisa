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
//<ID> 170
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    const char *sql_create = "CREATE TABLE test(id INTEGER PRIMARY KEY, name TEXT);";
    const char *sql_insert = "INSERT INTO test(name) VALUES('alice');";
    const char *pzDataType = NULL;
    const char *pzCollSeq = NULL;
    int pNotNull = 0;
    int pPrimaryKey = 0;
    int pAutoinc = 0;
    int rc_shared_on = sqlite3_enable_shared_cache(1);
    int rc_open = sqlite3_open(":memory:", &db);

    // step 2: Configure
    int rc_enable_load_ext = sqlite3_enable_load_extension(db, 0);
    int rc_exec_create = sqlite3_exec(db, sql_create, 0, 0, 0);
    int rc_exec_insert = sqlite3_exec(db, sql_insert, 0, 0, 0);

    // step 3: Operate / Validate metadata and error state
    int rc_meta = sqlite3_table_column_metadata(db, "main", "test", "name", &pzDataType, &pzCollSeq, &pNotNull, &pPrimaryKey, &pAutoinc);
    int rc_err = sqlite3_errcode(db);

    // step 4: Cleanup and final validation
    int rc_shared_off = sqlite3_enable_shared_cache(0);
    int rc_close = sqlite3_close_v2(db);
    (void)rc_shared_on;
    (void)rc_open;
    (void)rc_enable_load_ext;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)rc_meta;
    (void)rc_err;
    (void)pzDataType;
    (void)pzCollSeq;
    (void)pNotNull;
    (void)pPrimaryKey;
    (void)pAutoinc;
    (void)rc_shared_off;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}