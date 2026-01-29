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
//<ID> 87
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
    sqlite3_blob *blob = NULL;
    const char *pzTail = NULL;
    char *expanded = NULL;
    const char *sql = "CREATE TABLE t(a BLOB); INSERT INTO t(a) VALUES(zeroblob(4));";
    int rc_open = 0;
    int rc_prep1 = 0;
    int rc_step1 = 0;
    int rc_prep2 = 0;
    int rc_step2 = 0;
    int rc_blob_open = 0;
    int rc_blob_write = 0;
    int rc_blob_close = 0;
    int opt_used = 0;

    // step 2: Configure
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep1 = sqlite3_prepare_v2(db, sql, -1, &stmt1, &pzTail);

    // step 3: Operate / Validate
    rc_step1 = sqlite3_step(stmt1);
    rc_prep2 = sqlite3_prepare_v2(db, pzTail, -1, &stmt2, &pzTail);
    rc_step2 = sqlite3_step(stmt2);
    rc_blob_open = sqlite3_blob_open(db, "main", "t", "a", 1, 1, &blob);
    rc_blob_write = sqlite3_blob_write(blob, (const void *)"ABCD", 4, 0);
    rc_blob_close = sqlite3_blob_close(blob);
    expanded = sqlite3_expanded_sql(stmt2);
    expanded = (char*)sqlite3_realloc(expanded, 0);
    opt_used = sqlite3_compileoption_used("THREADSAFE");

    // step 4: Cleanup
    sqlite3_close_v2(db);
    (void)rc_open;
    (void)rc_prep1;
    (void)rc_step1;
    (void)rc_prep2;
    (void)rc_step2;
    (void)rc_blob_open;
    (void)rc_blob_write;
    (void)rc_blob_close;
    (void)opt_used;
    (void)stmt1;
    (void)stmt2;
    (void)pzTail;
    (void)expanded;
    (void)blob;
    (void)sql;

    // API sequence test completed successfully
    return 66;
}