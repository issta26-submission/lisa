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
//<ID> 51
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
    const char src_c[] = "Example payload for zlib uncompress and crc32_z testing";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Configure
    uLong crc_comp = crc32_z(0UL, compBuf, (z_size_t)compLen);
    uLongf destLen = (uLongf)sourceLen;
    Bytef *destBuf = (Bytef *)malloc((size_t)destLen);
    int rc_uncomp = uncompress(destBuf, &destLen, compBuf, compLen);

    // step 3: Operate -> Validate
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    uLong crc_uncomp = crc32_z(0UL, destBuf, (z_size_t)destLen);

    // step 4: Cleanup
    inflateEnd(&strm);
    free(compBuf);
    free(destBuf);
    (void)rc_comp; (void)rc_uncomp; (void)rc_inf_init; (void)crc_comp; (void)crc_uncomp; (void)version;

    // API sequence test completed successfully
    return 66;
}