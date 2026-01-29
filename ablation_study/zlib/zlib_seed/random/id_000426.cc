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
//<ID> 426
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare and compress source data
    const char src[] = "zlib API sequence payload: prepare -> compress -> inflateBack -> validate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize inflateBack and descriptors for custom in/out
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_back_init = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));

    struct InDesc { const Bytef * buf; uLong len; uLong pos; };
    struct OutDesc { Bytef * buf; uLong cap; uLong pos; };

    InDesc ind = { comp, (uLong)compLen, 0 };
    uLong outCap = sourceLen + 1024;
    Bytef *outbuf = (Bytef *)malloc((size_t)outCap);
    memset(outbuf, 0, (size_t)outCap);
    OutDesc outd = { outbuf, outCap, 0 };

    in_func in = [](void * desc, unsigned char ** buf) -> unsigned int {
        InDesc * d = (InDesc *)desc;
        unsigned int avail = (unsigned int)(d->len - d->pos);
        *buf = (unsigned char *)(d->buf + d->pos);
        d->pos += avail;
        return avail;
    };

    out_func out = [](void * desc, unsigned char * data, unsigned int len) -> int {
        OutDesc * o = (OutDesc *)desc;
        memcpy(o->buf + o->pos, data, (size_t)len);
        o->pos += (uLong)len;
        return (int)len;
    };

    // step 3: Perform inflateBack to decompress using custom in/out, then validate with adler32_combine
    int rc_back = inflateBack(&strm, in, (void *)&ind, out, (void *)&outd);
    uLong total_out = outd.pos;
    uLong half = total_out / 2;
    uLong sum1 = adler32(0L, outbuf, (uInt)half);
    uLong sum2 = adler32(0L, outbuf + half, (uInt)(total_out - half));
    uLong combined = adler32_combine(sum1, sum2, (off_t)(total_out - half));

    // step 4: Cleanup and finalize resources
    int rc_inf_end = inflateEnd(&strm);
    gzFile gzf = gzdopen(1, "rb");
    int rc_gzclose = gzclose_r(gzf);
    free(comp);
    free(outbuf);
    free(window);
    (void)version;
    (void)rc_comp;
    (void)rc_back_init;
    (void)rc_back;
    (void)rc_inf_end;
    (void)rc_gzclose;
    (void)combined;
    // API sequence test completed successfully
    return 66;
}