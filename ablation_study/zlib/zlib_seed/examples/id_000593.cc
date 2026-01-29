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
//<ID> 593
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, dictionary, and buffers
    const char src[] = "zlib api sequence source data for compression and inflation";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    Bytef dict[] = "preset-dictionary";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLong outBound = sourceLen + 128;
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    const char *version = zlibVersion();

    // step 2: Initialize and configure deflate, include a call to deflateSetDictionary
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_setdict = deflateSetDictionary(&dstrm, dict, 0); /* use zero-length dictionary to exercise API */
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_def = deflate(&dstrm, 4); /* Z_FINISH == 4 */

    // step 3: Initialize inflate, prime bit buffer, and perform inflation
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)dstrm.total_out;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outBound;
    int rc_prime = inflatePrime(&istrm, 3, 1);
    int rc_inflate = inflate(&istrm, 4); /* Z_FINISH == 4 */

    // step 4: Cleanup streams and buffers, validate via returned codes (silenced) and finish
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(outBuf);
    (void)rc_def_init;
    (void)rc_setdict;
    (void)rc_def;
    (void)rc_inf_init;
    (void)rc_prime;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}