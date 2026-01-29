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
//<ID> 1505
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload: gzopen + gzbuffer + gzwrite + crc32_z + gzclose";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const z_size_t half = (srcLen > 1u ? (z_size_t)(srcLen / 2u) : (z_size_t)1);

    // step 2: Setup (open gz file and configure buffering)
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_buf = gzbuffer(gzf, 16384);

    // step 3: Core operations (compute incremental CRC and write compressed data)
    uLong crc_first_half = crc32_z(0L, src, half);
    uLong crc_full_via_continue = crc32_z(crc_first_half, src + half, (z_size_t)(srcLen - half));
    int rc_write = gzwrite(gzf, src, (unsigned int)srcLen);

    // step 4: Cleanup and validation artifacts
    int rc_close = gzclose(gzf);
    delete [] src;
    (void)rc_buf;
    (void)rc_write;
    (void)rc_close;
    (void)crc_first_half;
    (void)crc_full_via_continue;
    // API sequence test completed successfully
    return 66;
}