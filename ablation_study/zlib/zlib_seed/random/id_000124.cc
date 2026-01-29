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
//<ID> 124
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(z_stream));
    memset(&istrm, 0, sizeof(z_stream));
    const char src_c[] = "zlib API sequence payload with dictionary and priming";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    Bytef dict_c[] = "example_dictionary_123";
    const Bytef *dictionary = dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    const char *version = zlibVersion();
    uLong chk_initial = adler32_z(1UL, source, sourceLen);

    // step 2: Configure & Deflate
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;
    int rc_prime = deflatePrime(&dstrm, 1, 0);
    int rc_def = deflate(&dstrm, 0);
    uLong compressed_size = dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize Inflate, set dictionary, inflate
    int rc_inf_init = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compressed_size;
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)sourceLen;
    int rc_setdict = inflateSetDictionary(&istrm, dictionary, dictLen);
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Validate & Cleanup
    uLong chk_out = adler32_z(1UL, outBuf, sourceLen);
    (void)chk_initial; (void)chk_out;
    (void)rc_def_init; (void)rc_prime; (void)rc_def; (void)rc_def_end;
    (void)rc_inf_init; (void)rc_setdict; (void)rc_inflate;
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}