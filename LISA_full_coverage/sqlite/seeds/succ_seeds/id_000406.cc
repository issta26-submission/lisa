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
//<ID> 406
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    const char *sql_create = "CREATE TABLE t(a INTEGER);";
    const char *sql_insert = "INSERT INTO t(a) VALUES(7);";
    int rc_open = 0;
    int rc_exec1 = 0;
    int rc_exec2 = 0;
    sqlite3_int64 lastid_set = 123456789LL;
    sqlite3_int64 lastid_read = 0;
    int txn_state = 0;
    int release_ret = 0;
    char *buf = NULL;
    void *tmp = NULL;
    int rc_close = 0;
    char fmtbuf[128];
    memset(fmtbuf, 0, sizeof(fmtbuf));

    // step 2: Setup - open database and initialize schema/data
    rc_open = sqlite3_open(":memory:", &db);
    rc_exec1 = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    rc_exec2 = sqlite3_exec(db, sql_insert, NULL, NULL, NULL);

    // step 3: Operate - set and read last insert rowid, query txn state, allocate/manage memory
    sqlite3_set_last_insert_rowid(db, lastid_set);
    lastid_read = sqlite3_last_insert_rowid(db);
    txn_state = sqlite3_txn_state(db, "main");
    buf = (char *)sqlite3_realloc(NULL, 128);
    sqlite3_snprintf((int)sizeof(fmtbuf), fmtbuf, "lastid=%lld txn=%d", (long long)lastid_read, txn_state);
    sqlite3_snprintf(128, buf, "%s", fmtbuf);
    tmp = sqlite3_realloc(buf, 256);
    buf = (char *)tmp;
    release_ret = sqlite3_release_memory(1024);

    // step 4: Validate and Cleanup - close DB and tidy local resources
    rc_close = sqlite3_close_v2(db);
    db = NULL;
    sqlite3_realloc(buf, 0);

    (void)rc_open;
    (void)rc_exec1;
    (void)rc_exec2;
    (void)lastid_set;
    (void)lastid_read;
    (void)txn_state;
    (void)release_ret;
    (void)tmp;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}