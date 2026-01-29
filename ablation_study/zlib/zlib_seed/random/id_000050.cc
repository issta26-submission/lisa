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
//<ID> 50
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "Example payload for zlib compression and verification";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong crc_before = crc32_z(0, source, (z_size_t)sourceLen);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *decompBuf = (Bytef *)malloc((size_t)bound);
    uLongf decompLen = (uLongf)bound;
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (compress source into compBuf)
    int rc_compress = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Operate & Validate (uncompress into decompBuf, initialize inflate for state, compute CRCs)
    int rc_uncompress = uncompress(decompBuf, &decompLen, compBuf, (uLong)compLen);
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    uLong crc_after = crc32_z(0, decompBuf, (z_size_t)decompLen);
    (void)rc_compress; (void)rc_uncompress; (void)rc_inflate_init; (void)crc_before; (void)crc_after;

    // step 4: Cleanup
    inflateEnd(&strm);
    free(compBuf);
    free(decompBuf);

    // API sequence test completed successfully
    return 66;
}