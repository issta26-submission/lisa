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
//<ID> 598
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and produce a compressed payload with compress2
    const char src[] = "zlib api sequence payload for testing inflatePrime and deflateSetDictionary";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    const char *version = zlibVersion();
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize a deflate stream and set a preset dictionary, then clean it up
    const Bytef dict[] = "common_dictionary_for_demo";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_setdict = deflateSetDictionary(&dstrm, dict, dictLen);
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize an inflate stream, prime it, and perform inflation into an output buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLong outBound = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outBound;
    int rc_prime = inflatePrime(&istrm, 0, 0);
    int rc_inflate = inflate(&istrm, 4); /* Z_FINISH == 4 */
    int rc_inf_end = inflateEnd(&istrm);

    // step 4: Cleanup resources and finalize
    free(compBuf);
    free(outBuf);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_def_setdict;
    (void)rc_def_end;
    (void)rc_inf_init;
    (void)rc_prime;
    (void)rc_inflate;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}