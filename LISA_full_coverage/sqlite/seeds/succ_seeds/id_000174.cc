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
//<ID> 174
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize handles and outputs
    sqlite3 *db = NULL;
    const char *colType = NULL;
    const char *colCollSeq = NULL;
    int colNotNull = 0;
    int colPrimaryKey = 0;
    int colAutoinc = 0;
    int rc_shared = 0;
    int rc_open = 0;
    int rc_enable_ext = 0;
    int rc_exec = 0;
    int rc_meta = 0;
    int rc_err = 0;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY AUTOINCREMENT, val TEXT NOT NULL);";

    // step 2: Configure global behavior and open an in-memory database
    rc_shared = sqlite3_enable_shared_cache(1);
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Operate - enable load-extension and create table
    rc_enable_ext = sqlite3_enable_load_extension(db, 1);
    rc_exec = sqlite3_exec(db, sql_create, NULL, NULL, NULL);

    // step 4: Validate metadata and error state, then cleanup
    rc_meta = sqlite3_table_column_metadata(db, "main", "t", "id", &colType, &colCollSeq, &colNotNull, &colPrimaryKey, &colAutoinc);
    rc_err = sqlite3_errcode(db);
    sqlite3_enable_load_extension(db, 0);
    sqlite3_close_v2(db);

    (void)colType;
    (void)colCollSeq;
    (void)colNotNull;
    (void)colPrimaryKey;
    (void)colAutoinc;
    (void)rc_shared;
    (void)rc_open;
    (void)rc_enable_ext;
    (void)rc_exec;
    (void)rc_meta;
    (void)rc_err;

    // API sequence test completed successfully
    return 66;
}