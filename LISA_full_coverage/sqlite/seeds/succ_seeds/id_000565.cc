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
//<ID> 565
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_str *builder = NULL;
    char *str_val = NULL;
    char buffer[32];
    int rc_init = 0;
    int rc_bind = 0;
    int rc_step = 0;
    int rc_blob_close = 0;

    // step 2: Setup / Configure
    rc_init = sqlite3_os_init();
    builder = (sqlite3_str *)NULL;
    str_val = sqlite3_str_value(builder);
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = 'x';
    buffer[1] = 'y';
    buffer[2] = 'z';

    // step 3: Operate / Validate
    rc_bind = sqlite3_bind_text64(stmt, 1, (str_val ? str_val : buffer), (sqlite3_uint64)3, (void (*)(void *))NULL, (unsigned char)1);
    rc_step = sqlite3_step(stmt);
    rc_blob_close = sqlite3_blob_close(blob);

    // step 4: Cleanup
    memset(buffer, 0, sizeof(buffer));
    (void)rc_init;
    (void)rc_bind;
    (void)rc_step;
    (void)rc_blob_close;
    (void)stmt;
    (void)blob;
    (void)builder;
    (void)str_val;

    // API sequence test completed successfully
    return 66;
}