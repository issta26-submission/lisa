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
//<ID> 552
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, query zlib version and declare streams
    const char src[] = "zlib API sequence payload: initialize -> copy -> deflate -> gzwrite -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    z_stream dstrm_copy;
    memset(&dstrm_copy, 0, sizeof(z_stream));

    // step 2: Initialize deflate stream, compute output bound and allocate buffers
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    uLong bound = deflateBound(&dstrm, sourceLen);
    uInt outSize = (uInt)bound;
    Bytef *outBuf1 = (Bytef *)malloc((size_t)bound);
    memset(outBuf1, 0, (size_t)bound);
    Bytef *outBuf2 = (Bytef *)malloc((size_t)bound);
    memset(outBuf2, 0, (size_t)bound);

    // step 3: Copy deflate stream state, perform deflate on both streams, and write outputs to a gzFile
    int rc_defcopy = deflateCopy(&dstrm_copy, &dstrm);
    dstrm.next_out = outBuf1;
    dstrm.avail_out = outSize;
    dstrm_copy.next_out = outBuf2;
    dstrm_copy.avail_out = outSize;
    int rc_def1 = deflate(&dstrm, 4);
    int rc_def2 = deflate(&dstrm_copy, 4);
    uLong compLen1 = dstrm.total_out;
    uLong compLen2 = dstrm_copy.total_out;
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite1 = gzwrite(gf, outBuf1, (unsigned int)compLen1);
    int rc_gzwrite2 = gzwrite(gf, outBuf2, (unsigned int)compLen2);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup streams and buffers
    int rc_def_end1 = deflateEnd(&dstrm);
    int rc_def_end2 = deflateEnd(&dstrm_copy);
    free(outBuf1);
    free(outBuf2);
    (void)version;
    (void)rc_def_init;
    (void)rc_defcopy;
    (void)rc_def1;
    (void)rc_def2;
    (void)rc_gzwrite1;
    (void)rc_gzwrite2;
    (void)rc_gzclose;
    (void)rc_def_end1;
    (void)rc_def_end2;
    // API sequence test completed successfully
    return 66;
}