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
//<ID> 544
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    const char *sql_create = "CREATE TABLE t1(id INTEGER);";
    const char *sql_insert = "INSERT INTO t1(id) VALUES(42);";
    const char *sql_select = "SELECT id FROM t1;";
    int rc_open = 0;
    int rc_exec = 0;
    const char *pzDataType = NULL;
    const char *pzCollSeq = NULL;
    int pNotNull = 0;
    int pPrimaryKey = 0;
    int pAutoinc = 0;
    void *mem = NULL;
    sqlite3_uint64 alloc_size = (sqlite3_uint64)128;
    const char *table_name = NULL;
    sqlite3_int64 total_changes = 0;
    int val_type = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open(":memory:", &db);
    rc_exec = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    rc_exec = sqlite3_exec(db, sql_insert, NULL, NULL, NULL);

    // step 3: Operate / Validate
    sqlite3_enable_load_extension(db, 1);
    /* Obtain column metadata for table t1 column id. */
    rc_exec = sqlite3_table_column_metadata(db, "main", "t1", "id", &pzDataType, &pzCollSeq, &pNotNull, &pPrimaryKey, &pAutoinc);
    table_name = sqlite3_db_name(db, 0);
    total_changes = sqlite3_total_changes64(db);
    mem = sqlite3_realloc64(NULL, alloc_size);
    /* Use a safe, available API to get an integer value */
    val_type = sqlite3_keyword_count();

    // step 4: Cleanup
    sqlite3_enable_load_extension(db, 0);
    mem = sqlite3_realloc64(mem, (sqlite3_uint64)0);
    sqlite3_db_release_memory(db);

    (void)rc_open;
    (void)rc_exec;
    (void)pzDataType;
    (void)pzCollSeq;
    (void)pNotNull;
    (void)pPrimaryKey;
    (void)pAutoinc;
    (void)mem;
    (void)alloc_size;
    (void)table_name;
    (void)total_changes;
    (void)val_type;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;

    // API sequence test completed successfully
    return 66;
}