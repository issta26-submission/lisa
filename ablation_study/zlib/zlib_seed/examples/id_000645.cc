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
//<ID> 645
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and perform one-shot compression
    const char src[] = "zlib API sequence payload for combined compress/deflate/inflate test";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLongf compAlloc = (uLongf)compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = compAlloc;
    int rc_compress = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize a deflate stream with deflateInit2_ and perform a streaming deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init2 = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));
    uLongf compAlloc2 = (uLongf)(sourceLen + 128);
    Bytef *compBuf2 = (Bytef *)malloc((size_t)compAlloc2);
    memset(compBuf2, 0, (size_t)compAlloc2);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf2;
    dstrm.avail_out = (uInt)compAlloc2;
    int rc_deflate = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize an inflate stream, set a dictionary, and attempt to inflate the one-shot compressed data
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    /* Use the original source as a dictionary candidate */
    int rc_inflate_setdict = inflateSetDictionary(&istrm, source, (uInt)sourceLen);
    uLongf outAlloc = (uLongf)(sourceLen + 256);
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 4); /* Z_FINISH == 4 */
    int rc_inf_end = inflateEnd(&istrm);

    // step 4: Demonstrate gz handle creation from an existing descriptor and close for reading, then cleanup
    gzFile gf = gzdopen(0, "rb"); /* attach to stdin descriptor for demonstration */
    int rc_gzclose_r = gzclose_r(gf);
    free(compBuf);
    free(compBuf2);
    free(outBuf);
    (void)rc_compress;
    (void)rc_def_init2;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_inf_init;
    (void)rc_inflate_setdict;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}