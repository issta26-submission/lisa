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
//<ID> 293
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compute compression bound and compress using compress()
    const char src_c[] = "zlib API sequence payload: compress -> write gz -> deflate params -> deflate";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Open a gz file, write compressed data, and query offsets with gzoffset64 and gztell
    gzFile gzf = gzopen("zlib_seq_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)compLen);
    off64_t file_off64 = gzoffset64(gzf);
    off_t file_off = gztell(gzf);
    int rc_gzflush = gzflush(gzf, 0);

    // step 3: Initialize a deflate stream, adjust parameters with deflateParams, and perform a deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_params = deflateParams(&dstrm, 9, 1);
    uLong dBound = deflateBound(&dstrm, sourceLen);
    Bytef *dOut = (Bytef *)malloc((size_t)dBound);
    memset(dOut, 0, (size_t)dBound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = dOut;
    dstrm.avail_out = (uInt)dBound;
    int rc_def = deflate(&dstrm, 0);

    // step 4: Cleanup streams, files and buffers
    int rc_def_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gzf);
    free(compBuf);
    free(dOut);

    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_gzwrite;
    (void)file_off64;
    (void)file_off;
    (void)rc_gzflush;
    (void)rc_def_init;
    (void)rc_def_params;
    (void)dBound;
    (void)rc_def;
    (void)rc_def_end;
    (void)rc_gzclose;
    (void)version;
    // API sequence test completed successfully
    return 66;
}