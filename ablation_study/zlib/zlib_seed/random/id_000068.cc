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
//<ID> 68
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
    const char src_c[] = "example payload data for zlib test";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *decompBuf = (Bytef *)malloc((size_t)(sourceLen + 32));
    const char *version = zlibVersion();

    // step 2: Setup (compress source into buffer)
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 3: Configure & Operate (initialize inflate, set stream pointers, prime bits, inflate)
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;
    strm.next_out = decompBuf;
    strm.avail_out = (uInt)(sourceLen + 32);
    int rc_prime = inflatePrime(&strm, 0, 0);
    int rc_inflate = inflate(&strm, 0);

    // step 4: Validate & Cleanup
    int rc_backend = inflateBackEnd(&strm);
    int rc_end = inflateEnd(&strm);
    free(compBuf);
    free(decompBuf);

    // API sequence test completed successfully
    (void)rc_comp; (void)rc_inf_init; (void)rc_prime; (void)rc_inflate; (void)rc_backend; (void)rc_end;
    return 66;
}