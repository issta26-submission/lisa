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
//<ID> 190
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    const char *src = sqlite3_sourceid();
    const char *opt0 = sqlite3_compileoption_get(0);
    int threadsafe = sqlite3_threadsafe();
    sqlite3_int64 mem_used = sqlite3_memory_used();
    sqlite3_int64 cur64 = 0;
    sqlite3_int64 high64 = 0;
    sqlite3_status64(0, &cur64, &high64, 0);
    sqlite3_int64 soft_limit = sqlite3_soft_heap_limit64((sqlite3_int64)1024);
    void *mem = sqlite3_malloc64((sqlite3_uint64)128);
    sqlite3_randomness(16, mem);
    int cmp_n = sqlite3_strnicmp("Abc", "aBc", 3);
    int glob = sqlite3_strglob("t*", "test");
    char buf[128];
    sqlite3_snprintf(sizeof(buf), buf, "%s", src);
    int cur_int = 0;
    int high_int = 0;
    int st_status = sqlite3_status(0, &cur_int, &high_int, 0);
    int rc_enable = sqlite3_enable_shared_cache(1);
    int rc_config = sqlite3_config(0);
    int rc_auto = sqlite3_auto_extension((void(*)())0);
    sqlite3_mutex *m = sqlite3_mutex_alloc(0);
    int mtry = sqlite3_mutex_try(m);

    (void)opt0;
    (void)threadsafe;
    (void)mem_used;
    (void)cur64;
    (void)high64;
    (void)soft_limit;
    (void)mem;
    (void)cmp_n;
    (void)glob;
    (void)buf;
    (void)cur_int;
    (void)high_int;
    (void)st_status;
    (void)rc_enable;
    (void)rc_config;
    (void)rc_auto;
    (void)m;
    (void)mtry;

    return 66;
}