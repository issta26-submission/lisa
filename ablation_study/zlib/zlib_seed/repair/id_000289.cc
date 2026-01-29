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
//<ID> 289
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver;
    gzFile gz;
    unsigned char write_buf[] = "zlib gzwrite test payload";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    unsigned char scratch[64];
    int bytes_written1;
    int bytes_written2;
    off_t offset_after_first_write;
    off_t offset_after_second_write;
    int ret_rewind;
    int ret_close;

    // step 2: Setup / initialize
    ver = zlibVersion();
    memset(scratch, 0, sizeof(scratch));
    gz = gzopen("test_zlib_api_sequence.gz", "wb");

    // step 3: Core operations - write, inspect offset, rewind, write again, inspect offset
    bytes_written1 = gzwrite(gz, (voidpc)write_buf, write_len);
    offset_after_first_write = gzoffset(gz);
    ret_rewind = gzrewind(gz);
    bytes_written2 = gzwrite(gz, (voidpc)write_buf, write_len);
    offset_after_second_write = gzoffset(gz);

    // step 4: Validate / cleanup
    ret_close = gzclose(gz);
    (void)ver;
    (void)scratch;
    (void)bytes_written1;
    (void)bytes_written2;
    (void)offset_after_first_write;
    (void)offset_after_second_write;
    (void)ret_rewind;
    (void)ret_close;
    // API sequence test completed successfully
    return 66;
}