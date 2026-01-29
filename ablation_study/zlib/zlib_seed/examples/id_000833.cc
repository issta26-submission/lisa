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
//<ID> 833
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char src[] = "zlib api sequence payload for inflateBack, gzwrite and crc32";
    const uLong sourceLen = (uLong)(sizeof(src) - 1);
    const Bytef *source = (const Bytef *)src;
    uLong crc_initial = crc32(0L, source, (uInt)sourceLen);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Compress original data into a raw deflate stream (deflateInit2_ with negative windowBits)
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, -15, 8, 0, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, sourceLen);
    uInt compBufSize = (uInt)((bound > 0xFFFFFFFFUL) ? 0xFFFFFFFFUL : bound);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufSize);
    memset(compBuf, 0, (size_t)compBufSize);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = compBufSize;
    int rc_deflate = deflate(&dstrm, 4);
    uLong compProduced = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Persist original data with gzwrite, then query file offset with gzoffset64 and close
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, source, (unsigned int)sourceLen);
    off64_t offset_after_write = gzoffset64(gf);
    int rc_gz_close = gzclose(gf);

    // step 4: Inflate the raw deflate stream using inflateBack with in/out callbacks, then cleanup
    unsigned char *window = (unsigned char *)malloc((size_t)(1U << 15));
    memset(window, 0, (size_t)(1U << 15));
    int rc_inflate_back_init = inflateBackInit_(&istrm, -15, window, version, (int)sizeof(z_stream));

    struct InDesc { unsigned char *buf; size_t size; size_t pos; };
    struct OutDesc { unsigned char *buf; size_t capacity; size_t pos; };
    InDesc in_desc;
    in_desc.buf = compBuf;
    in_desc.size = (size_t)compProduced;
    in_desc.pos = 0;
    OutDesc out_desc;
    out_desc.capacity = (size_t)sourceLen + 16;
    out_desc.buf = (unsigned char *)malloc(out_desc.capacity);
    memset(out_desc.buf, 0, out_desc.capacity);
    out_desc.pos = 0;

    in_func in_cb = [](void *vdesc, unsigned char **bufptr) -> unsigned int {
        InDesc *d = (InDesc *)vdesc;
        unsigned int remaining = (unsigned int)(d->size - d->pos);
        *bufptr = d->buf + d->pos;
        d->pos += remaining;
        return remaining;
    };
    out_func out_cb = [](void *vdesc, unsigned char *buf, unsigned int len) -> int {
        OutDesc *d = (OutDesc *)vdesc;
        memcpy(d->buf + d->pos, buf, (size_t)len);
        d->pos += (size_t)len;
        return 0;
    };

    int rc_inflate_back = inflateBack(&istrm, in_cb, (void *)&in_desc, out_cb, (void *)&out_desc);
    int rc_inflate_back_end = inflateBackEnd(&istrm);

    (void)crc_initial;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)compProduced;
    (void)rc_deflate_end;
    (void)rc_gz_write;
    (void)offset_after_write;
    (void)rc_gz_close;
    (void)rc_inflate_back_init;
    (void)rc_inflate_back;
    (void)rc_inflate_back_end;

    free(compBuf);
    free(window);
    free(out_desc.buf);

    // API sequence test completed successfully
    return 66;
}