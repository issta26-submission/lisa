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
//<ID> 89
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    sqlite3_stmt *stmt3 = NULL;
    sqlite3_blob *blob = NULL;
    const char *pzTail = NULL;
    char *expanded = NULL;
    int rc_open = 0;
    int rc_prep1 = 0;
    int rc_step1 = 0;
    int rc_prep2 = 0;
    int rc_step2 = 0;
    int rc_prep3 = 0;
    int rc_step3 = 0;
    int rc_blob_open = 0;
    int rc_blob_close = 0;
    int rc_close = 0;
    int rc_compileopt = 0;

    // step 2: Configure
    rc_compileopt = sqlite3_compileoption_used("ENABLE_FTS3");

    // step 3: Operate / Validate
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep1 = sqlite3_prepare_v2(db, "CREATE TABLE t(a BLOB);", -1, &stmt1, &pzTail);
    rc_step1 = sqlite3_step(stmt1);
    rc_prep2 = sqlite3_prepare_v2(db, "INSERT INTO t VALUES(x'0F');", -1, &stmt2, &pzTail);
    rc_step2 = sqlite3_step(stmt2);
    rc_prep3 = sqlite3_prepare_v2(db, "SELECT a FROM t;", -1, &stmt3, &pzTail);
    expanded = sqlite3_expanded_sql(stmt3);
    rc_step3 = sqlite3_step(stmt3);
    rc_blob_open = sqlite3_blob_open(db, "main", "t", "a", 1, 0, &blob);
    rc_blob_close = sqlite3_blob_close(blob);

    // step 4: Cleanup
    rc_close = sqlite3_close_v2(db);
    sqlite3_realloc(expanded, 0);
    (void)rc_compileopt;
    (void)rc_open;
    (void)rc_prep1;
    (void)rc_step1;
    (void)rc_prep2;
    (void)rc_step2;
    (void)rc_prep3;
    (void)rc_step3;
    (void)rc_blob_open;
    (void)rc_blob_close;
    (void)rc_close;
    (void)pzTail;
    (void)stmt1;
    (void)stmt2;
    (void)stmt3;
    (void)blob;

    // API sequence test completed successfully
    return 66;
}