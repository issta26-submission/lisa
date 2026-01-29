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
//<ID> 325
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    sqlite3_snapshot snapshot;
    char **table = NULL;
    char *schema = NULL;
    const void *filename = (const void *)L":memory:";
    int rc_open = 0;
    int rc_cmp = 0;
    sqlite3_int64 lastid = (sqlite3_int64)12345;
    int errcode = 0;

    rc_open = sqlite3_open16(filename, &db);
    sqlite3_extended_result_codes(db, 1);
    table = (char **)sqlite3_malloc64(sizeof(char *));
    schema = (char *)sqlite3_malloc64(5);
    schema[0] = 'm';
    schema[1] = 'a';
    schema[2] = 'i';
    schema[3] = 'n';
    schema[4] = '\0';
    table[0] = schema;
    sqlite3_set_last_insert_rowid(db, lastid);
    rc_cmp = sqlite3_snapshot_cmp(&snapshot, &snapshot);
    errcode = sqlite3_errcode(db);
    sqlite3_free(table[0]);
    sqlite3_free(table);
    sqlite3_close(db);

    (void)rc_open;
    (void)rc_cmp;
    (void)errcode;
    (void)lastid;
    (void)filename;

    return 66;
}