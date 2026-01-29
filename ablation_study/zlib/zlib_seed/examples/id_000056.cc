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
//<ID> 56
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "Test payload for zlib APIs demonstrating compress/uncompress and CRC";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));

    // step 2: Operate (uncompress the previously compressed data)
    uLongf decompLen = (uLongf)sourceLen;
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompLen);
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, compLen);

    // step 3: Validate (compute CRC of the decompressed payload)
    uLong crc = crc32_z(0UL, decompBuf, (z_size_t)decompLen);
    (void)crc; (void)rc_comp; (void)rc_inf_init; (void)rc_uncomp;

    // step 4: Cleanup
    inflateEnd(&strm);
    free(compBuf);
    free(decompBuf);

    // API sequence test completed successfully
    return 66;
}