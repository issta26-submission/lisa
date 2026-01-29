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
//<ID> 591
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and dictionary, initialize deflate, set dictionary, and compress
    const char src[] = "zlib API sequence test payload with preset dictionary";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char dict_c[] = "common dict content for compression";
    const Bytef *dictionary = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    Bytef *compBuf = (Bytef *)malloc((size_t)(sourceLen + dictLen + 128));
    memset(compBuf, 0, (size_t)(sourceLen + dictLen + 128));
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)(sourceLen + dictLen + 128);
    int rc_setdict = deflateSetDictionary(&dstrm, dictionary, dictLen);
    int rc_deflate = deflate(&dstrm, 4); /* Z_FINISH == 4 */

    // step 2: Initialize inflate, prime bit buffer, and perform inflation of compressed output
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)dstrm.total_out;
    Bytef *outBuf = (Bytef *)malloc((size_t)(sourceLen + 256));
    memset(outBuf, 0, (size_t)(sourceLen + 256));
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)(sourceLen + 256);
    int rc_prime = inflatePrime(&istrm, 1, 1);
    int rc_inflate = inflate(&istrm, 4); /* Z_FINISH == 4 */

    // step 3: Cleanup streams and buffers
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(outBuf);

    (void)rc_def_init;
    (void)rc_setdict;
    (void)rc_deflate;
    (void)rc_inf_init;
    (void)rc_prime;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}