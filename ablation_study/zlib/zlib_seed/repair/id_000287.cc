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
//<ID> 287
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver = 0;
    gzFile gz = 0;
    char write_buf[] = "zlib API sequence test payload: write -> rewind -> write -> offset -> close.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    int ret_write1 = 0;
    int ret_rewind = 0;
    int ret_write2 = 0;
    int ret_close = 0;
    off_t offset_after_first_write = 0;
    off_t offset_after_second_write = 0;

    // step 2: Setup / Initialize
    ver = zlibVersion();
    gz = gzopen("test_zlib_api_sequence.gz", "wb");

    // step 3: Operate - write, query offset, rewind, write again, query offset
    ret_write1 = gzwrite(gz, (voidpc)write_buf, write_len);
    offset_after_first_write = gzoffset(gz);
    ret_rewind = gzrewind(gz);
    ret_write2 = gzwrite(gz, (voidpc)write_buf, write_len);
    offset_after_second_write = gzoffset(gz);

    // step 4: Validate / Cleanup
    ret_close = gzclose(gz);
    (void)ver;
    (void)ret_write1;
    (void)offset_after_first_write;
    (void)ret_rewind;
    (void)ret_write2;
    (void)offset_after_second_write;
    (void)ret_close;
    // API sequence test completed successfully
    return 66;
}