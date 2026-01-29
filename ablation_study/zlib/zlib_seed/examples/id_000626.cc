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
//<ID> 626
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and buffers
    const char src[] = "zlib API sequence test payload for compress and gzwrite";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBufSize = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufSize);
    memset(compBuf, 0, (size_t)compBufSize);
    uLongf compLen = (uLongf)compBufSize;

    // step 2: Initialize streams (deflate source, deflate copy target, inflate for validation)
    z_stream dstrm;
    z_stream dstrm_copy;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(z_stream));
    memset(&dstrm_copy, 0, sizeof(z_stream));
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_copy = deflateCopy(&dstrm_copy, &dstrm);
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Operate - compress the source buffer and write compressed bytes to a gzFile
    int rc_compress = compress(compBuf, &compLen, source, sourceLen);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)compLen);
    int rc_gzeof = gzeof(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate & Cleanup - end streams and free buffers
    int rc_def_end_src = deflateEnd(&dstrm);
    int rc_def_end_copy = deflateEnd(&dstrm_copy);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    (void)rc_def_init;
    (void)rc_def_copy;
    (void)rc_inf_init;
    (void)rc_compress;
    (void)rc_gzwrite;
    (void)rc_gzeof;
    (void)rc_gzclose;
    (void)rc_def_end_src;
    (void)rc_def_end_copy;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}