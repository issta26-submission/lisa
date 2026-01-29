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
//<ID> 93
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    char *errmsg = NULL;
    char **results = NULL;
    int nrow = 0;
    int ncol = 0;
    int rc_open = sqlite3_open(":memory:", &db);
    int rc_create = sqlite3_get_table(db, "CREATE TABLE t(a INTEGER);", &results, &nrow, &ncol, &errmsg);
    sqlite3_free_table(results);
    sqlite3_free(errmsg);
    results = NULL;
    errmsg = NULL;
    int rc_insert = sqlite3_get_table(db, "INSERT INTO t(a) VALUES(12345);", &results, &nrow, &ncol, &errmsg);
    sqlite3_free_table(results);
    sqlite3_free(errmsg);
    results = NULL;
    errmsg = NULL;
    int rc_bad = sqlite3_get_table(db, "SELEC bad_sql;", &results, &nrow, &ncol, &errmsg);
    sqlite3_free_table(results);
    int errcode = sqlite3_errcode(db);
    int changes = sqlite3_changes(db);
    sqlite3_free(errmsg);
    (void)rc_open;
    (void)rc_create;
    (void)rc_insert;
    (void)rc_bad;
    (void)errcode;
    (void)changes;
    (void)db;
    (void)results;
    (void)nrow;
    (void)ncol;
    return 66;
}