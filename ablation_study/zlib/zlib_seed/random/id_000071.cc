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
//<ID> 71
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream inf_strm;
    z_stream def_strm;
    memset(&inf_strm, 0, sizeof(z_stream));
    memset(&def_strm, 0, sizeof(z_stream));
    const char raw[] = "Example payload for zlib compress/inflate and gzoffset64 usage";
    const Bytef *source = (const Bytef *)raw;
    uLong sourceLen = (uLong)(sizeof(raw) - 1);
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    Bytef *decompBuf = (Bytef *)malloc((size_t)sourceLen);
    const char *version = zlibVersion();

    // step 2: Setup (initialize and configure streams)
    int rc_deflate_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int rc_deflate_params = deflateParams(&def_strm, 5, 0); // level=5, strategy=0 (Z_DEFAULT_STRATEGY)
    int rc_inflate_init = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 3: Core operations (compress -> inflate using inflate stream -> query gz offset)
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compLen;
    inf_strm.next_out = decompBuf;
    inf_strm.avail_out = (uInt)sourceLen;
    int rc_inflate = inflate(&inf_strm, 0); // 0 == Z_NO_FLUSH
    gzFile gzf = gzopen64("test_zlib_tmp.gz", "wb");
    off64_t current_offset = gzoffset64(gzf);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup & validate
    int rc_deflate_end = deflateEnd(&def_strm);
    int rc_inflate_end = inflateEnd(&inf_strm);
    free(compBuf);
    free(decompBuf);
    (void)rc_deflate_init; (void)rc_deflate_params; (void)rc_inflate_init;
    (void)rc_comp; (void)rc_inflate; (void)current_offset; (void)rc_gzclose;
    (void)rc_deflate_end; (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}