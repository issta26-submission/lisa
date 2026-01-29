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
//<ID> 658
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and allocate buffers
    const char src[] = "zlib API sequence test payload for compress/decompress and gz tell";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    uLongf compAlloc_f = (uLongf)compAlloc;
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc_f);
    memset(compBuf, 0, (size_t)compAlloc_f);
    uLongf compLen = compAlloc_f;
    int rc_compress = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize a deflate stream (configuration) and an inflate stream (operation)
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Prepare output buffer, perform inflate, and initialize inflateBack with a window
    uLongf outAlloc = (uLongf)(sourceLen + 256);
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 4);

    z_stream bstrm;
    memset(&bstrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_inflate_back_init = inflateBackInit_(&bstrm, 15, window, version, (int)sizeof(z_stream));

    // step 4: Persist compressed bytes to a gzFile, query current position, and cleanup
    gzFile gf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf_w, compBuf, (unsigned int)compLen);
    int rc_gzclose_w = gzclose(gf_w);
    gzFile gf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off_t pos = gztell(gf_r);
    int rc_gzclose_r = gzclose(gf_r);

    int rc_inflate_end = inflateEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inflate_back_end = inflateBackEnd(&bstrm);

    free(compBuf);
    free(outBuf);
    free(window);

    (void)rc_compress;
    (void)rc_def_init;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inflate_back_init;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)pos;
    (void)rc_gzclose_r;
    (void)rc_inflate_end;
    (void)rc_def_end;
    (void)rc_inflate_back_end;

    // API sequence test completed successfully
    return 66;
}