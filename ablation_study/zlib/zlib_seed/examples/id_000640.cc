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
//<ID> 640
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and perform one-shot compress()
    const char src[] = "zlib API sequence test payload for combined usage of APIs";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLongf compAlloc = (uLongf)(sourceLen + 128);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = compAlloc;
    int rc_compress = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize a deflate stream with deflateInit2_ and run deflate()
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));
    Bytef *defOut = (Bytef *)malloc((size_t)compAlloc);
    memset(defOut, 0, (size_t)compAlloc);
    dstrm.next_in = compBuf;
    dstrm.avail_in = (uInt)compLen;
    dstrm.next_out = defOut;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_deflate = deflate(&dstrm, 4);

    // step 3: Initialize an inflate stream and set a dictionary with inflateSetDictionary()
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_setdict = inflateSetDictionary(&istrm, source, (uInt)sourceLen);

    // step 4: Open a gzFile from an existing file descriptor with gzdopen, close for read, and cleanup
    gzFile gf = gzdopen(0, "rb");
    int rc_gzclose_r = gzclose_r(gf);
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(defOut);
    (void)rc_compress;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_inf_init;
    (void)rc_setdict;
    (void)rc_gzclose_r;
    (void)rc_def_end;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}