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
//<ID> 482
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    /* Declarations (only using APIs/types from the provided list) */
    int rc_lib = sqlite3_libversion_number();
    int rc_test = sqlite3_test_control(0);
    int cur = 0;
    int high = 0;
    int rc_status = sqlite3_status(0, &cur, &high, 0);
    sqlite3_int64 prev_limit = sqlite3_soft_heap_limit64(0);
    int rc_mem_alarm = sqlite3_memory_alarm((void (*)(void *, sqlite3_int64, int))0, (void *)0, (sqlite3_int64)0);
    int rc_release = sqlite3_release_memory(0);
    const char *errstr = sqlite3_errstr(0);
    int complete = sqlite3_complete("");
    void *mem = sqlite3_malloc64((sqlite3_uint64)16);
    sqlite3_uint64 msz = sqlite3_msize(mem);
    sqlite3_free(mem);

    /* Silence unused-variable warnings without branching */
    (void)rc_lib;
    (void)rc_test;
    (void)rc_status;
    (void)cur;
    (void)high;
    (void)prev_limit;
    (void)rc_mem_alarm;
    (void)rc_release;
    (void)errstr;
    (void)complete;
    (void)msz;

    return 66;
}