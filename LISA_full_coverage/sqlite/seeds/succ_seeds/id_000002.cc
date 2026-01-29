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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    const char *ver = sqlite3_libversion();
    int threadsafe_flag = sqlite3_threadsafe();
    int kw_count = sqlite3_keyword_count();
    const char *kw_name = NULL;
    int kw_len = 0;
    int rc_kw = sqlite3_keyword_name(0, &kw_name, &kw_len);
    int compile_used = sqlite3_compileoption_used("DEBUG");
    char *mp = sqlite3_mprintf("%s-%d", ver, compile_used);
    char buf[128];
    char *sn_res = sqlite3_snprintf((int)sizeof(buf), buf, "%s", mp);
    int strnicmp_res = sqlite3_strnicmp("ABC", "abc", 3);
    int strglob_res = sqlite3_strglob("a*", "abc");
    int complete_res = sqlite3_complete("SELECT 1;");
    int complete16_res = sqlite3_complete16((const void *)L"SELECT 1;");
    void *realloc_res = sqlite3_realloc(mp, 0);
    sqlite3_thread_cleanup();
    (void)ver;
    (void)threadsafe_flag;
    (void)kw_count;
    (void)kw_name;
    (void)kw_len;
    (void)rc_kw;
    (void)compile_used;
    (void)sn_res;
    (void)strnicmp_res;
    (void)strglob_res;
    (void)complete_res;
    (void)complete16_res;
    (void)realloc_res;
    return 66;
}