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
//<ID> 123
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
    const char src_c[] = "zlib API sequence payload: dictionary + prime bits -> deflate -> inflate with dictionary";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    Bytef dict_c[] = "example-dictionary";
    const Bytef *dictionary = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    const char *version = zlibVersion();
    uLong checksum_src = adler32_z(0L, source, (z_size_t)sourceLen);

    // step 2: Setup (initialize deflate, set dictionary, and prime bit buffer)
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_set_dict = deflateSetDictionary(&dstrm, dictionary, dictLen);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;
    int rc_def_prime = deflatePrime(&dstrm, 3, 5);
    int rc_def = deflate(&dstrm, 0);
    uLong compSize = dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Core operations (initialize inflate with custom windowBits, set dictionary, then inflate)
    int rc_inf_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compSize;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)sourceLen;
    int rc_inf_set_dict = inflateSetDictionary(&istrm, dictionary, dictLen);
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Validate & Cleanup
    uLong checksum_out = adler32_z(0L, outBuf, (z_size_t)sourceLen);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)rc_def_init; (void)rc_def_set_dict; (void)rc_def_prime; (void)rc_def;
    (void)rc_def_end; (void)rc_inf_init2; (void)rc_inf_set_dict; (void)rc_inflate;
    (void)rc_inf_end; (void)checksum_src; (void)checksum_out;
    // API sequence test completed successfully
    return 66;
}