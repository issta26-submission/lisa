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
//<ID> 599
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_str *builder = NULL;
    char *sql = NULL;
    char buffer[256];
    const char *tail = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_bind = 0;
    int autocommit = 0;

    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    rc_open = sqlite3_open(":memory:", &db);
    sqlite3_snprintf(sizeof(buffer), buffer, "CREATE TABLE IF NOT EXISTS t(id INTEGER PRIMARY KEY, value TEXT);");
    rc_prep = sqlite3_prepare_v3(db, buffer, -1, 0, &stmt, &tail);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);

    builder = sqlite3_str_new(db);
    sqlite3_str_vappendf(builder, "INSERT INTO t(value) VALUES(?);", NULL);
    sql = sqlite3_str_finish(builder);
    builder = NULL;

    // step 3: Operate / Validate
    rc_prep = sqlite3_prepare_v3(db, sql, -1, 0, &stmt, NULL);
    rc_bind = sqlite3_bind_text(stmt, 1, "hello", -1, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);

    rc_prep = sqlite3_prepare_v3(db, "SELECT id, value FROM t;", -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    autocommit = sqlite3_get_autocommit(db);
    rc_finalize = sqlite3_finalize(stmt);

    // step 4: Cleanup
    sqlite3_realloc(sql, 0);
    rc_open = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_bind;
    (void)autocommit;
    (void)tail;
    (void)stmt;
    (void)builder;
    (void)db;
    (void)buffer;

    // API sequence test completed successfully
    return 66;
}