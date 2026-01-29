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
//<ID> 77
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, sizeof(z_stream));
    memset(&inf_strm, 0, sizeof(z_stream));
    const char src_c[] = "Example payload for zlib APIs including compress and gzoffset64";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    const char *version = zlibVersion();
    gzFile gzf = gzopen("test_zlib_api.gz", "wb");

    // step 2: Setup (initialize deflate and inflate streams)
    int rc_deflate_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    int rc_deflate_params = deflateParams(&def_strm, 9, 1);

    // step 3: Operate (compress source, write compressed bytes to gz file, query offset)
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    int rc_gzwrite = gzwrite(gzf, compBuf, (unsigned int)compLen);
    int rc_gzflush = gzflush(gzf, 0);
    off64_t offset = gzoffset64(gzf);

    // step 4: Validate & Cleanup
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    gzclose(gzf);
    free(compBuf);
    (void)rc_deflate_init; (void)rc_inflate_init; (void)rc_deflate_params;
    (void)rc_comp; (void)rc_gzwrite; (void)rc_gzflush; (void)offset;
    // API sequence test completed successfully
    return 66;
}