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
//<ID> 1487
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib sequence payload: version/compress/gzwrite/crc32_combine";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();

    // step 2: Setup (compute CRCs and allocate destination buffer)
    uLong crc_a = crc32(0L, src, (uInt)(srcLen / 2));
    uLong crc_b = crc32(0L, src + (srcLen / 2), (uInt)(srcLen - (srcLen / 2)));
    uLong crc_combined = crc32_combine(crc_a, crc_b, (off_t)(srcLen - (srcLen / 2)));
    uLongf destCap = compressBound(srcLen);
    Bytef *dest = new Bytef[(size_t)destCap];
    memset(dest, 0, (size_t)destCap);

    // step 3: Core operations (compress and write compressed data to a gz file)
    uLongf destLen = destCap;
    int rc_compress = compress(dest, &destLen, src, srcLen);
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_write = gzwrite(gzf, dest, (unsigned int)destLen);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup
    delete [] src;
    delete [] dest;
    (void)version;
    (void)crc_a;
    (void)crc_b;
    (void)crc_combined;
    (void)destCap;
    (void)rc_compress;
    (void)rc_write;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}