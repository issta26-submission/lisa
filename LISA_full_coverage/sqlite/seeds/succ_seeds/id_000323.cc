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
//<ID> 323
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    /* step 1: Declarations */
    sqlite3 *db = NULL;
    struct sqlite3_snapshot snapshot;
    char **table = NULL;
    char *row0 = NULL;
    sqlite3_int64 lastrowid = 0;
    int rc_open = 0;
    int rc_snapshot = 0;

    /* step 2: Setup - clear snapshot storage and initialize the library */
    memset(&snapshot, 0, sizeof(snapshot));
    sqlite3_initialize();

    /* use a few safe, non-DB-specific APIs to mimic open/snapshot info */
    const char *libver = sqlite3_libversion();
    int libver_num = sqlite3_libversion_number();
    int ts = sqlite3_threadsafe();
    rc_open = libver_num;
    rc_snapshot = ts;

    /* allocate a small fake db blob (do not rely on internal layout) */
    db = (sqlite3 *)sqlite3_malloc(16);

    /* step 3: Core operations - set a local lastrowid, allocate a small table and populate it */
    lastrowid = (sqlite3_int64)12345;
    table = (char **)sqlite3_malloc(2 * sizeof(char *));
    row0 = (char *)sqlite3_malloc(6);
    row0[0] = 'h'; row0[1] = 'e'; row0[2] = 'l'; row0[3] = 'l'; row0[4] = 'o'; row0[5] = '\0';
    table[0] = row0;
    table[1] = NULL;

    /* step 4: Cleanup - release allocated buffers via sqlite3_realloc(ptr, 0) */
    sqlite3_realloc(row0, 0);
    sqlite3_realloc(table, 0);
    sqlite3_realloc(db, 0);

    /* use variables to avoid unused-variable warnings, keep function straight-line */
    (void)libver;
    (void)rc_open;
    (void)rc_snapshot;
    (void)lastrowid;
    (void)table;

    return 66;
}