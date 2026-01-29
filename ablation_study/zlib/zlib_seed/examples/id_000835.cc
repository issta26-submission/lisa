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
//<ID> 835
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char src[] = "zlib api sequence payload: deflate -> inflateBack -> gzwrite -> crc32";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uInt compBufSize = (uInt)(sourceLen + 128);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufSize);
    memset(compBuf, 0, (size_t)compBufSize);
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));

    // step 2: Compress source into compBuf using deflate
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = compBufSize;
    int rc_deflate = deflate(&dstrm, 0);
    uLong compProduced = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Inflate the compressed data back using inflateBack with callbacks
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_inflate_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));

    struct InDesc { Bytef *buf; uLong size; unsigned int pos; };
    struct OutDesc { Bytef *buf; uLong cap; unsigned int pos; };
    InDesc in_desc;
    in_desc.buf = compBuf;
    in_desc.size = compProduced;
    in_desc.pos = 0;
    Bytef *decompBuf = (Bytef *)malloc((size_t)(sourceLen + 64));
    memset(decompBuf, 0, (size_t)(sourceLen + 64));
    OutDesc out_desc;
    out_desc.buf = decompBuf;
    out_desc.cap = sourceLen + 64;
    out_desc.pos = 0;

    in_func in_cb = [](void *in_d, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)in_d;
        unsigned int remain = (unsigned int)(d->size - d->pos);
        *buf = (unsigned char *)(d->buf + d->pos);
        d->pos += remain;
        return remain;
    };
    out_func out_cb = [](void *out_d, unsigned char *data, unsigned int len) -> int {
        OutDesc *d = (OutDesc *)out_d;
        memcpy(d->buf + d->pos, data, (size_t)len);
        d->pos += len;
        return 0;
    };

    int rc_inflate_back = inflateBack(&istrm, in_cb, &in_desc, out_cb, &out_desc);
    int rc_inflate_back_end = inflateBackEnd(&istrm);

    // step 4: Persist compressed data to a gzip file, inspect offset, compute crc and cleanup
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, compBuf, (unsigned int)compProduced);
    off64_t off = gzoffset64(gf);
    int rc_gz_close = gzclose(gf);
    uLong computed_crc = crc32(0L, decompBuf, (uInt)out_desc.pos);

    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)compProduced;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate_back;
    (void)rc_inflate_back_end;
    (void)rc_gz_write;
    (void)off;
    (void)rc_gz_close;
    (void)computed_crc;
    free(compBuf);
    free(decompBuf);
    free(window);
    // API sequence test completed successfully
    return 66;
}