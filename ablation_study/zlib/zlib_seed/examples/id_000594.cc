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
//<ID> 594
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, dictionary and buffers
    const char src[] = "zlib preset-dictionary payload for inflate/deflate sequence";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char dict_c[] = "preset_dictionary";
    const Bytef *dict = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    uInt compBufSize = (uInt)(sourceLen + 128);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufSize);
    memset(compBuf, 0, (size_t)compBufSize);
    uInt outBufSize = (uInt)(sourceLen + 256);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBufSize);
    memset(outBuf, 0, (size_t)outBufSize);

    // step 2: Initialize deflate stream, set dictionary, compress data, and finalize deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_set_dict = deflateSetDictionary(&dstrm, dict, dictLen);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = compBufSize;
    int rc_deflate = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    uLong compLen = dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize inflate stream, prime bit buffer, provide dictionary, inflate data
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = outBufSize;
    int rc_prime = inflatePrime(&istrm, 1, 0);
    int rc_inflate_set_dict = inflateSetDictionary(&istrm, dict, dictLen);
    int rc_inflate = inflate(&istrm, 4); /* Z_FINISH == 4 */

    // step 4: Cleanup inflate, free buffers, and finish
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)rc_def_init;
    (void)rc_set_dict;
    (void)rc_deflate;
    (void)compLen;
    (void)rc_def_end;
    (void)rc_inf_init;
    (void)rc_prime;
    (void)rc_inflate_set_dict;
    (void)rc_inflate;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}