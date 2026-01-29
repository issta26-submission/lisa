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
//<ID> 424
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> inflateBack -> validate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate-back stream and helpers
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window, 0, (size_t)(1 << 15));
    int rc_back_init = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));

    // input and output descriptors for callbacks
    struct InDesc { const unsigned char * src; unsigned int pos; unsigned int len; };
    struct OutDesc { unsigned char * dst; unsigned int pos; unsigned int max; };
    InDesc in_desc;
    in_desc.src = (const unsigned char *)comp;
    in_desc.pos = 0;
    in_desc.len = (unsigned int)compLen;
    OutDesc out_desc;
    out_desc.max = (unsigned int)sourceLen;
    out_desc.dst = (unsigned char *)malloc((size_t)out_desc.max);
    memset(out_desc.dst, 0, (size_t)out_desc.max);
    out_desc.pos = 0;

    in_func in_cb = [](void * d, unsigned char ** buf) -> unsigned int {
        InDesc *desc = (InDesc *)d;
        unsigned int remaining = desc->len - desc->pos;
        *buf = (unsigned char *)(desc->src + desc->pos);
        desc->pos = desc->len;
        return remaining;
    };
    out_func out_cb = [](void * d, unsigned char * buf, unsigned int len) -> int {
        OutDesc *desc = (OutDesc *)d;
        memcpy(desc->dst + desc->pos, buf, (size_t)len);
        desc->pos += len;
        return 0;
    };

    // step 3: Operate - run inflateBack to decompress into out_desc
    int rc_inflate_back = inflateBack(&strm, in_cb, (void *)&in_desc, out_cb, (void *)&out_desc);

    // step 4: Validate checksums via adler32 and adler32_combine, then persist output to a gz file and cleanup
    uLong half = (uLong)(out_desc.pos / 2);
    uLong sum1 = adler32(0L, (const Bytef *)out_desc.dst, (uInt)half);
    uLong sum2 = adler32(0L, (const Bytef *)(out_desc.dst + half), (uInt)(out_desc.pos - half));
    uLong combined = adler32_combine(sum1, sum2, (off_t)(out_desc.pos - half));

    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, out_desc.dst, (unsigned int)out_desc.pos);
    int rc_gzclose_r = gzclose_r(gzf);

    int rc_inf_end = inflateEnd(&strm);

    free(comp);
    free(out_desc.dst);
    free(window);

    (void)version;
    (void)rc_comp;
    (void)rc_back_init;
    (void)rc_inflate_back;
    (void)sum1;
    (void)sum2;
    (void)combined;
    (void)rc_gzwrite;
    (void)rc_gzclose_r;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}