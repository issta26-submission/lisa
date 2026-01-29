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
//<ID> 326
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations (keep original names/types)
    sqlite3 *db = NULL;
    sqlite3_snapshot snap;
    char **table_result = NULL;
    char *inner_str = NULL;
    void *buffer = NULL;
    const void *filename16 = (const void *)L":memory:";
    int rc_open = 0;
    int rc_snapshot = 0;

    // step 2: Initialize library and zero snapshot
    rc_open = sqlite3_initialize();
    memset(&snap, 0, sizeof(snap));

    // step 3: Allocate and resize buffers, fill with data
    buffer = sqlite3_realloc(NULL, 64);
    memset(buffer, 0x41, 64);   // fill buffer with 'A'
    buffer = sqlite3_realloc(buffer, 128);
    memset(buffer, 0x42, 128);  // overwrite with 'B'
    inner_str = (char *)sqlite3_realloc(NULL, 16);
    memset(inner_str, 'X', 15);
    inner_str[15] = '\0';
    inner_str = (char *)sqlite3_realloc(inner_str, 0); // free inner_str and set to NULL (per sqlite3_realloc semantics)

    // mimic table result allocation and free using sqlite3_realloc
    table_result = (char **)sqlite3_realloc(NULL, sizeof(char *));
    table_result[0] = NULL;

    // step 4: Query some library info and memory statistics (non-db operations)
    rc_snapshot = (int)sqlite3_memory_used();
    sqlite3_release_memory(1);
    (void)sqlite3_sourceid();
    (void)sqlite3_libversion();
    (void)sqlite3_compileoption_get(0);
    (void)sqlite3_compileoption_used("dummy_option");

    // step 5: Cleanup - free allocated blocks and shutdown library
    sqlite3_realloc(table_result, 0); // free table_result
    sqlite3_realloc(buffer, 0);       // free buffer
    sqlite3_shutdown();

    // silence unused-but-intended variables
    (void)db;
    (void)filename16;
    (void)rc_open;
    (void)rc_snapshot;
    (void)snap;

    // API sequence test completed successfully
    return 66;
}