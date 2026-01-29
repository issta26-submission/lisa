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
//<ID> 175
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT NOT NULL);";
    int rc_open = sqlite3_open(":memory:", &db);
    int rc_shared = sqlite3_enable_shared_cache(1);

    // step 2: Configure behavior for this connection
    int rc_load_on = sqlite3_enable_load_extension(db, 1);

    // step 3: Operate - create table and query metadata for a column
    int rc_exec = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    const char *pzDataType = NULL;
    const char *pzCollSeq = NULL;
    int notNull = 0;
    int primaryKey = 0;
    int autoinc = 0;
    int rc_meta = sqlite3_table_column_metadata(db, "main", "t", "val", &pzDataType, &pzCollSeq, &notNull, &primaryKey, &autoinc);
    int rc_err = sqlite3_errcode(db);

    // step 4: Validate / Cleanup
    int rc_load_off = sqlite3_enable_load_extension(db, 0);
    int rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_shared;
    (void)rc_load_on;
    (void)rc_exec;
    (void)pzDataType;
    (void)pzCollSeq;
    (void)notNull;
    (void)primaryKey;
    (void)autoinc;
    (void)rc_meta;
    (void)rc_err;
    (void)rc_load_off;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}