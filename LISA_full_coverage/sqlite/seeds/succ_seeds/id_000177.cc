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
//<ID> 177
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

    // step 2: Configure - open an in-memory database and enable load-extension
    int rc_open = sqlite3_open(":memory:", &db);
    int rc_load_on = sqlite3_enable_load_extension(db, 1);

    // step 3: Operate - create a table and insert a row
    const char *sql = "CREATE TABLE t(id INTEGER PRIMARY KEY AUTOINCREMENT, val TEXT NOT NULL COLLATE NOCASE); INSERT INTO t(val) VALUES('hello');";
    int rc_exec = sqlite3_exec(db, sql, NULL, NULL, NULL);

    // step 4: Validate and Cleanup - inspect column metadata and restore settings
    const char *pzDataType = NULL;
    const char *pzCollSeq = NULL;
    int notNull = 0;
    int primaryKey = 0;
    int autoinc = 0;
    int rc_meta = sqlite3_table_column_metadata(db, "main", "t", "id", &pzDataType, &pzCollSeq, &notNull, &primaryKey, &autoinc);
    int err = sqlite3_errcode(db);
    int rc_load_off = sqlite3_enable_load_extension(db, 0);
    int rc_shared_off = sqlite3_enable_shared_cache(0);

    (void)rc_init;
    (void)rc_shared_on;
    (void)rc_open;
    (void)rc_load_on;
    (void)rc_exec;
    (void)pzDataType;
    (void)pzCollSeq;
    (void)notNull;
    (void)primaryKey;
    (void)autoinc;
    (void)rc_meta;
    (void)err;
    (void)rc_load_off;
    (void)rc_shared_off;
    (void)db;

    // API sequence test completed successfully
    return 66;
}