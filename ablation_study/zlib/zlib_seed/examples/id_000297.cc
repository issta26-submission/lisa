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
//<ID> 297
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization of source and compression buffers
    const char src_c[] = "zlib API sequence payload: compute bound -> compress -> deflateParams -> write gz";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize and configure a deflate stream, then apply deflate parameters
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_params = deflateParams(&dstrm, 6, 0);
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Write the compressed data to a gz file and query positions (32-bit and 64-bit)
    gzFile gzf = gzopen("zlib_api_seq_test.gz", "wb");
    int rc_gz_write = gzwrite(gzf, compBuf, (unsigned int)compLen);
    off_t pos32 = gztell(gzf);
    off64_t pos64 = gzoffset64(gzf);
    int rc_gz_close = gzclose(gzf);

    // step 4: Cleanup and validation placeholders
    free(compBuf);
    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_def_init;
    (void)rc_def_params;
    (void)rc_def_end;
    (void)version;
    (void)rc_gz_write;
    (void)pos32;
    (void)pos64;
    (void)rc_gz_close;
    // API sequence test completed successfully
    return 66;
}