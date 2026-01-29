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
//<ID> 298
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial compression using compress()
    const char src_c[] = "zlib API sequence payload: use compress -> deflateParams -> write gz -> query offsets";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_compress = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize a deflate stream, tune parameters with deflateParams, and perform deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    uLong bound2 = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf2 = (Bytef *)malloc((size_t)bound2);
    memset(compBuf2, 0, (size_t)bound2);
    dstrm.next_out = compBuf2;
    dstrm.avail_out = (uInt)bound2;
    int rc_deflate_params = deflateParams(&dstrm, 5, 0);
    int rc_deflate = deflate(&dstrm, 0);
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Write compressed data to a gz file and query offsets with gztell and gzoffset64
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, compBuf, (unsigned int)compLen);
    int rc_gzflush = gzflush(gzf, 0);
    off_t pos_offt = gztell(gzf);
    off64_t pos_off64 = gzoffset64(gzf);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup allocations and finalize
    free(compBuf);
    free(compBuf2);
    (void)rc_compress;
    (void)compBound;
    (void)compLen;
    (void)version;
    (void)rc_def_init;
    (void)rc_deflate_params;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)pos_offt;
    (void)pos_off64;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}