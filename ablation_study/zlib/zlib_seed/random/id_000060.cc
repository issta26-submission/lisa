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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers/streams
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char src_c[] = "Test payload for zlib to compress and then inflate with prime";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    Bytef *decompBuf = (Bytef *)malloc((size_t)(sourceLen + 16));
    const char *version = zlibVersion();

    // step 2: Setup (compress input and prepare inflate stream)
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;
    strm.next_out = decompBuf;
    strm.avail_out = (uInt)(sourceLen + 16);
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));

    // step 3: Operate (prime inflate and perform inflate)
    int rc_prime = inflatePrime(&strm, 3, 1);
    int rc_inf = inflate(&strm, 0);

    // step 4: Validate & Cleanup
    uLong decomp_written = strm.total_out;
    int rc_back_end = inflateBackEnd(&strm);
    int rc_inf_end = inflateEnd(&strm);
    free(compBuf);
    free(decompBuf);
    (void)rc_comp; (void)rc_inf_init; (void)rc_prime; (void)rc_inf; (void)decomp_written; (void)rc_back_end; (void)rc_inf_end;

    // API sequence test completed successfully
    return 66;
}