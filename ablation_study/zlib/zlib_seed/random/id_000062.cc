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
//<ID> 62
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char src_c[] = "Example payload for zlib compress/inflate API sequence";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *decompBuf = (Bytef *)malloc((size_t)(sourceLen + 16));
    z_stream inf_strm;
    z_stream back_strm;
    memset(&inf_strm, 0, sizeof(z_stream));
    memset(&back_strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();

    // step 2: Setup
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compLen;
    inf_strm.next_out = decompBuf;
    inf_strm.avail_out = (uInt)sourceLen;
    int rc_inf_init = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    int rc_prime = inflatePrime(&inf_strm, 0, 0);
    unsigned char *window = (unsigned char *)malloc(32768);
    int rc_back_init = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Core operations
    int rc_inf = inflate(&inf_strm, 0);
    uLong decomp_written = inf_strm.total_out;
    int rc_back_end = inflateBackEnd(&back_strm);

    // step 4: Cleanup
    (void)rc_comp; (void)rc_inf_init; (void)rc_prime; (void)rc_back_init; (void)rc_inf; (void)decomp_written; (void)rc_back_end;
    inflateEnd(&inf_strm);
    free(compBuf);
    free(decompBuf);
    free(window);

    // API sequence test completed successfully
    return 66;
}