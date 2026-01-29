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
//<ID> 594
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_str *builder = NULL;
    sqlite3_stmt *stmt = NULL;
    char sqlbuf[128];
    char *sql_final = NULL;
    void *mem = NULL;
    int rc_bind = 0;
    int autocommit = 0;

    // step 2: Setup / Configure
    builder = sqlite3_str_new(db);
    sql_final = sqlite3_mprintf("INSERT INTO t VALUES('sample');");
    memset(sqlbuf, 0, sizeof(sqlbuf));
    sqlite3_str_appendall(builder, sql_final);
    sqlite3_str_appendchar(builder, 1, 'A');
    builder = NULL;

    // step 3: Operate / Validate
    mem = sqlite3_realloc(NULL, 64);
    rc_bind = sqlite3_bind_pointer(stmt, 1, sqlbuf, "text", (void(*)(void *))0);
    autocommit = sqlite3_get_autocommit(db);

    // step 4: Cleanup
    sqlite3_realloc(sql_final, 0);
    sqlite3_realloc(mem, 0);
    (void)db;
    (void)stmt;
    (void)rc_bind;
    (void)autocommit;
    (void)sqlbuf;
    (void)sql_final;
    (void)builder;
    (void)mem;

    // API sequence test completed successfully
    return 66;
}