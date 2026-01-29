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
//<ID> 492
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3_str *str = NULL;
    char *mp = NULL;
    char *sval = NULL;
    const char *ver = NULL;
    int vernum = 0;
    unsigned long long msz = 0;
    int rc_err = 0;
    int cmp = 0;
    int like = 0;
    int kw = 0;
    int complete = 0;

    /* create and manipulate an sqlite3_str */
    str = sqlite3_str_new(NULL);
    sqlite3_str_append(str, "init", 4);
    sqlite3_str_appendchar(str, 3, 'Z');
    sval = sqlite3_str_value(str);
    rc_err = sqlite3_str_errcode(str);

    /* query library version info and simple string ops */
    ver = sqlite3_libversion();
    vernum = sqlite3_libversion_number();
    mp = sqlite3_mprintf("static"); /* use the single-arg form from the exported API list */
    msz = (unsigned long long)sqlite3_msize((void *)mp);
    cmp = sqlite3_strnicmp(sval, ver, 3);
    like = sqlite3_strlike("pat", "str", 0);
    kw = sqlite3_keyword_check("select", 6);
    complete = sqlite3_complete("SELECT 1;");

    /* some harmless utility calls */
    sqlite3_randomness(4, (void *)mp);
    sqlite3_sleep(0);
    sqlite3_thread_cleanup();

    (void)str;
    (void)mp;
    (void)sval;
    (void)ver;
    (void)vernum;
    (void)msz;
    (void)rc_err;
    (void)cmp;
    (void)like;
    (void)kw;
    (void)complete;

    return 66;
}