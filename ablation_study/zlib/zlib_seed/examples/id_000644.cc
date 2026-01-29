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
//<ID> 644
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into a buffer
    const char src[] = "zlib API sequence test payload for combined usage of specified APIs";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLongf compAlloc = (uLongf)compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = compAlloc;
    int rc_compress = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize a deflate stream with deflateInit2_
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init2 = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));
    uLong bound_after_init = deflateBound(&dstrm, sourceLen);

    // step 3: Prepare an inflate stream and set a dictionary using compressed bytes
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    uInt dictLen = (uInt)compLen;
    int rc_inflate_set_dict = inflateSetDictionary(&istrm, compBuf, dictLen);

    // step 4: Persist compressed bytes via gzdopen/gzwrite and clean up streams and buffers
    gzFile gf = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)compLen);
    int rc_gzclose_r = gzclose_r(gf);
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);

    (void)rc_compress;
    (void)rc_def_init2;
    (void)bound_after_init;
    (void)rc_inflate_set_dict;
    (void)rc_gzwrite;
    (void)rc_gzclose_r;
    (void)rc_def_end;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}