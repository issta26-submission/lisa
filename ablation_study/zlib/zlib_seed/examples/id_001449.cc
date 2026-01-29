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
//<ID> 1449
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for crc/compress/gzputc/combine";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();

    // step 2: Setup
    uLongf compDestLen = 0;
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    int rc_compress = compress(compBuf, &compDestLen, src, srcLen);

    // step 3: Core operations
    uLong crc_src = crc32_z(0UL, src, (z_size_t)srcLen);
    uLong crc_comp = crc32_z(0UL, compBuf, (z_size_t)compDestLen);
    uLong crc_combined = crc32_combine(crc_src, crc_comp, (off_t)compDestLen);
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_put0 = gzputc(gzf, (int)src[0]);
    int rc_put1 = gzputc(gzf, (int)src[1]);
    int rc_put2 = gzputc(gzf, (int)src[2]);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup
    delete [] src;
    delete [] compBuf;
    (void)version;
    (void)rc_compress;
    (void)crc_src;
    (void)crc_comp;
    (void)crc_combined;
    (void)rc_put0;
    (void)rc_put1;
    (void)rc_put2;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}