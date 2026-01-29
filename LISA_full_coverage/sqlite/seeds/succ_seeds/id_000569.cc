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
//<ID> 569
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    int rc_os_init = sqlite3_os_init();
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_str *builder = NULL;
    char *str_val = NULL;
    const char *text = "hello";
    sqlite3_uint64 text_len = (sqlite3_uint64)5;
    int rc_bind = 0;
    int rc_step = 0;
    int rc_blob_close = 0;

    // step 2: Configure
    rc_bind = sqlite3_bind_text64(stmt, 1, text, text_len, NULL, (unsigned char)1);
    str_val = sqlite3_str_value(builder);

    // step 3: Operate / Validate
    rc_step = sqlite3_step(stmt);

    // step 4: Cleanup
    rc_blob_close = sqlite3_blob_close(blob);

    (void)rc_os_init;
    (void)rc_bind;
    (void)str_val;
    (void)rc_step;
    (void)rc_blob_close;
    (void)stmt;
    (void)builder;
    (void)blob;
    (void)text;
    (void)text_len;

    // API sequence test completed successfully
    return 66;
}