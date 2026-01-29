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
//<ID> 150
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    int rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    char *errmsg = NULL;
    const char *sql_setup = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT); INSERT INTO t(val) VALUES('alpha');";
    sqlite3_str *s = NULL;
    int rc_exec = 0;
    int rc_checkpoint = 0;
    int nLog = 0;
    int nCkpt = 0;
    sqlite3_blob *blob = NULL;

    // step 2: Configure - create table and insert a row using sqlite3_exec
    rc_exec = sqlite3_exec(db, sql_setup, (int (*)(void *, int, char **, char **))NULL, NULL, &errmsg);

    // step 3: Operate / Validate - perform WAL checkpoint, request DB to release memory, and append to an sqlite3_str
    rc_checkpoint = sqlite3_wal_checkpoint_v2(db, "main", 0, &nLog, &nCkpt);
    int rc_release = sqlite3_db_release_memory(db);
    s = (sqlite3_str *)sqlite3_realloc(NULL, 64);
    sqlite3_str_appendf(s, "Inserted row count: %d");
    int rc_blob_close = sqlite3_blob_close(blob);

    // step 4: Cleanup - free allocated sqlite3_str memory, close DB, silence unused warnings
    sqlite3_realloc(s, 0);
    int rc_close = sqlite3_close_v2(db);
    (void)rc_open;
    (void)errmsg;
    (void)rc_exec;
    (void)rc_checkpoint;
    (void)rc_release;
    (void)nLog;
    (void)nCkpt;
    (void)rc_blob_close;
    (void)rc_close;

    return 66; // API sequence test completed successfully
}