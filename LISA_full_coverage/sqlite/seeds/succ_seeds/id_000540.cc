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
//<ID> 540
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_sqlite3_api_sequence() {
    /* step 1: Declarations / Initialize */
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_stmt *stmt_sel = NULL;
    const char *sql_create = "CREATE TABLE t(a INTEGER);";
    const char *sql_insert = "INSERT INTO t(a) VALUES(?);";
    const char *sql_select = "SELECT a FROM t;";
    int rc_init = 0;
    int ver = 0;
    int used_opt = 0;
    int cmp = 0;
    int cmpn = 0;
    void *mem = NULL;
    void *mem2 = NULL;
    sqlite3_uint64 size1 = 0;
    sqlite3_uint64 size2 = 0;
    sqlite3_filename fname = NULL;
    sqlite3_int64 uri_val = 0;
    int comp16 = 0;

    /* step 2: Straight-line API calls using only available exports */
    rc_init = sqlite3_initialize();
    ver = sqlite3_libversion_number();
    used_opt = sqlite3_compileoption_used("ENABLE_FTS3");
    cmp = sqlite3_stricmp("Hello", "hello");
    cmpn = sqlite3_strnicmp("AbCd", "abCD", 4);
    mem = sqlite3_realloc64(NULL, (sqlite3_uint64)64);
    size1 = sqlite3_msize(mem);
    mem2 = sqlite3_malloc(32);
    size2 = sqlite3_msize(mem2);
    /* free memory by reallocating to size 0 */
    mem = sqlite3_realloc64(mem, 0);
    mem2 = sqlite3_realloc64(mem2, 0);
    sqlite3_soft_heap_limit64(1024);
    sqlite3_release_memory(1);
    sqlite3_reset_auto_extension();
    fname = sqlite3_create_filename("dbfile", "journalfile", "walfile", 0, (const char **)NULL);
    uri_val = sqlite3_uri_int64(fname, "cache_size", (sqlite3_int64)128);
    comp16 = sqlite3_complete16((const void *)L"SELECT 1;");
    /* avoid unused warnings for original variables and strings */
    (void)db;
    (void)stmt;
    (void)stmt_sel;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;
    (void)rc_init;
    (void)ver;
    (void)used_opt;
    (void)cmp;
    (void)cmpn;
    (void)mem;
    (void)mem2;
    (void)size1;
    (void)size2;
    (void)fname;
    (void)uri_val;
    (void)comp16;

    /* API sequence test completed successfully */
    return 66;
}