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
//<ID> 281
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    gzFile gz;
    const char payload[] = "zlib API sequence payload: gzopen -> gzwrite -> gzrewind -> gzoffset -> gzwrite -> gzclose.";
    unsigned int payload_len = (unsigned int)(sizeof(payload) - 1);
    char scratch[256];
    const char *ver;
    int ret_write1;
    int ret_rewind;
    int ret_write2;
    int ret_close;
    int ret_open;
    off_t off_before_rewind;
    off_t off_after_rewind;
    off_t off_after_second_write;

    // step 2: Setup and initialization
    memset(scratch, 0, sizeof(scratch));
    ver = zlibVersion();

    // step 3: Core operations - open gzip file, write, rewind, inspect offsets, write again
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_open = (gz != 0);
    ret_write1 = gzwrite(gz, payload, payload_len);
    off_before_rewind = gzoffset(gz);
    ret_rewind = gzrewind(gz);
    off_after_rewind = gzoffset(gz);
    ret_write2 = gzwrite(gz, payload, payload_len);
    off_after_second_write = gzoffset(gz);

    // step 4: Validate and cleanup
    ret_close = gzclose(gz);

    (void)ver;
    (void)scratch;
    (void)ret_open;
    (void)ret_write1;
    (void)off_before_rewind;
    (void)ret_rewind;
    (void)off_after_rewind;
    (void)ret_write2;
    (void)off_after_second_write;
    (void)ret_close;
    // API sequence test completed successfully
    return 66;
}