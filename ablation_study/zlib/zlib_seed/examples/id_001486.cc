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
//<ID> 1486
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib sequence payload: compress -> write gz -> crc32_combine";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();

    // step 2: Setup (prepare destination buffer and compute initial CRCs)
    uLong bound = compressBound(srcLen);
    uLongf destLen = (uLongf)bound;
    Bytef *dest = new Bytef[(size_t)bound];
    memset(dest, 0, (size_t)bound);
    uLong crc_first = crc32(0UL, src, (unsigned int)(srcLen / 2));
    uLong crc_second = crc32(0UL, src + (srcLen / 2), (unsigned int)(srcLen - (srcLen / 2)));

    // step 3: Core operations (compress source, combine CRCs, write compressed data to gz file)
    int rc_compress = compress(dest, &destLen, src, srcLen);
    uLong crc_combined = crc32_combine(crc_first, crc_second, (off_t)(srcLen - (srcLen / 2)));
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int bytes_written = gzwrite(gzf, dest, (unsigned int)destLen);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup
    delete [] src;
    delete [] dest;
    (void)version;
    (void)bound;
    (void)rc_compress;
    (void)crc_first;
    (void)crc_second;
    (void)crc_combined;
    (void)bytes_written;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}