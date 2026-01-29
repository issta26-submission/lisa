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
//<ID> 428
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it into a buffer
    const char src[] = "zlib API sequence payload: prepare -> compress -> inflateBack -> validate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize inflate stream, prepare descriptors for inflateBack, and output buffer
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    uLong outBound = sourceLen;
    Bytef *outbuf = (Bytef *)malloc((size_t)outBound);
    memset(outbuf, 0, (size_t)outBound);

    struct InDesc { Bytef *ptr; unsigned int avail; };
    struct OutDesc { Bytef *ptr; unsigned int capacity; unsigned int written; };
    InDesc ind;
    ind.ptr = comp;
    ind.avail = (unsigned int)compLen;
    OutDesc outd;
    outd.ptr = outbuf;
    outd.capacity = (unsigned int)outBound;
    outd.written = 0;

    in_func in_cb = [](void *d, unsigned char **buf) -> unsigned int {
        InDesc *id = (InDesc *)d;
        *buf = (unsigned char *)id->ptr;
        unsigned int n = id->avail;
        id->ptr += n;
        id->avail = 0;
        return n;
    };
    out_func out_cb = [](void *d, unsigned char *buf, unsigned int len) -> int {
        OutDesc *od = (OutDesc *)d;
        memcpy(od->ptr + od->written, buf, (size_t)len);
        od->written += len;
        return 0;
    };

    // step 3: Operate: run inflateBack to decompress via callbacks, then validate using adler32_combine
    int rc_inflate_back = inflateBack(&strm, in_cb, (void *)&ind, out_cb, (void *)&outd);
    uLong sum1 = adler32(0L, outd.ptr, (uInt)(outd.written / 2));
    uLong sum2 = adler32(0L, outd.ptr + (outd.written / 2), (uInt)(outd.written - (outd.written / 2)));
    uLong combined = adler32_combine(sum1, sum2, (off_t)(outd.written - (outd.written / 2)));
    gzFile gzf = gzdopen(0, "rb");
    int rc_gzclose_r = gzclose_r(gzf);

    // step 4: Cleanup inflate resources and allocated memory
    int rc_inf_end = inflateEnd(&strm);
    free(comp);
    free(outbuf);
    (void)version;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate_back;
    (void)sum1;
    (void)sum2;
    (void)combined;
    (void)rc_gzclose_r;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}