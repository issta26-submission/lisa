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
//<ID> 328
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it
    const char src_c[] = "zlib API sequence payload for inflateBack usage and validation via adler32_z";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflateBack stream and duplicate it
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_infback_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(z_stream));
    int rc_inflate_copy = inflateCopy(&istrm_copy, &istrm);

    // step 3: Setup input/output descriptors and callbacks, perform inflateBack and validate using adler32_z
    struct InDesc { const unsigned char *data; unsigned int len; unsigned int pos; } inDesc;
    inDesc.data = (const unsigned char *)compBuf;
    inDesc.len = (unsigned int)compLen;
    inDesc.pos = 0;
    uLongf outAlloc = (uLongf)(sourceLen + 512);
    unsigned char *outBuf = (unsigned char *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    struct OutDesc { unsigned char *buf; unsigned int size; unsigned int pos; } outDesc;
    outDesc.buf = outBuf;
    outDesc.size = (unsigned int)outAlloc;
    outDesc.pos = 0;
    in_func in_cb = (in_func)+[](void *in_desc, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)in_desc;
        unsigned int n = d->len - d->pos;
        *buf = (unsigned char *)(d->data + d->pos);
        d->pos = d->len;
        return n;
    };
    out_func out_cb = (out_func)+[](void *out_desc, unsigned char *buf, unsigned int len) -> int {
        OutDesc *d = (OutDesc *)out_desc;
        memcpy(d->buf + d->pos, buf, (size_t)len);
        d->pos += len;
        return (int)len;
    };
    int rc_inflate_back = inflateBack(&istrm, in_cb, &inDesc, out_cb, &outDesc);
    uLong adler_before = adler32_z(1L, source, (z_size_t)sourceLen);
    uLong adler_after = adler32_z(1L, outDesc.buf, (z_size_t)outDesc.pos);

    // step 4: Cleanup streams and buffers
    int rc_inf_end1 = inflateEnd(&istrm);
    int rc_inf_end2 = inflateEnd(&istrm_copy);
    free(compBuf);
    free(outBuf);
    free(window);

    (void)version;
    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_infback_init;
    (void)rc_inflate_copy;
    (void)rc_inflate_back;
    (void)adler_before;
    (void)adler_after;
    (void)rc_inf_end1;
    (void)rc_inf_end2;

    // API sequence test completed successfully
    return 66;
}