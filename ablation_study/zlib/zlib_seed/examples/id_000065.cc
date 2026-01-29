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
//<ID> 65
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "Example payload data to compress and then inflate using zlib APIs";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    Bytef *decompBuf = (Bytef *)malloc((size_t)sourceLen);

    // step 2: Setup (compress source into buffer)
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 3: Operate (initialize inflate, prime, and inflate the compressed data)
    int rc_inf_init = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compLen;
    inf_strm.next_out = decompBuf;
    inf_strm.avail_out = (uInt)sourceLen;
    int rc_prime = inflatePrime(&inf_strm, 1, 1);
    int rc_inflate = inflate(&inf_strm, 0);

    // step 4: Validate & Cleanup
    int rc_back_end = inflateBackEnd(&inf_strm);
    free(compBuf);
    free(decompBuf);
    (void)rc_comp; (void)rc_inf_init; (void)rc_prime; (void)rc_inflate; (void)rc_back_end;
    // API sequence test completed successfully
    return 66;
}