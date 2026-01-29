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
//<ID> 128
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
    const char src_c[] = "zlib API sequence payload: prime bits, compress, inflate with dict & checksum";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    Bytef dict_c[] = "example-dictionary";
    const Bytef *dictionary = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    const char *version = zlibVersion();
    uLong adler_before = adler32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Setup (initialize deflate, allocate buffers, prime the bit buffer)
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    dstrm.zalloc = 0;
    dstrm.zfree = 0;
    dstrm.opaque = 0;
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;
    int rc_def_prime = deflatePrime(&dstrm, 3, 5);
    int rc_def = deflate(&dstrm, 0);
    uLong compressed_size = dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Core operations (initialize inflate with specific window bits, set dictionary, inflate, compute checksum)
    istrm.zalloc = 0;
    istrm.zfree = 0;
    istrm.opaque = 0;
    int rc_inf_init = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compressed_size;
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)sourceLen;
    int rc_set_dict = inflateSetDictionary(&istrm, dictionary, dictLen);
    int rc_inflate = inflate(&istrm, 0);
    uLong adler_after = adler32_z(0UL, outBuf, (z_size_t)sourceLen);

    // step 4: Validate & Cleanup
    (void)rc_def_init; (void)rc_def_prime; (void)rc_def; (void)rc_def_end;
    (void)rc_inf_init; (void)rc_set_dict; (void)rc_inflate;
    (void)adler_before; (void)adler_after; (void)compressed_size;
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}