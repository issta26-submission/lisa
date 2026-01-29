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
//<ID> 55
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "Example payload for zlib uncompress test.";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));

    // step 2: Configure (compress source, get version, init inflate)
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));

    // step 3: Operate (uncompress into dest and compute CRC)
    uLongf destLen = (uLongf)bound;
    Bytef *dest = (Bytef *)malloc((size_t)bound);
    int rc_uncomp = uncompress(dest, &destLen, comp, (uLong)compLen);
    uLong crc = crc32_z(0UL, dest, (z_size_t)destLen);

    // step 4: Validate & Cleanup
    inflateEnd(&strm);
    free(comp);
    free(dest);

    // API sequence test completed successfully
    (void)rc_comp; (void)rc_inf_init; (void)rc_uncomp; (void)crc;
    return 66;
}