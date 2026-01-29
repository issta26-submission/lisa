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
//<ID> 518
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
    const char *sql = "SELECT 123 AS value";
    char *expanded_sql = NULL;
    void *buffer = NULL;
    sqlite3_uint64 alloc_size = 0;
    int rc_open = 0;
    int rc_prep = 0;
    int db_err = 0;
    size_t sql_len = 0;

    // step 2: Initialize / Prepare
    rc_open = sqlite3_open16((const void*)L":memory:", &db);
    rc_prep = sqlite3_prepare_v3(db, sql, -1, 0, &stmt, NULL);

    // step 3: Operate / Use APIs (data flows between calls)
    sql_len = strlen(sql);
    alloc_size = (sqlite3_uint64)(sql_len + 1);

    // allocate two separate buffers so both can be freed independently
    expanded_sql = (char *)sqlite3_realloc(NULL, (int)alloc_size);
    buffer = sqlite3_realloc(NULL, (int)alloc_size);

    // copy the SQL into expanded_sql and then into buffer
    sqlite3_snprintf((int)alloc_size, expanded_sql, "%s", sql);
    memcpy(buffer, expanded_sql, sql_len + 1);

    db_err = sqlite3_extended_errcode(db);

    // step 4: Cleanup / Finalize
    sqlite3_free(expanded_sql);
    sqlite3_free(buffer);
    sqlite3_close(db);

    (void)rc_open;
    (void)rc_prep;
    (void)db_err;
    (void)alloc_size;
    (void)sql_len;
    (void)sql;
    (void)stmt;

    // API sequence test completed successfully
    return 66;
}