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
//<ID> 566
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    int rc_init = sqlite3_os_init();
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_str *builder = NULL;
    char *str_val = NULL;
    int rc_bind = 0;
    int rc_step = 0;
    int rc_blob_close = 0;
    char buffer[16];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = 'h';
    buffer[1] = 'e';
    buffer[2] = 'l';
    buffer[3] = 'l';
    buffer[4] = 'o';

    // step 2: Setup / Configure
    rc_bind = sqlite3_bind_text64(stmt, 1, buffer, (sqlite3_uint64)5, (void(*)(void *))NULL, (unsigned char)1);

    // step 3: Operate / Validate
    rc_step = sqlite3_step(stmt);
    rc_blob_close = sqlite3_blob_close(blob);
    str_val = sqlite3_str_value(builder);

    // step 4: Cleanup
    (void)rc_init;
    (void)rc_bind;
    (void)rc_step;
    (void)rc_blob_close;
    (void)buffer;
    (void)str_val;
    (void)stmt;
    (void)blob;
    (void)builder;

    // API sequence test completed successfully
    return 66;
}