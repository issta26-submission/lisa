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
//<ID> 64
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize
    const char src_c[] = "Sample payload for zlib API sequence with inflatePrime";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *decompBuf = (Bytef *)malloc((size_t)sourceLen);
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();

    // step 2: Setup (compress source and initialize inflate)
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;
    strm.next_out = decompBuf;
    strm.avail_out = (uInt)sourceLen;
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));

    // step 3: Operate (prime inflate bit buffer and perform inflate)
    int rc_prime = inflatePrime(&strm, 0, 0);
    int rc_inflate = inflate(&strm, 0);
    uLong decompressed = strm.total_out;

    // step 4: Validate & Cleanup
    inflateEnd(&strm);
    int rc_back_end = inflateBackEnd(&strm);
    free(compBuf);
    free(decompBuf);
    (void)rc_comp; (void)rc_inf_init; (void)rc_prime; (void)rc_inflate; (void)decompressed; (void)rc_back_end; (void)version; (void)bound; (void)compLen;

    // API sequence test completed successfully
    return 66;
}