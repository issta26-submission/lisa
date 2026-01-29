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
//<ID> 1508
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char payload[] = "zlib api sequence payload: open gz, set buffer, write, compute crc32_z";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    uLong crc_before = crc32_z(0UL, src, (z_size_t)srcLen);

    // step 2: Configure (open gz file and set buffer)
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_buf = gzbuffer(gzf, 8192);

    // step 3: Operate / Validate (write data and recompute checksum)
    int rc_write = gzwrite(gzf, src, (unsigned int)srcLen);
    uLong crc_after = crc32_z(0UL, src, (z_size_t)srcLen);

    // step 4: Cleanup
    int rc_close = gzclose(gzf);
    delete [] src;
    (void)crc_before;
    (void)crc_after;
    (void)rc_buf;
    (void)rc_write;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}