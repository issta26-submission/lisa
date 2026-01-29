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
//<ID> 59
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
    const char src_c[] = "Sample payload for zlib compress/uncompress and crc check";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    uLongf compBufLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    uLongf compLen = compBufLen;
    Bytef *decompBuf = (Bytef *)malloc((size_t)sourceLen);
    uLongf decompLen = (uLongf)sourceLen;
    const char *version = zlibVersion();

    // step 2: Setup (initialize inflate state)
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));

    // step 3: Operate (compress -> uncompress -> compute crc)
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, (uLong)compLen);
    uLong crc = crc32_z(0UL, decompBuf, (z_size_t)decompLen);

    // step 4: Validate & Cleanup
    (void)rc_comp; (void)rc_uncomp; (void)rc_inflate_init; (void)crc;
    inflateEnd(&strm);
    free(compBuf);
    free(decompBuf);

    // API sequence test completed successfully
    return 66;
}