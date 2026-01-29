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
//<ID> 595
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, dictionary and allocate buffers
    const char src[] = "zlib API sequence payload for deflate/inflate with dictionary and priming";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const Bytef dictBytes[] = "preset_dict";
    uInt dictLen = (uInt)(sizeof(dictBytes) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLong outBound = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);

    // step 2: Initialize deflate, configure with a dictionary, and perform compression
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_set_dict = deflateSetDictionary(&dstrm, dictBytes, dictLen);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_def = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize inflate, prime bit buffer, supply dictionary, and perform inflation
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_prime = inflatePrime(&istrm, 1, 1);
    int rc_inf_setdict = inflateSetDictionary(&istrm, dictBytes, dictLen);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)dstrm.total_out;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outBound;
    int rc_inflate = inflate(&istrm, 4); /* Z_FINISH == 4 */

    // step 4: Cleanup inflate, free buffers and finalize
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)rc_def_init;
    (void)rc_set_dict;
    (void)rc_def;
    (void)rc_def_end;
    (void)rc_inf_init;
    (void)rc_prime;
    (void)rc_inf_setdict;
    (void)rc_inflate;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}