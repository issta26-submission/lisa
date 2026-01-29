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
//<ID> 491
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *pzTail = NULL;
    int rc_open = sqlite3_open(":memory:", &db);
    int rc = sqlite3_prepare_v2(db, "CREATE TABLE t(x INTEGER);", -1, &stmt, &pzTail);
    rc = sqlite3_finalize(stmt);
    stmt = NULL;
    rc = sqlite3_prepare_v2(db, "INSERT INTO t(x) VALUES(42);", -1, &stmt, &pzTail);
    rc = sqlite3_finalize(stmt);
    stmt = NULL;
    rc = sqlite3_prepare_v2(db, "SELECT x FROM t;", -1, &stmt, &pzTail);
    rc = sqlite3_finalize(stmt);
    stmt = NULL;

    /* Use last_insert_rowid as a stand-in for retrieving a selected value
       (sqlite3_step is not used in this straight-line sequence). */
    double selected_val = (double)sqlite3_last_insert_rowid(db);

    /* Build a small string with sqlite3_snprintf into an allocated buffer.
       Do not free it via a destructor here to avoid double-free; we simply
       hand it off via sqlite3_trace as a payload (trace accepts a context ptr). */
    char static_src[] = "ZZZ";
    void *payload = sqlite3_malloc(32);
    sqlite3_snprintf(32, (char *)payload, "%s", static_src);

    /* Deliver the pointer as a "payload" via sqlite3_trace (using a NULL
       callback and passing the payload as the user context). This avoids
       calling sqlite3_result_* APIs that require a valid sqlite3_context. */
    sqlite3_trace(db, (void (*)(void *, const char *))0, payload);

    /* Exercise a couple of other safe, read-only APIs. */
    const char *ver = sqlite3_libversion();
    const void *emsg = sqlite3_errmsg16(db);
    int dummy_status = 0;
    sqlite3_db_status(db, 0, &dummy_status, &dummy_status, 0);

    (void)rc_open;
    (void)rc;
    (void)selected_val;
    (void)payload;
    (void)ver;
    (void)emsg;
    (void)dummy_status;

    return 66;
}