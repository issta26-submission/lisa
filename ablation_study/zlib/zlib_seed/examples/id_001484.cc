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
//<ID> 1484
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence: compress + gzwrite + crc32_combine + zlibVersion";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();

    // step 2: Setup (compute partial CRCs and prepare destination buffer)
    uLong half = (uLong)(srcLen / 2);
    uLong crc1 = crc32(0L, src, (uInt)half);
    uLong crc2 = crc32(0L, src + half, (uInt)(srcLen - half));
    off_t len2 = (off_t)(srcLen - half);
    uLong combined_crc = crc32_combine(crc1, crc2, len2);
    uLong bound = compressBound(srcLen);
    uLongf destLen = (uLongf)bound;
    Bytef *dest = new Bytef[(size_t)destLen];
    memset(dest, 0, (size_t)destLen);

    // step 3: Core operations (compress in-memory and write original data to a gz file)
    int rc_compress = compress(dest, &destLen, src, srcLen);
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_write = gzwrite(gzf, src, (unsigned int)srcLen);
    int rc_close = gzclose(gzf);

    // step 4: Cleanup and finalize
    delete [] src;
    delete [] dest;
    (void)version;
    (void)crc1;
    (void)crc2;
    (void)combined_crc;
    (void)bound;
    (void)destLen;
    (void)rc_compress;
    (void)rc_write;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}