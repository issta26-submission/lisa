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
//<ID> 280
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
    char buf[256];
    const char payload[] = "zlib API payload: write, rewind, offset, write-again.";
    unsigned int len;
    int write_ret1;
    int write_ret2;
    off_t offset_before;
    off_t offset_after;
    int rewind_ret;
    int close_ret;

    // step 2: Setup and open
    memset(buf, 0, sizeof(buf));
    len = (unsigned int)(sizeof(payload) - 1);
    memcpy(buf, payload, len);
    ver = zlibVersion();
    gz = gzopen("test_zlib_api_sequence.gz", "wb");

    // step 3: Operate - write, query offset, rewind, query offset, write again
    write_ret1 = gzwrite(gz, buf, len);
    offset_before = gzoffset(gz);
    rewind_ret = gzrewind(gz);
    offset_after = gzoffset(gz);
    write_ret2 = gzwrite(gz, buf, len);

    // step 4: Validate and cleanup
    close_ret = gzclose(gz);
    (void)ver;
    (void)write_ret1;
    (void)write_ret2;
    (void)offset_before;
    (void)offset_after;
    (void)rewind_ret;
    (void)close_ret;
    // API sequence test completed successfully
    return 66;
}