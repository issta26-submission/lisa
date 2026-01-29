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
//<ID> 172
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    const char *create_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT); INSERT INTO t(val) VALUES('hello');";
    int rc_open = sqlite3_open(":memory:", &db);

    // step 2: Configure
    int rc_shared_enable = sqlite3_enable_shared_cache(1);
    int rc_load_ext_on = sqlite3_enable_load_extension(db, 1);

    // step 3: Operate
    int rc_exec = sqlite3_exec(db, create_sql, 0, 0, 0);
    const char *colType = NULL;
    const char *collSeq = NULL;
    int notNull = 0;
    int primaryKey = 0;
    int autoinc = 0;
    int rc_meta = sqlite3_table_column_metadata(db, "main", "t", "id", &colType, &collSeq, &notNull, &primaryKey, &autoinc);

    // step 4: Validate / Cleanup
    int last_err = sqlite3_errcode(db);
    int rc_load_ext_off = sqlite3_enable_load_extension(db, 0);
    int rc_shared_disable = sqlite3_enable_shared_cache(0);

    (void)rc_open;
    (void)rc_shared_enable;
    (void)rc_load_ext_on;
    (void)rc_exec;
    (void)colType;
    (void)collSeq;
    (void)notNull;
    (void)primaryKey;
    (void)autoinc;
    (void)rc_meta;
    (void)last_err;
    (void)rc_load_ext_off;
    (void)rc_shared_disable;
    db = NULL;

    // API sequence test completed successfully
    return 66;
}