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
//<ID> 73
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize
    z_stream strm_inflate;
    z_stream strm_deflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    memset(&strm_deflate, 0, sizeof(z_stream));
    const char src_c[] = "Example payload for zlib compress/inflate and parameter tuning";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    Bytef *decompBuf = (Bytef *)malloc((size_t)sourceLen);
    uLongf decompLen = (uLongf)sourceLen;
    const char *version = zlibVersion();

    // step 2: Setup (initialize inflate & deflate streams, configure deflate)
    int rc_inflate_init = inflateInit_(&strm_inflate, version, (int)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&strm_deflate, 6, version, (int)sizeof(z_stream));
    int rc_deflate_params = deflateParams(&strm_deflate, 5, 0);

    // step 3: Operate (compress source, then inflate the compressed data)
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    strm_inflate.next_in = compBuf;
    strm_inflate.avail_in = (uInt)compLen;
    strm_inflate.next_out = decompBuf;
    strm_inflate.avail_out = (uInt)decompLen;
    int rc_inflate = inflate(&strm_inflate, 0);

    // step 4: Validate & Cleanup
    gzFile gz = (gzFile)0;
    off64_t offset = gzoffset64(gz);
    int rc_inflate_end = inflateEnd(&strm_inflate);
    int rc_deflate_end = deflateEnd(&strm_deflate);
    free(compBuf);
    free(decompBuf);
    (void)rc_inflate_init; (void)rc_deflate_init; (void)rc_deflate_params;
    (void)rc_comp; (void)rc_inflate; (void)offset; (void)rc_inflate_end; (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}