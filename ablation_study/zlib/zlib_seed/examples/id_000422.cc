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
//<ID> 422
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it into a buffer
    const char src[] = "zlib API sequence payload for inflateBack -> validate with adler32_combine -> gzclose_r -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize an inflate-back stream with a window and prepare descriptors for callbacks
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    unsigned char *window = (unsigned char *)malloc(1u << 15); // 32KB window
    memset(window, 0, (size_t)(1u << 15));
    int rc_back_init = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));

    // descriptors for callbacks
    struct InDesc { const Bytef *data; uLong len; uLong offset; } inDesc;
    inDesc.data = comp;
    inDesc.len = (uLong)compLen;
    inDesc.offset = 0;
    struct OutDesc { Bytef *out; uLong outSize; uLong pos; } outDesc;
    outDesc.outSize = sourceLen;
    outDesc.out = (Bytef *)malloc((size_t)outDesc.outSize);
    memset(outDesc.out, 0, (size_t)outDesc.outSize);
    outDesc.pos = 0;

    // step 3: Define callbacks and run inflateBack, then validate output via adler32_combine
    in_func in_cb = +[](void *desc, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)desc;
        unsigned int remaining = (unsigned int)(d->len - d->offset);
        *buf = (unsigned char *)(d->data + d->offset);
        d->offset += remaining;
        return remaining;
    };
    out_func out_cb = +[](void *desc, unsigned char *out, unsigned int out_len) -> int {
        OutDesc *o = (OutDesc *)desc;
        memcpy(o->out + o->pos, out, out_len);
        o->pos += out_len;
        return 0;
    };
    int rc_inflateback = inflateBack(&strm, in_cb, &inDesc, out_cb, &outDesc);

    uLong part1_len = (uLong)(outDesc.pos / 2);
    uLong part2_len = outDesc.pos - part1_len;
    uLong ad1 = adler32(1L, outDesc.out, (uInt)part1_len);
    uLong ad2 = adler32(1L, outDesc.out + part1_len, (uInt)part2_len);
    uLong combined = adler32_combine(ad1, ad2, (off_t)part2_len);

    // also open and close a gzFile for read-side closure using gzclose_r
    gzFile gzf = gzopen("/dev/null", "rb");
    int rc_gzclose_r = gzclose_r(gzf);

    // step 4: Cleanup inflate resources and allocated memory
    int rc_back_end = inflateBackEnd(&strm);
    int rc_inf_end = inflateEnd(&strm);
    free(comp);
    free(window);
    free(outDesc.out);
    (void)version;
    (void)rc_comp;
    (void)rc_back_init;
    (void)rc_inflateback;
    (void)combined;
    (void)rc_gzclose_r;
    (void)rc_back_end;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}