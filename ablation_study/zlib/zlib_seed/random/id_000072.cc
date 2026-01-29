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
//<ID> 72
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Step 1: Declarations & Initialize
    z_stream deflateStrm;
    z_stream inflateStrm;
    memset(&deflateStrm, 0, (size_t)sizeof(z_stream));
    memset(&inflateStrm, 0, (size_t)sizeof(z_stream));
    const char src_c[] = "payload for zlib compress, write to gz, and inflate test";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    uLongf decompLen = (uLongf)sourceLen;
    Bytef *decompBuf = (Bytef *)malloc((size_t)sourceLen);
    const char *version = zlibVersion();

    // Step 2: Setup (initialize inflate and deflate streams, configure deflate)
    int rc_inflate_init = inflateInit_(&inflateStrm, version, (int)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&deflateStrm, 6, version, (int)sizeof(z_stream));
    int rc_deflate_params = deflateParams(&deflateStrm, 5, 0);

    // Step 3: Operate (compress source, write compressed data to gz file, obtain offset, then uncompress)
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    gzFile gzf = gzopen("ztest_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, compBuf, (unsigned int)compLen);
    off64_t gz_off = gzoffset64(gzf);
    int rc_gzclose = gzclose(gzf);
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, (uLong)compLen);
    uLong crc = crc32_z(0UL, decompBuf, (z_size_t)decompLen);

    // Step 4: Validate & Cleanup
    (void)rc_inflate_init;
    (void)rc_deflate_init;
    (void)rc_deflate_params;
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)gz_off;
    (void)rc_gzclose;
    (void)rc_uncomp;
    (void)crc;
    inflateEnd(&inflateStrm);
    deflateEnd(&deflateStrm);
    free(compBuf);
    free(decompBuf);
    // API sequence test completed successfully
    return 66;
}