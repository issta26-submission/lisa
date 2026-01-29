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
//<ID> 86
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    const char *pzTail = NULL;
    char *expanded = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_blob_open = 0;
    int rc_blob_close = 0;
    int rc_close = 0;
    int compile_used = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open(":memory:", &db);
    compile_used = sqlite3_compileoption_used("ENABLE_FTS5");

    // step 3: Operate / Validate
    rc_prepare = sqlite3_prepare_v2(db, "CREATE TABLE t(a); INSERT INTO t VALUES(1); SELECT a FROM t;", -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prepare = sqlite3_prepare_v2(db, pzTail, -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prepare = sqlite3_prepare_v2(db, pzTail, -1, &stmt, &pzTail);
    expanded = sqlite3_expanded_sql(stmt);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;
    rc_blob_open = sqlite3_blob_open(db, "main", "t", "a", 1, 0, &blob);
    rc_blob_close = sqlite3_blob_close(blob);

    // step 4: Cleanup
    if (expanded) { sqlite3_free(expanded); expanded = NULL; }
    rc_close = sqlite3_close_v2(db);
    (void)rc_open;
    (void)compile_used;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_blob_open;
    (void)rc_blob_close;
    (void)rc_close;
    (void)blob;
    (void)stmt;
    (void)pzTail;

    // API sequence test completed successfully
    return 66;
}