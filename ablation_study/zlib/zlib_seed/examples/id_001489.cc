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
//<ID> 1489
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload: compress + gzwrite + crc32_combine";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();

    // step 2: Setup (prepare checksums and compression buffers)
    uLong half = (srcLen > 1u ? (srcLen / 2u) : 1u);
    uLong crc1 = crc32(0L, src, (uInt)half);
    uLong crc2 = crc32(0L, src + half, (uInt)(srcLen - half));
    uLong crc_combined = crc32_combine(crc1, crc2, (off_t)(srcLen - half));
    uLong bound = compressBound(srcLen);
    Bytef *dest = new Bytef[(size_t)bound];
    memset(dest, 0, (size_t)bound);
    uLongf destLen = (uLongf)bound;

    // step 3: Core operations (compress and write to a gz file)
    int rc_compress = compress(dest, &destLen, src, srcLen);
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_write = gzwrite(gzf, dest, (unsigned int)destLen);
    int rc_close = gzclose(gzf);

    // step 4: Cleanup
    int rc_gzclose_unused = rc_close;
    delete [] src;
    delete [] dest;
    (void)version;
    (void)crc1;
    (void)crc2;
    (void)crc_combined;
    (void)bound;
    (void)rc_compress;
    (void)rc_write;
    (void)rc_gzclose_unused;
    // API sequence test completed successfully
    return 66;
}