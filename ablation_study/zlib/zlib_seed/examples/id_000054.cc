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
//<ID> 54
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char src_c[] = "Sample payload for zlib API sequence";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Setup (initialize inflate)
    const char *version = zlibVersion();
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));

    // step 3: Operate -> Uncompress and compute CRC
    uLongf destBufLen = (uLongf)sourceLen;
    Bytef *destBuf = (Bytef *)malloc((size_t)destBufLen);
    int rc_uncomp = uncompress(destBuf, &destBufLen, compBuf, (uLong)compLen);
    uLong crc = crc32_z(0UL, destBuf, (z_size_t)destBufLen);

    // step 4: Validate & Cleanup
    inflateEnd(&strm);
    free(compBuf);
    free(destBuf);
    (void)rc_comp; (void)rc_inflate_init; (void)rc_uncomp; (void)crc; (void)version;

    // API sequence test completed successfully
    return 66;
}