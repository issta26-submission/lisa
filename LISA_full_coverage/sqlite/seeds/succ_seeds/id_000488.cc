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
//<ID> 488
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    /* Declarations */
    void *p1 = NULL;
    void *p2 = NULL;
    sqlite3_uint64 size1 = 0;
    sqlite3_uint64 size2 = 0;
    sqlite3_int64 mem_before = 0;
    sqlite3_int64 mem_after = 0;
    sqlite3_int64 mem_high = 0;
    sqlite3_int64 prev_limit = 0;
    int libver = 0;
    int threadsafe = 0;
    const char *source_id = NULL;

    /* Straight-line sequence of safe API calls */
    mem_before = sqlite3_memory_used();
    p1 = sqlite3_malloc64((sqlite3_uint64)16);
    size1 = sqlite3_msize(p1);
    p2 = sqlite3_malloc64((sqlite3_uint64)32);
    size2 = sqlite3_msize(p2);
    mem_after = sqlite3_memory_used();
    mem_high = sqlite3_memory_highwater(0);
    prev_limit = sqlite3_soft_heap_limit64((sqlite3_int64)1024 * 1024);
    libver = sqlite3_libversion_number();
    source_id = sqlite3_sourceid();
    threadsafe = sqlite3_threadsafe();
    sqlite3_realloc(p2, 0);
    sqlite3_realloc(p1, 0);

    /* Silence unused-variable warnings */
    (void)size1;
    (void)size2;
    (void)mem_before;
    (void)mem_after;
    (void)mem_high;
    (void)prev_limit;
    (void)libver;
    (void)threadsafe;
    (void)source_id;

    return 66;
}