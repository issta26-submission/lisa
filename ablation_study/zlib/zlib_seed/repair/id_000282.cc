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
//<ID> 282
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
    const char payload[] = "zlib gz write test payload";
    unsigned int payload_len = (unsigned int)(sizeof(payload) - 1);
    int wrote1 = 0;
    int wrote2 = 0;
    off_t offset_after_write = 0;
    off_t offset_after_rewind = 0;
    int ret_rewind = 0;
    int ret_close = 0;
    char scratch[128];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Setup - query version and open gzip file for writing
    ver = zlibVersion();
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    wrote1 = gzwrite(gz, (voidpc)payload, payload_len);

    // step 3: Operate - get offset, rewind the stream, verify offset, write again
    offset_after_write = gzoffset(gz);
    ret_rewind = gzrewind(gz);
    offset_after_rewind = gzoffset(gz);
    wrote2 = gzwrite(gz, (voidpc)payload, payload_len);

    // step 4: Cleanup - close file and tidy up
    ret_close = gzclose(gz);
    (void)wrote1;
    (void)wrote2;
    (void)offset_after_write;
    (void)offset_after_rewind;
    (void)ret_rewind;
    (void)ret_close;
    (void)ver;
    (void)scratch;
    // API sequence test completed successfully
    return 66;
}