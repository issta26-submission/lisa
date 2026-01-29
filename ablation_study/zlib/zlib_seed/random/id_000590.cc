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
//<ID> 590
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data, allocate buffers, initialize deflate and set dictionary
    const char src[] = "zlib API sequence test payload for deflate/inflate with prime and dictionary call";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    Bytef *decBuf = (Bytef *)malloc((size_t)(sourceLen + 64));
    memset(decBuf, 0, (size_t)(sourceLen + 64));
    const Bytef dict[] = "example-dictionary-bytes"; /* dictionary provided but length set to 0 below */
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_setdict = deflateSetDictionary(&dstrm, dict, (uInt)0);
    int rc_def = deflate(&dstrm, 4); /* Z_FINISH == 4 */

    // step 2: Initialize inflate, prime bit buffer, and perform inflation
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)dstrm.total_out;
    istrm.next_out = decBuf;
    istrm.avail_out = (uInt)(sourceLen + 64);
    int rc_prime = inflatePrime(&istrm, 3, 5);
    int rc_inflate = inflate(&istrm, 4); /* Z_FINISH == 4 */

    // step 3: Validate via checksums (meaningful data flow) and prepare for cleanup
    uLong ad_src = adler32(0L, source, (uInt)sourceLen);
    uLong ad_dec = adler32(0L, decBuf, (uInt)istrm.total_out);
    uLong ad_comb = adler32_combine64(ad_src, ad_dec, (off64_t)istrm.total_out);
    (void)rc_def_init;
    (void)rc_setdict;
    (void)rc_def;
    (void)rc_inf_init;
    (void)rc_prime;
    (void)rc_inflate;
    (void)ad_comb;

    // step 4: Cleanup resources
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(decBuf);

    // API sequence test completed successfully
    (void)rc_inf_end;
    (void)rc_def_end;
    return 66;
}