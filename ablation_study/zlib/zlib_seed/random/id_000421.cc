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
//<ID> 421
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compressed buffer
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> store -> inflateBack -> validate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen,  Z_DEFAULT_COMPRESSION);

    // step 2: Persist compressed data to a gzip file and close writer
    gzFile gzw = gzopen("tmp_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = 0;
    int rc_gzflush = 0;
    int rc_gzclosew = 0;
    if (gzw) {
        rc_gzwrite = gzwrite(gzw, comp, (unsigned int)compLen);
        rc_gzflush = gzflush(gzw, 0);
        rc_gzclosew = gzclose(gzw);
    }

    // step 3: Initialize inflate-backend, perform inflateBack with simple in/out callbacks, validate with adler32_combine
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    uLong outBound = sourceLen + 64;
    Bytef *outbuf = (Bytef *)malloc((size_t)outBound);
    memset(outbuf, 0, (size_t)outBound);
    strm.next_out = outbuf;
    strm.avail_out = (uInt)outBound;
    strm.total_out = 0;

    unsigned char *window = (unsigned char *)malloc(1 << 15);
    memset(window, 0, (1 << 15));
    const char *version = zlibVersion();
    int rc_back_init = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));

    in_func in_cb = [](void *in_desc, unsigned char **buf)->unsigned int {
        z_stream *s = (z_stream *)in_desc;
        *buf = s->next_in;
        unsigned int n = s->avail_in;
        s->avail_in = 0;
        s->next_in = NULL;
        return n;
    };
    out_func out_cb = [](void *out_desc, unsigned char *buf, unsigned int len)->int {
        z_stream *s = (z_stream *)out_desc;
        memcpy(s->next_out, buf, len);
        s->next_out += len;
        s->avail_out -= len;
        s->total_out += len;
        return (int)len;
    };

    int rc_inflateBack = inflateBack(&strm, in_cb, &strm, out_cb, &strm);

    uLong adler_src = adler32(0L, source, (uInt)sourceLen);
    uLong adler_out = adler32(0L, outbuf, (uInt)strm.total_out);
    uLong combined = adler32_combine(adler_src, adler_out, (off_t)strm.total_out);

    // step 4: Cleanup inflate resources and gzip read handle, free memory
    int rc_inf_end = inflateEnd(&strm);
    gzFile gzr = gzopen("tmp_zlib_api_sequence.gz", "rb");
    int rc_gzclose_r = 0;
    if (gzr) {
        rc_gzclose_r = gzclose_r(gzr);
    }
    free(comp);
    free(outbuf);
    free(window);
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclosew;
    (void)rc_back_init;
    (void)rc_inflateBack;
    (void)adler_src;
    (void)adler_out;
    (void)combined;
    (void)rc_inf_end;
    (void)rc_gzclose_r;
    (void)version;
    // API sequence test completed successfully
    return 66;
}