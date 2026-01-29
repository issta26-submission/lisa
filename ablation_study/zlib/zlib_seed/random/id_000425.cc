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
//<ID> 425
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it into a buffer
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> inflateBack -> combine -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate-back stream with a sliding window and descriptors
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_back_init = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));

    struct InDesc { const unsigned char *data; uLong len; uLong pos; };
    struct OutDesc { unsigned char *data; uLong size; uLong pos; };
    InDesc in_desc;
    in_desc.data = comp;
    in_desc.len = (uLong)compLen;
    in_desc.pos = 0u;
    uLong outBufSize = sourceLen ? sourceLen * 2u : 1024u;
    Bytef *outbuf = (Bytef *)malloc((size_t)outBufSize);
    memset(outbuf, 0, (size_t)outBufSize);
    OutDesc out_desc;
    out_desc.data = outbuf;
    out_desc.size = outBufSize;
    out_desc.pos = 0u;

    in_func in_cb = +[](void *d, unsigned char **buf) -> unsigned int {
        InDesc *desc = (InDesc *)d;
        unsigned int avail = (unsigned int)(desc->len - desc->pos);
        *buf = (unsigned char *)(desc->data + desc->pos);
        desc->pos += avail;
        return avail;
    };
    out_func out_cb = +[](void *d, unsigned char *buf, unsigned int len) -> int {
        OutDesc *od = (OutDesc *)d;
        if (len && od->pos + (uLong)len <= od->size) {
            memcpy(od->data + od->pos, buf, (size_t)len);
            od->pos += (uLong)len;
        }
        return 0;
    };

    // step 3: Perform inflateBack, then validate by computing adler32 segments and combining
    int rc_inflateback = inflateBack(&strm, in_cb, &in_desc, out_cb, &out_desc);
    uLong total_out = out_desc.pos;
    uLong part1 = total_out / 2u;
    uLong part2 = total_out - part1;
    uLong a1 = adler32(0L, outbuf, (uInt)part1);
    uLong a2 = adler32(0L, outbuf + part1, (uInt)part2);
    uLong combined = adler32_combine(a1, a2, (off_t)part2);

    // step 4: Cleanup inflate stream, gz resource, and allocated memory
    int rc_inf_end = inflateEnd(&strm);
    gzFile gzf = gzdopen(1, "rb");
    int rc_gzclose_r = gzclose_r(gzf);
    free(comp);
    free(outbuf);
    free(window);
    (void)rc_comp;
    (void)rc_back_init;
    (void)rc_inflateback;
    (void)total_out;
    (void)a1;
    (void)a2;
    (void)combined;
    (void)rc_inf_end;
    (void)rc_gzclose_r;
    (void)version;
    // API sequence test completed successfully
    return 66;
}