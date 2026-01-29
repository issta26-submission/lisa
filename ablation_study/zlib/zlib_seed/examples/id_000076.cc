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
//<ID> 76
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers/streams
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, sizeof(z_stream));
    memset(&inf_strm, 0, sizeof(z_stream));
    const char src_c[] = "Payload for zlib compress/uncompress and gz offset test";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    uLongf decompLen = (uLongf)sourceLen;
    Bytef *decompBuf = (Bytef *)malloc((size_t)sourceLen);
    const char *version = zlibVersion();

    // step 2: Setup: initialize deflate & inflate and configure deflate parameters
    int rc_deflate_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int rc_deflate_params = deflateParams(&def_strm, 9, 0);
    int rc_inflate_init = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 3: Core operations: compress, uncompress, write compressed data to gz and get offset
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, (uLong)compLen);
    gzFile gf = gzopen64("test_zlib_api_seq.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)compLen);
    int rc_gzflush = gzflush(gf, 0);
    off64_t offset = gzoffset64(gf);

    // step 4: Validate & Cleanup
    int rc_gzclose = gzclose(gf);
    int rc_deflate_end = deflateEnd(&def_strm);
    int rc_inflate_end = inflateEnd(&inf_strm);
    free(compBuf);
    free(decompBuf);
    (void)rc_deflate_init; (void)rc_deflate_params; (void)rc_inflate_init;
    (void)rc_comp; (void)rc_uncomp; (void)rc_gzwrite; (void)rc_gzflush;
    (void)rc_gzclose; (void)rc_deflate_end; (void)rc_inflate_end; (void)offset;
    // API sequence test completed successfully
    return 66;
}