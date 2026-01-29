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
//<ID> 283
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
    const char filename[] = "test_zlib_api_sequence.gz";
    const char write_buf[] = "zlib test payload: gzwrite -> gzoffset -> gzrewind -> gzwrite";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    int ret_write1;
    int ret_write2;
    int ret_rewind;
    int ret_close;
    off_t offset_after_first;
    off_t offset_after_second;

    // step 2: Setup / Initialize
    ver = zlibVersion();
    gz = gzopen(filename, "wb");

    // step 3: Operate - write data, inspect offset, rewind, write again, inspect offset
    ret_write1 = gzwrite(gz, (voidpc)write_buf, write_len);
    offset_after_first = gzoffset(gz);
    ret_rewind = gzrewind(gz);
    ret_write2 = gzwrite(gz, (voidpc)write_buf, write_len);
    offset_after_second = gzoffset(gz);
    ret_close = gzclose(gz);

    // step 4: Validate & Cleanup
    (void)ver;
    (void)ret_write1;
    (void)ret_write2;
    (void)ret_rewind;
    (void)ret_close;
    (void)offset_after_first;
    (void)offset_after_second;
    // API sequence test completed successfully
    return 66;
}