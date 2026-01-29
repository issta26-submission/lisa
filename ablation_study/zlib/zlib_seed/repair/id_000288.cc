#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 288
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    gzFile gz;
    const char *ver;
    char data[] = "zlib gz test: write, rewind, offset check.";
    unsigned int data_len = (unsigned int)(sizeof(data) - 1);
    int wrote1 = 0;
    off_t offset_after_write1 = 0;
    int rewind_ret = 0;
    off_t offset_after_rewind = 0;
    int wrote2 = 0;
    off_t offset_after_write2 = 0;
    int close_ret = 0;

    // step 2: Setup / Initialize
    ver = zlibVersion();
    gz = gzopen("test_zlib_api_sequence.gz", "wb");

    // step 3: Operate - write, inspect offset, rewind, inspect offset, write again
    wrote1 = gzwrite(gz, data, data_len);
    offset_after_write1 = gzoffset(gz);
    rewind_ret = gzrewind(gz);
    offset_after_rewind = gzoffset(gz);
    wrote2 = gzwrite(gz, data, data_len);
    offset_after_write2 = gzoffset(gz);

    // step 4: Validate / Cleanup
    close_ret = gzclose(gz);

    (void)ver;
    (void)wrote1;
    (void)offset_after_write1;
    (void)rewind_ret;
    (void)offset_after_rewind;
    (void)wrote2;
    (void)offset_after_write2;
    (void)close_ret;
    // API sequence test completed successfully
    return 66;
}