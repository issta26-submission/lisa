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
//<ID> 1527
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload: gzopen + gzwrite + crc32_z + zlibVersion";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();

    // step 2: Setup (open gz file)
    const char *fname = "test_zlib_api_sequence_output.gz";
    gzFile gzf = gzopen(fname, "wb");

    // step 3: Operate (compute CRC, write payload and version)
    uLong crc_before = crc32_z(0UL, src, (z_size_t)srcLen);
    int written1 = gzwrite(gzf, src, (unsigned int)srcLen);
    unsigned int verLen = (unsigned int)strlen(version);
    int written2 = gzwrite(gzf, version, verLen);
    uLong crc_after = crc32_z(crc_before, (const Bytef *)version, (z_size_t)verLen);

    // step 4: Validate and Cleanup
    int rc_close = gzclose(gzf);
    delete [] src;
    (void)written1;
    (void)written2;
    (void)crc_before;
    (void)crc_after;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}