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
//<ID> 420
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compress to buffer, and write compressed bytes to a gz file
    const char src[] = "zlib API sequence payload: compress -> inflateBack -> inflate -> adler32_combine -> gzclose_r -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    gzFile gzw = gzopen("tmp_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gzw, comp, (unsigned int)compLen);
    int rc_gzclose_w = gzclose_w ? gzclose_w(gzw) : gzclose(gzw);

    gzFile gzr = gzopen("tmp_zlib_api_sequence.gz", "rb");
    int rc_gzclose_r = gzclose_r(gzr);

    // step 2: Inflate using inflateBack with callback providers to fill an output buffer
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(1u << 15);
    memset(window, 0, 1u << 15);
    const char *version = zlibVersion();
    int rc_back_init = inflateBackInit_(&backstrm, 15, window, version, (int)sizeof(z_stream));

    struct InDesc { const unsigned char *data; unsigned int size; unsigned int pos; };
    struct OutDesc { unsigned char *data; unsigned int size; unsigned int pos; };
    InDesc inDesc;
    inDesc.data = (const unsigned char *)comp;
    inDesc.size = (unsigned int)compLen;
    inDesc.pos = 0;
    unsigned long outBufSize = sourceLen;
    unsigned char *outbuf = (unsigned char *)malloc((size_t)outBufSize);
    memset(outbuf, 0, (size_t)outBufSize);
    OutDesc outDesc;
    outDesc.data = outbuf;
    outDesc.size = (unsigned int)outBufSize;
    outDesc.pos = 0;

    in_func in_cb = [](void *desc, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)desc;
        unsigned int provide = d->size - d->pos;
        *buf = (unsigned char *)(d->data + d->pos);
        d->pos += provide;
        return provide;
    };
    out_func out_cb = [](void *desc, unsigned char *data, unsigned int len) -> int {
        OutDesc *od = (OutDesc *)desc;
        memcpy(od->data + od->pos, data, (size_t)len);
        od->pos += len;
        return 0;
    };

    int rc_inflateBack = inflateBack(&backstrm, in_cb, &inDesc, out_cb, &outDesc);
    int rc_back_end = inflateBackEnd(&backstrm);

    // step 3: Initialize a regular inflate stream, perform inflate for validation, compute adler32s and combine them
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    Bytef *out2 = (Bytef *)malloc((size_t)sourceLen);
    memset(out2, 0, (size_t)sourceLen);
    strm.next_out = out2;
    strm.avail_out = (uInt)sourceLen;
    int rc_inflate = inflate(&strm, 0);
    uLong inflated_total = strm.total_out;
    uLong len1 = (uLong)(inflated_total / 2);
    uLong len2 = inflated_total - len1;
    uLong a1 = adler32(0L, outbuf, (uInt)len1);
    uLong a2 = adler32(0L, outbuf + len1, (uInt)len2);
    uLong combined = adler32_combine(a1, a2, (off_t)len2);
    int rc_inf_end = inflateEnd(&strm);

    // step 4: Cleanup resources
    free(comp);
    free(outbuf);
    free(out2);
    free(window);
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzclose_r;
    (void)rc_back_init;
    (void)rc_inflateBack;
    (void)rc_back_end;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)inflated_total;
    (void)a1;
    (void)a2;
    (void)combined;
    (void)version;
    // API sequence test completed successfully
    return 66;
}