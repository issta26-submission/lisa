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
//<ID> 9
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = 0;
    sqlite3_stmt *stmt = 0;
    sqlite3_stmt *next_stmt = 0;
    sqlite3_file *file_obj = 0;
    sqlite3_value *val = 0;
    const char *db_name = 0;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_val_frombind = 0;

    /* Step 1: allocate some memory and query library info */
    void *mem = sqlite3_malloc64(128);
    rc_open = sqlite3_libversion_number();

    /* Step 2: mix in some randomness and grow the allocation */
    sqlite3_randomness(16, mem);
    void *mem2 = sqlite3_realloc64(mem, (sqlite3_uint64)256);

    /* Step 3: string/completion/keyword related APIs (no DB handle required) */
    sqlite3_int64 hh = sqlite3_hard_heap_limit64(0);
    int cmp1 = sqlite3_stricmp("alpha", "beta");
    int cmp2 = sqlite3_strnicmp("abcde", "abzzz", 3);
    int comp = sqlite3_complete("SELECT 1");
    int comp16 = sqlite3_complete16((const void *)"SELECT 1");
    rc_prep = sqlite3_keyword_check("select", 6);
    const char *kwname = 0;
    int kwlen = 0;
    rc_val_frombind = sqlite3_keyword_name(1, &kwname, &kwlen);

    /* Step 4: logging/sleep and further randomness, then shrink/free via realloc to zero */
    sqlite3_log(0, "sqlite3 test sequence");
    sqlite3_sleep(1);
    sqlite3_randomness(8, mem2);
    void *mem3 = sqlite3_realloc64(mem2, (sqlite3_uint64)0);

    /* Query the allocation size (may be 0 if freed) to exercise msize API */
    sqlite3_uint64 msize_val = sqlite3_msize(mem3);

    /* Silence unused-variable warnings, keep the original variables present */
    (void)db;
    (void)stmt;
    (void)next_stmt;
    (void)file_obj;
    (void)val;
    (void)db_name;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_val_frombind;
    (void)hh;
    (void)cmp1;
    (void)cmp2;
    (void)comp;
    (void)comp16;
    (void)kwname;
    (void)kwlen;
    (void)mem3;
    (void)msize_val;

    return 66;
}