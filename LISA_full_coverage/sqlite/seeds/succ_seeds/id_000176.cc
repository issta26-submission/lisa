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
//<ID> 176
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Initialize sqlite and configure shared cache
    sqlite3 *db = NULL;
    int rc_init = sqlite3_initialize();
    int rc_shared_enable = sqlite3_enable_shared_cache(1);

    // step 2: Open an in-memory database and enable loadable extensions
    int rc_open = sqlite3_open(":memory:", &db);
    int rc_load_enable = sqlite3_enable_load_extension(db, 1);

    // step 3: Operate - create a table and insert a row
    const char *sql_create_insert = "CREATE TABLE meta_test(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL); INSERT INTO meta_test(name) VALUES('Alice');";
    int rc_exec_create = sqlite3_exec(db, sql_create_insert, 0, 0, 0);

    // step 4: Validate metadata and perform cleanup operations
    const char *pzDataType = NULL;
    const char *pzCollSeq = NULL;
    int pNotNull = 0;
    int pPrimaryKey = 0;
    int pAutoinc = 0;
    int rc_meta = sqlite3_table_column_metadata(db, "main", "meta_test", "id", &pzDataType, &pzCollSeq, &pNotNull, &pPrimaryKey, &pAutoinc);
    int rc_err = sqlite3_errcode(db);
    int rc_exec_drop = sqlite3_exec(db, "DROP TABLE IF EXISTS meta_test;", 0, 0, 0);
    int rc_load_disable = sqlite3_enable_load_extension(db, 0);
    int rc_shared_disable = sqlite3_enable_shared_cache(0);

    (void)rc_init;
    (void)rc_shared_enable;
    (void)rc_open;
    (void)rc_load_enable;
    (void)rc_exec_create;
    (void)pzDataType;
    (void)pzCollSeq;
    (void)pNotNull;
    (void)pPrimaryKey;
    (void)pAutoinc;
    (void)rc_meta;
    (void)rc_err;
    (void)rc_exec_drop;
    (void)rc_load_disable;
    (void)rc_shared_disable;
    (void)db;

    // API sequence test completed successfully
    return 66;
}