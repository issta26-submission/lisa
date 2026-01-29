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
//<ID> 641
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and buffers and perform one-shot compression
    const char src[] = "zlib API sequence test payload for combined APIs; dictionary-enabled example";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLongf compAlloc = (uLongf)compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = compAlloc;
    int rc_compress = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize and configure a deflate stream using deflateInit2_
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));

    // step 3: Persist compressed bytes via gzdopen -> gzFile and close with gzclose_r
    gzFile gz_out = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gz_out, compBuf, (unsigned int)compLen);
    int rc_gzclose_r = gzclose_r(gz_out);

    // step 4: Initialize inflate stream, set dictionary, run inflate, and cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLongf outAlloc = (uLongf)(sourceLen + 128);
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_set_dict = inflateSetDictionary(&istrm, source, (uInt)sourceLen);
    int rc_inflate = inflate(&istrm, 4); /* Z_FINISH == 4 */

    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);

    free(compBuf);
    free(outBuf);

    (void)rc_compress;
    (void)rc_def_init;
    (void)rc_gzwrite;
    (void)rc_gzclose_r;
    (void)rc_inf_init;
    (void)rc_set_dict;
    (void)rc_inflate;
    (void)rc_def_end;
    (void)rc_inf_end;

    // API sequence test completed successfully
    return 66;
}