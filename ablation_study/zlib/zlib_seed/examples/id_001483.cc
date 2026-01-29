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
//<ID> 1483
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for compress/gzwrite/crc32_combine/zlibVersion";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong crc1 = crc32(0L, src, (uInt)(srcLen / 2));
    uLong crc2 = crc32(0L, src + (srcLen / 2), (uInt)(srcLen - (srcLen / 2)));
    uLong crc_combined = crc32_combine(crc1, crc2, (off_t)(srcLen - (srcLen / 2)));

    // step 2: Setup (compress source into destination and write to gz file)
    uLongf destLen = compressBound(srcLen);
    Bytef *dest = new Bytef[(size_t)destLen];
    memset(dest, 0, (size_t)destLen);
    int rc_compress = compress(dest, &destLen, src, srcLen);
    gzFile gzf_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_write = gzwrite(gzf_w, dest, (unsigned int)destLen);
    int rc_close_w = gzclose(gzf_w);

    // step 3: Operate and Validate (read back the written gz file)
    gzFile gzf_r = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *readBuf = new Bytef[(size_t)srcLen];
    memset(readBuf, 0, (size_t)srcLen);
    int bytes_read = gzread(gzf_r, readBuf, (unsigned int)srcLen);
    int rc_close_r = gzclose(gzf_r);

    // step 4: Cleanup
    delete [] src;
    delete [] dest;
    delete [] readBuf;
    (void)version;
    (void)rc_compress;
    (void)rc_write;
    (void)rc_close_w;
    (void)bytes_read;
    (void)rc_close_r;
    (void)crc_combined;
    // API sequence test completed successfully
    return 66;
}