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
//<ID> 498
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    const char *tail = NULL;
    int rc_open = 0;
    int rc_prepare_create = 0;
    int rc_step_create = 0;
    int rc_prepare_insert = 0;
    int rc_step_insert = 0;
    char *expanded_sql = NULL;
    sqlite3_str *pstr = (sqlite3_str *)malloc(64);
    void *payload = NULL;

    // step 2: Setup / Initialize
    rc_open = sqlite3_open(":memory:", &db);
    rc_prepare_create = sqlite3_prepare_v2(db, "CREATE TABLE demo(a INTEGER);", -1, &stmt_create, &tail);

    // step 3: Operate / Validate
    rc_step_create = sqlite3_step(stmt_create);
    expanded_sql = sqlite3_expanded_sql(stmt_create);
    sqlite3_str_appendchar(pstr, 3, 'Z');
    payload = (void *)expanded_sql;
    sqlite3_result_pointer(NULL, payload, "expanded", sqlite3_free);
    rc_prepare_insert = sqlite3_prepare_v2(db, "INSERT INTO demo(a) VALUES(123);", -1, &stmt_insert, &tail);
    rc_step_insert = sqlite3_step(stmt_insert);

    // step 4: Cleanup / Finalize
    sqlite3_reset(stmt_create);
    sqlite3_reset(stmt_insert);
    sqlite3_close_v2(db);
    free(pstr);

    (void)rc_open;
    (void)rc_prepare_create;
    (void)rc_step_create;
    (void)rc_prepare_insert;
    (void)rc_step_insert;
    (void)stmt_create;
    (void)stmt_insert;
    (void)tail;
    (void)payload;

    // API sequence test completed successfully
    return 66;
}