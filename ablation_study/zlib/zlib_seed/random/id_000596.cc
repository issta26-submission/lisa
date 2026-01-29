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
//<ID> 596
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and allocate buffers
    const char src[] = "zlib API sequence test payload for deflate/inflate";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLong outCap = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outCap);
    memset(outBuf, 0, (size_t)outCap);

    // step 2: Initialize deflate stream, optionally set an empty dictionary, and compress
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    /* use an empty dictionary (dict length 0) to demonstrate deflateSetDictionary usage without requiring inflateSetDictionary */
    const Bytef *dict = (const Bytef *)"";
    uInt dictLen = 0;
    int rc_def_setdict = deflateSetDictionary(&dstrm, dict, dictLen);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_deflate = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    uLong compSize = dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize inflate stream, prime bit buffer, and inflate the compressed data
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compSize;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outCap;
    int rc_prime = inflatePrime(&istrm, 0, 0); /* no-op priming but exercises API */
    int rc_inflate = inflate(&istrm, 4); /* Z_FINISH == 4 */
    int rc_inf_end = inflateEnd(&istrm);

    // step 4: Cleanup and finalize
    free(compBuf);
    free(outBuf);
    (void)rc_def_init;
    (void)rc_def_setdict;
    (void)rc_deflate;
    (void)compSize;
    (void)rc_def_end;
    (void)rc_inf_init;
    (void)rc_prime;
    (void)rc_inflate;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}