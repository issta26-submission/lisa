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
//<ID> 659
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and buffers
    const char src[] = "zlib API sequence test payload for compress/decompress and gz operations";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_compress = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize deflate stream and inflate-back stream
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    z_stream ibstrm;
    memset(&ibstrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_ib_init = inflateBackInit_(&ibstrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Initialize inflate stream and perform inflate on compressed buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong outAlloc = sourceLen + 128;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Persist compressed bytes to a gzFile, query position, and cleanup
    gzFile gf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf_w, compBuf, (unsigned int)compLen);
    int rc_gzclose_w = gzclose(gf_w);
    gzFile gf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off_t pos = gztell(gf_r);
    int rc_gzclose_r = gzclose(gf_r);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_ib_end = inflateBackEnd(&ibstrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(outBuf);
    free(window);
    (void)rc_compress;
    (void)rc_def_init;
    (void)rc_ib_init;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)pos;
    (void)rc_gzclose_r;
    (void)rc_inflate_end;
    (void)rc_ib_end;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}