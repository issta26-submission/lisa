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
//<ID> 583
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    int rc_open = sqlite3_open(":memory:", &db);
    sqlite3_str *builder = sqlite3_str_new(db);
    const char *snippet = "SELECT 1;"; /* length = 9 */
    int snippet_len = 9;
    int complete_before = sqlite3_complete(snippet);
    int threadsafe = sqlite3_threadsafe();
    void *alloc_mem = sqlite3_realloc(NULL, 64);

    // step 2: Configure
    sqlite3_str_append(builder, snippet, snippet_len);
    char *final_sql = sqlite3_str_finish(builder);

    // step 3: Operate / Validate
    int complete_after = sqlite3_complete(final_sql);
    sqlite3_result_null((sqlite3_context *)NULL);
    void *shrunk_mem = sqlite3_realloc(alloc_mem, 32);

    // step 4: Cleanup
    sqlite3_realloc(shrunk_mem, 0);
    sqlite3_realloc(final_sql, 0);
    int rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)complete_before;
    (void)threadsafe;
    (void)complete_after;
    (void)rc_close;
    (void)builder;
    (void)snippet;
    (void)snippet_len;

    // API sequence test completed successfully
    return 66;
}