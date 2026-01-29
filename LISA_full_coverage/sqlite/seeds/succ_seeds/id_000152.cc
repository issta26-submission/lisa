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
//<ID> 152
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
    const char *sql_setup = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT); INSERT INTO t(val) VALUES('hello');";

    // step 2: Prepare and step a statement (db is NULL by design for this sequence)
    int rc_prepare = sqlite3_prepare_v2(db, sql_setup, -1, &stmt, NULL);
    int rc_step = sqlite3_step(stmt);

    // step 3: Inspect prepared statement and compile options
    char *expanded = sqlite3_expanded_sql(stmt);
    const char *opt0 = sqlite3_compileoption_get(0);

    // step 4: Checkpoint WAL and release memory
    int rc_ckpt = sqlite3_wal_checkpoint(db, "main");
    int rc_release = sqlite3_db_release_memory(db);

    // step 5: Operate on a blob pointer (NULL) and format a message string
    sqlite3_blob *blob = NULL;
    int rc_blob_read = sqlite3_blob_read(blob, NULL, 0, 0);
    char *msg = sqlite3_mprintf("Sample: %s", opt0 ? opt0 : "none");

    // step 6: Query some global info and close the DB (db is NULL here)
    sqlite3_int64 mem_used = sqlite3_memory_used();
    sqlite3_int64 lastid = sqlite3_last_insert_rowid(db);
    int rc_close = sqlite3_close_v2(db);

    // silence unused-variable warnings
    (void)rc_prepare;
    (void)rc_step;
    (void)expanded;
    (void)opt0;
    (void)rc_ckpt;
    (void)rc_release;
    (void)blob;
    (void)rc_blob_read;
    (void)msg;
    (void)mem_used;
    (void)lastid;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}