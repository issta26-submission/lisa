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
//<ID> 52
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "Sample payload data for zlib API sequence test";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLongf compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLongf compLen = compBound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    uLongf decompLen = compBound;
    Bytef *decompBuf = (Bytef *)malloc((size_t)compBound);

    // step 2: Setup (version, crc, inflate init)
    const char *version = zlibVersion();
    uLong crc_before = crc32_z(0L, source, (z_size_t)sourceLen);
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));

    // step 3: Operate -> uncompress and compute crc of result
    uLong compWritten = (uLong)compLen;
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, compWritten);
    uLong crc_after = crc32_z(0L, decompBuf, (z_size_t)decompLen);

    // step 4: Cleanup
    int rc_inflate_end = inflateEnd(&strm);
    free(compBuf);
    free(decompBuf);

    // API sequence test completed successfully
    (void)rc_comp; (void)rc_inflate_init; (void)rc_uncomp; (void)crc_before; (void)crc_after; (void)rc_inflate_end;
    return 66;
}