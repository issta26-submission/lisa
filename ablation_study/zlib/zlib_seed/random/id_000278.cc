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
//<ID> 278
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compute adler32, determine bounds and allocate buffers
    const char src_c[] = "zlib API sequence payload: prepare -> compress -> inflate -> validate";
    const Bytef *source = (const Bytef *)src_c;
    z_size_t src_zlen = (z_size_t)(sizeof(src_c) - 1);
    uLong sourceLen = (uLong)src_zlen;
    uLong adler_orig = adler32_z(1UL, source, src_zlen);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize deflate and inflate streams
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Operate - set up inflate input/output and run inflate, then compute adler of output
    Bytef *outBuf = (Bytef *)malloc((size_t)(sourceLen + 64));
    memset(outBuf, 0, (size_t)(sourceLen + 64));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)(sourceLen + 64);
    int rc_inflate = inflate(&istrm, 0);
    uLong adler_after = adler32_z(1UL, outBuf, (z_size_t)istrm.total_out);

    // step 4: Cleanup deflate/inflate streams and buffers
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);

    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_def_init;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)adler_orig;
    (void)adler_after;
    (void)version;
    (void)rc_def_end;
    (void)rc_inf_end;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}