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
//<ID> 1488
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
    uLong crc_first = crc32(0L, src, (uInt)(srcLen / 2));
    uLong crc_second = crc32(0L, src + (srcLen / 2), (uInt)(srcLen - (srcLen / 2)));
    uLong crc_combined = crc32_combine(crc_first, crc_second, (off_t)(srcLen - (srcLen / 2)));

    // step 2: Setup
    uLongf destLen = (uLongf)compressBound(srcLen);
    Bytef *dest = new Bytef[(size_t)destLen];
    memset(dest, 0, (size_t)destLen);
    int rc_compress = compress(dest, &destLen, src, srcLen);

    // step 3: Core operations
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_write = gzwrite(gzf, dest, (unsigned int)destLen);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup
    delete [] src;
    delete [] dest;
    (void)version;
    (void)crc_first;
    (void)crc_second;
    (void)crc_combined;
    (void)rc_compress;
    (void)rc_write;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}