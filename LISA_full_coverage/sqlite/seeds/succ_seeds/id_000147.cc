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
//<ID> 147
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    int rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    char *exec_errmsg = NULL;
    const char *last_errmsg = NULL;
    void *allocated = NULL;
    int rc_release = 0;
    void *freed_back = NULL;
    int rc_close = 0;

    // step 2: Configure - create a table and insert a row
    const char *setup_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT); INSERT INTO t(val) VALUES('hello');";
    int rc_exec = sqlite3_exec(db, setup_sql, NULL, NULL, &exec_errmsg);
    last_errmsg = sqlite3_errmsg(db);

    // step 3: Operate / Validate - allocate memory, reset a sqlite3_str, release some memory and set a result pointer
    allocated = sqlite3_malloc64(128);
    if (allocated) {
        memset(allocated, 0, 128);
    }
    sqlite3_str_reset((sqlite3_str *)allocated);
    rc_release = sqlite3_release_memory(1024);
    sqlite3_result_pointer((sqlite3_context *)NULL, allocated, "custom.ptr", (void (*)(void *))NULL);

    // step 4: Cleanup
    freed_back = sqlite3_realloc64(allocated, 0);
    allocated = NULL;
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)exec_errmsg;
    (void)last_errmsg;
    (void)rc_exec;
    (void)rc_release;
    (void)freed_back;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}