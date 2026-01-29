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
//<ID> 1500
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib sequence payload: gzopen + gzbuffer + crc32_z + gzwrite";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);

    // step 2: Setup / Initialize checksums
    uLong half = (srcLen > 1u ? (srcLen / 2u) : 1u);
    uLong crc_partial = crc32_z(0L, src, (z_size_t)half);
    uLong crc_final = crc32_z(crc_partial, src + half, (z_size_t)(srcLen - half));

    // step 3: Operate (open gz, configure buffer, write data)
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_buf = gzbuffer(gzf, 32768);
    int rc_write = gzwrite(gzf, src, (unsigned int)srcLen);

    // step 4: Cleanup and finalize
    int rc_close = gzclose(gzf);
    delete [] src;
    (void)crc_partial;
    (void)crc_final;
    (void)rc_buf;
    (void)rc_write;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}