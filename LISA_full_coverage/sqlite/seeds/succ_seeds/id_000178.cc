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
//<ID> 178
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Initialize and Configure
    sqlite3 *db = NULL;
    int rc_enable_shared = sqlite3_enable_shared_cache(1);
    int rc_open = sqlite3_open(":memory:", &db);
    int rc_enable_ext = sqlite3_enable_load_extension(db, 1);

    // step 2: Operate (create table and populate)
    const char *sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT); INSERT INTO t(val) VALUES('hello');";
    char *errmsg = NULL;
    int rc_exec = sqlite3_exec(db, sql, NULL, NULL, &errmsg);

    // step 3: Validate metadata and error state
    const char *pzDataType = NULL;
    const char *pzCollSeq = NULL;
    int pNotNull = 0;
    int pPrimaryKey = 0;
    int pAutoinc = 0;
    int rc_meta = sqlite3_table_column_metadata(db, "main", "t", "val", &pzDataType, &pzCollSeq, &pNotNull, &pPrimaryKey, &pAutoinc);
    int rc_errcode = sqlite3_errcode(db);

    // step 4: Cleanup
    int rc_close = sqlite3_close_v2(db);
    (void)rc_enable_shared;
    (void)rc_open;
    (void)rc_enable_ext;
    (void)rc_exec;
    (void)errmsg;
    (void)pzDataType;
    (void)pzCollSeq;
    (void)pNotNull;
    (void)pPrimaryKey;
    (void)pAutoinc;
    (void)rc_meta;
    (void)rc_errcode;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}