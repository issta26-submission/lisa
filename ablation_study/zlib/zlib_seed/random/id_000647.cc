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
//<ID> 647
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and perform one-shot compression
    const char src[] = "zlib API sequence test payload with dictionary";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLongf compAlloc = (uLongf)(sourceLen + 128);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = compAlloc;
    int rc_compress = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize a deflate stream with custom parameters and run deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init2 = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));
    uLongf outAlloc = (uLongf)(sourceLen + 256);
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    dstrm.next_in = compBuf;
    dstrm.avail_in = (uInt)compLen;
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outAlloc;
    int rc_deflate = deflate(&dstrm, 4);

    // step 3: Initialize an inflate stream and set a dictionary
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    const Bytef dictionary[] = { 0x10, 0x20, 0x30, 0x40 };
    uInt dictLen = (uInt)(sizeof(dictionary));
    int rc_inflate_setdict = inflateSetDictionary(&istrm, dictionary, dictLen);

    // step 4: Persist via gzdopen (using stdout fd) and perform cleanup
    gzFile gf = gzdopen(1, "wb");
    int rc_gzclose_r = gzclose_r(gf);
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)rc_compress;
    (void)rc_def_init2;
    (void)rc_deflate;
    (void)rc_inflate_init;
    (void)rc_inflate_setdict;
    (void)rc_gzclose_r;
    (void)rc_def_end;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}