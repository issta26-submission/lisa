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
//<ID> 1485
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib sequence payload: compress/crc32_combine/gzwrite/gzclose";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    (void)version;

    // step 2: Setup (allocate destination buffer and compute checksums)
    uLongf destLen = (uLongf)compressBound(srcLen);
    Bytef *dest = new Bytef[(size_t)destLen];
    memset(dest, 0, (size_t)destLen);
    uLong crc1 = crc32(0L, src, (uInt)srcLen);
    uLong crc2 = crc32(0L, src, (uInt)(srcLen / 2));
    uLong crc_combined = crc32_combine(crc1, crc2, (off_t)(srcLen / 2));

    // step 3: Core operations (compress and write compressed data to a .gz file)
    int rc_compress = compress(dest, &destLen, src, srcLen);
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_write = gzwrite(gzf, dest, (unsigned int)destLen);
    int rc_close = gzclose(gzf);

    // step 4: Cleanup
    delete [] src;
    delete [] dest;
    (void)crc1;
    (void)crc2;
    (void)crc_combined;
    (void)rc_compress;
    (void)rc_write;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}