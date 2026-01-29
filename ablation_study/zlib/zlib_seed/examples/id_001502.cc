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
//<ID> 1502
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

    // step 2: Setup
    uLong crc_before = crc32_z(0L, src, (z_size_t)srcLen);
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_buf = gzbuffer(gzf, 4096u);

    // step 3: Core operations
    int rc_write = gzwrite(gzf, src, (unsigned int)srcLen);
    uLong crc_after = crc32_z(crc_before, src, (z_size_t)srcLen);
    int rc_close = gzclose(gzf);

    // step 4: Cleanup
    delete [] src;
    (void)crc_before;
    (void)crc_after;
    (void)rc_buf;
    (void)rc_write;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}