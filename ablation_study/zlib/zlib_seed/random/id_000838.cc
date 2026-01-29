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
//<ID> 838
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Helper input descriptor for inflateBack in_func */
struct InDesc {
    const unsigned char *ptr;
    unsigned int len;
};

/* Helper output descriptor for inflateBack out_func */
struct OutDesc {
    unsigned char *buf;
    unsigned int size;
    unsigned int used;
};

/* in_func for inflateBack: provide all available data once */
static unsigned int my_in_func(void *in_desc, unsigned char **buf) {
    struct InDesc *d = (struct InDesc *)in_desc;
    if (d->len == 0) {
        *buf = NULL;
        return 0;
    }
    *buf = (unsigned char *)d->ptr;
    unsigned int n = d->len;
    d->ptr += n;
    d->len = 0;
    return n;
}

/* out_func for inflateBack: copy output into provided buffer, return 0 on success */
static int my_out_func(void *out_desc, unsigned char *data, unsigned int len) {
    struct OutDesc *o = (struct OutDesc *)out_desc;
    unsigned int space = (o->size > o->used) ? (o->size - o->used) : 0;
    unsigned int to_copy = (len <= space) ? len : space;
    if (to_copy) {
        memcpy(o->buf + o->used, data, to_copy);
        o->used += to_copy;
    }
    return 0;
}

int test_zlib_api_sequence() {
    const char src[] = "zlib api sequence payload for gzwrite, crc32 and inflateBack";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong crc_initial = crc32(0L, source, (uInt)sourceLen);
    gzFile gf = gzopen("/dev/null", "wb");
    unsigned int to_write = (unsigned int)sourceLen;
    int rc_gz_write = gzwrite(gf, source, to_write);
    off64_t file_offset = gzoffset64(gf);

    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    const char *version = zlibVersion();
    unsigned char *window = (unsigned char *)malloc(1u << 15);
    int rc_inflate_back_init = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));

    struct InDesc in_desc;
    in_desc.ptr = (const unsigned char *)source;
    in_desc.len = (unsigned int)sourceLen;

    uInt outBufSize = (uInt)(sourceLen + 32);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBufSize);
    memset(outBuf, 0, (size_t)outBufSize);

    struct OutDesc out_desc;
    out_desc.buf = outBuf;
    out_desc.size = outBufSize;
    out_desc.used = 0;

    int rc_inflate_back = inflateBack(&strm, my_in_func, &in_desc, my_out_func, &out_desc);

    int rc_inflate_back_end = inflateBackEnd(&strm);
    int rc_gz_close = gzclose(gf);

    (void)crc_initial;
    (void)rc_gz_write;
    (void)file_offset;
    (void)rc_inflate_back_init;
    (void)rc_inflate_back;
    (void)rc_inflate_back_end;
    (void)rc_gz_close;
    (void)version;

    free(window);
    free(outBuf);

    return 66;
}