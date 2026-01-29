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
//<ID> 284
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver = zlibVersion();
    const char payload[] = "zlib API sequence payload: write version and payload, rewind, write again.";
    unsigned int payload_len = (unsigned int)(sizeof(payload) - 1);
    char tmpbuf[128];
    memset(tmpbuf, 0, sizeof(tmpbuf));
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int wrote_ver = 0;
    int wrote_payload1 = 0;
    int wrote_payload2 = 0;
    off_t offset_after_writes = 0;
    off_t offset_after_rewind = 0;
    int ret_rewind = 0;
    int ret_close = 0;

    // step 2: Setup and initial writes
    wrote_payload1 = gzwrite(gz, payload, payload_len);
    wrote_ver = gzwrite(gz, ver, (unsigned int)strlen(ver));
    offset_after_writes = gzoffset(gz);

    // step 3: Rewind and subsequent write
    ret_rewind = gzrewind(gz);
    offset_after_rewind = gzoffset(gz);
    wrote_payload2 = gzwrite(gz, payload, payload_len);

    // step 4: Cleanup
    ret_close = gzclose(gz);

    (void)tmpbuf;
    (void)wrote_ver;
    (void)wrote_payload1;
    (void)wrote_payload2;
    (void)offset_after_writes;
    (void)offset_after_rewind;
    (void)ret_rewind;
    (void)ret_close;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}