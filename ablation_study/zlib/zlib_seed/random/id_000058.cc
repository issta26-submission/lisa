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
//<ID> 58
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
    const char src_c[] = "Example payload for zlib API sequence";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    uLongf decompLen = (uLongf)sourceLen;
    Bytef *decompBuf = (Bytef *)malloc((size_t)sourceLen);
    uLong crc_before = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Setup (compress source into compBuf)
    const char *version = zlibVersion();
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 3: Operate (uncompress back into decompBuf and initialize an inflate stream)
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, (uLong)compLen);
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));

    // step 4: Validate & Cleanup
    uLong crc_after = crc32_z(0UL, decompBuf, (z_size_t)decompLen);
    (void)crc_before; (void)crc_after; (void)rc_comp; (void)rc_uncomp; (void)rc_inflate_init;
    inflateEnd(&strm);
    free(compBuf);
    free(decompBuf);

    // API sequence test completed successfully
    return 66;
}