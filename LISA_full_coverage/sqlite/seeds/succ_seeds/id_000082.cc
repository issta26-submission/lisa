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
//<ID> 82
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_stmt *stmt_select = NULL;
    sqlite3_blob *blob = NULL;
    const char *pzTail = NULL;
    char *expanded = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_blob_close = 0;
    int compile_used = 0;

    // step 2: Configure - open in-memory database and create a table, insert a row
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep = sqlite3_prepare_v2(db, "CREATE TABLE t(id INTEGER PRIMARY KEY, a BLOB);", -1, &stmt_create, &pzTail);
    rc_step = sqlite3_step(stmt_create);
    rc_prep = sqlite3_prepare_v2(db, "INSERT INTO t(a) VALUES(x'010203');", -1, &stmt_insert, &pzTail);
    rc_step = sqlite3_step(stmt_insert);

    // step 3: Operate / Validate - open a blob on the inserted row, prepare a select and get expanded SQL, check compile option
    rc_prep = sqlite3_prepare_v2(db, "SELECT a FROM t WHERE id=1;", -1, &stmt_select, &pzTail);
    expanded = sqlite3_expanded_sql(stmt_select);
    sqlite3_blob_open(db, "main", "t", "a", 1, 0, &blob);
    compile_used = sqlite3_compileoption_used("THREADSAFE");

    // step 4: Cleanup
    rc_blob_close = sqlite3_blob_close(blob);
    sqlite3_close_v2(db);
    if (expanded) sqlite3_free(expanded); // use sqlite3_free to release expanded SQL

    (void)stmt_create;
    (void)stmt_insert;
    (void)stmt_select;
    (void)pzTail;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_blob_close;
    (void)compile_used;

    // API sequence test completed successfully
    return 66;
}