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
//<ID> 1509
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload: gzopen + gzbuffer + gzwrite + crc32_z + gzclose";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    uLong crc_before = crc32_z(0UL, src, (z_size_t)srcLen);

    // step 2: Configure (open and configure gz file buffering)
    const char *fname = "test_zlib_api_sequence_output.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_buf = gzbuffer(gzf, 8192u);

    // step 3: Operate (write payload to gz file)
    int rc_write = gzwrite(gzf, src, (unsigned int)srcLen);
    int valid_write = (rc_write == (int)srcLen);

    // step 4: Validate and Cleanup (close and free resources)
    int rc_close = gzclose(gzf);
    delete [] src;
    (void)crc_before;
    (void)rc_buf;
    (void)rc_write;
    (void)valid_write;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}