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
//<ID> 761
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

static unsigned int my_in(void *dptr, unsigned char **buf) {
    struct BufDesc {
        const Bytef *data;
        uLong len;
        uLong pos;
        Bytef *out;
        uLong outlen;
        uLong outpos;
    };
    struct BufDesc *d = (struct BufDesc *)dptr;
    unsigned int avail = (unsigned int)(d->len - d->pos);
    *buf = (unsigned char *)(d->data + d->pos);
    d->pos = d->len;
    return avail;
}

static int my_out(void *dptr, unsigned char *buf, unsigned int len) {
    struct BufDesc {
        const Bytef *data;
        uLong len;
        uLong pos;
        Bytef *out;
        uLong outlen;
        uLong outpos;
    };
    struct BufDesc *d = (struct BufDesc *)dptr;
    memcpy(d->out + d->outpos, buf, (size_t)len);
    d->outpos += len;
    return 0;
}

int test_zlib_api_sequence() {
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 256;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    compress(compBuf, &compLen, input, src_len);

    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inf.zalloc = NULL;
    inf.zfree = NULL;
    inf.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window, 0, (size_t)(1 << 15));
    inflateBackInit_(&inf, 15, window, ver, (int)sizeof(z_stream));

    struct BufDesc {
        const Bytef *data;
        uLong len;
        uLong pos;
        Bytef *out;
        uLong outlen;
        uLong outpos;
    };
    struct BufDesc desc;
    desc.data = compBuf;
    desc.len = (uLong)compLen;
    desc.pos = 0;
    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    desc.out = outBuf;
    desc.outlen = OUT_LEN;
    desc.outpos = 0;
    in_func in_cb = my_in;
    out_func out_cb = my_out;
    inflateBack(&inf, in_cb, (void *)&desc, out_cb, (void *)&desc);
    gz_header head;
    memset(&head, 0, sizeof(head));
    inflateGetHeader(&inf, &head);
    inflateSetDictionary(&inf, input, (uInt)32);

    inflateBackEnd(&inf);
    inflateEnd(&inf);
    free(window);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    return 66;
}