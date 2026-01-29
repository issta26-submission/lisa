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
//<ID> 1481
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

    // step 2: Setup (compute CRCs for two halves and combine)
    uLong half_len = (uLong)(srcLen / 2);
    uLong crc1 = crc32(0L, src, (uInt)half_len);
    uLong crc2 = crc32(0L, src + half_len, (uInt)(srcLen - half_len));
    uLong crc_combined = crc32_combine(crc1, crc2, (off_t)(srcLen - half_len));

    // step 3: Core operations (compress and write to gz file)
    uLong destBound = compressBound(srcLen);
    Bytef *dest = new Bytef[(size_t)destBound];
    memset(dest, 0, (size_t)destBound);
    uLongf destLen = (uLongf)destBound;
    int rc_compress = compress(dest, &destLen, src, srcLen);
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int bytes_written = gzwrite(gzf, dest, (unsigned int)destLen);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup
    delete [] src;
    delete [] dest;
    (void)version;
    (void)crc1;
    (void)crc2;
    (void)crc_combined;
    (void)rc_compress;
    (void)bytes_written;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}