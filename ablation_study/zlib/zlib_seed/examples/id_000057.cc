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
//<ID> 57
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "The quick brown fox jumps over the lazy dog";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    uLongf decompLen = (uLongf)sourceLen;
    Bytef *decompBuf = (Bytef *)malloc((size_t)sourceLen);
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    uLong crc_orig = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Configure (compress data)
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Operate -> Initialize inflate and uncompress
    int rc_inf_init = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, (uLong)compLen);
    uLong crc_decomp = crc32_z(0UL, decompBuf, (z_size_t)decompLen);

    // step 4: Cleanup
    inflateEnd(&inf_strm);
    free(compBuf);
    free(decompBuf);

    // API sequence test completed successfully
    (void)rc_comp; (void)rc_inf_init; (void)rc_uncomp; (void)crc_orig; (void)crc_decomp;
    return 66;
}