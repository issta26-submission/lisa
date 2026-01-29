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
//<ID> 649
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, dictionary and compression buffer
    const char src[] = "zlib API sequence test payload to compress and decompress";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLongf compAlloc = (uLongf)compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compDestLen = compAlloc;
    int rc_compress = compress(compBuf, &compDestLen, source, sourceLen);

    // step 2: Initialize and configure a deflate stream via deflateInit2_
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init2 = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));

    // step 3: Initialize inflate stream, set a dictionary, and perform inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_set_dict = inflateSetDictionary(&istrm, source, (uInt)sourceLen);
    uLongf outAlloc = (uLongf)(sourceLen + 128);
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compDestLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 4);

    // step 4: Open a gzFile from an existing file descriptor and cleanup resources
    gzFile gf = gzdopen(0, "rb");
    int rc_gzclose_r = gzclose_r(gf);
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)rc_compress;
    (void)rc_def_init2;
    (void)rc_inf_init;
    (void)rc_set_dict;
    (void)rc_inflate;
    (void)rc_gzclose_r;
    (void)rc_def_end;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}