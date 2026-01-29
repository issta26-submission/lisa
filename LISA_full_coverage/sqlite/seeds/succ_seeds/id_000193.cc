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
//<ID> 193
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    char **table_result = NULL;
    int nrow = 0;
    int ncol = 0;
    int rc_open = sqlite3_open(":memory:", &db);
    int rc_create = sqlite3_get_table(db, "CREATE TABLE t(id INTEGER PRIMARY KEY, info BLOB, txt TEXT);", &table_result, &nrow, &ncol, NULL);
    sqlite3_free_table(table_result);
    table_result = NULL;
    nrow = 0;
    ncol = 0;
    int rc_insert = sqlite3_get_table(db, "INSERT INTO t(info, txt) VALUES(x'4142', 'statictext');", &table_result, &nrow, &ncol, NULL);
    sqlite3_free_table(table_result);
    table_result = NULL;
    nrow = 0;
    ncol = 0;
    int rc_select = sqlite3_get_table(db, "SELECT txt FROM t;", &table_result, &nrow, &ncol, NULL);
    sqlite3_free_table(table_result);
    table_result = NULL;
    nrow = 0;
    ncol = 0;
    int rc_close = sqlite3_close(db);
    (void)rc_open;
    (void)rc_create;
    (void)rc_insert;
    (void)rc_select;
    (void)rc_close;
    (void)db;
    (void)table_result;
    (void)nrow;
    (void)ncol;
    return 66;
}