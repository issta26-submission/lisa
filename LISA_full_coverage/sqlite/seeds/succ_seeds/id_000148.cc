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
//<ID> 148
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    int rc_open = sqlite3_open(":memory:", &db);

    const char *dbname = sqlite3_db_name(db, 0);
    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendchar(str, 5, 'A');
    sqlite3_str_appendf(str, " dbname=%s", dbname);
    sqlite3_str_appendchar(str, 1, '!');
    sqlite3_str_reset(str);

    int rc_release = sqlite3_db_release_memory(db);
    const char *libver = sqlite3_libversion();
    sqlite3_int64 hw = sqlite3_memory_highwater(0);

    int rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)dbname;
    (void)str;
    (void)rc_release;
    (void)libver;
    (void)hw;
    (void)rc_close;

    return 66;
}