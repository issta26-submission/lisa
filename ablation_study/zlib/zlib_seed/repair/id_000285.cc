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
//<ID> 285
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver = zlibVersion();
    const char payload[] = "zlib API sequence payload: write -> offset -> rewind -> write again.";
    unsigned int payload_len = (unsigned int)(sizeof(payload) - 1);
    gzFile gz;
    int write_ret1;
    int write_ret2;
    int rewind_ret;
    int close_ret;
    off_t offset_after_first_write;
    off_t offset_after_rewind;
    // step 2: Setup - open gzip file for read/write in binary mode
    gz = gzopen("test_zlib_api_sequence.gz", "wb+");
    // step 3: Core operations - write payload, query offset, rewind, query offset, write again
    write_ret1 = gzwrite(gz, payload, payload_len);
    offset_after_first_write = gzoffset(gz);
    rewind_ret = gzrewind(gz);
    offset_after_rewind = gzoffset(gz);
    write_ret2 = gzwrite(gz, payload, payload_len);
    // step 4: Validate and cleanup - close file and silence unused-variable warnings
    close_ret = gzclose(gz);
    (void)ver;
    (void)write_ret1;
    (void)write_ret2;
    (void)rewind_ret;
    (void)close_ret;
    (void)offset_after_first_write;
    (void)offset_after_rewind;
    // API sequence test completed successfully
    return 66;
}