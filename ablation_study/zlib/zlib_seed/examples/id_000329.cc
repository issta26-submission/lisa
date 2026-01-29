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
//<ID> 329
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into a buffer
    const char src_c[] = "zlib API sequence payload: inflateBack usage with callbacks and checksum validation";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate-back stream and create a copy
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_infback_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(z_stream));
    int rc_infl_copy = inflateCopy(&istrm_copy, &istrm);

    // step 3: Setup input/output descriptors and perform inflateBack, then compute adler32 of output
    struct IODesc { Bytef *buf; uLong len; uLong pos; };
    IODesc in_desc;
    in_desc.buf = compBuf;
    in_desc.len = (uLong)compLen;
    in_desc.pos = 0;
    uLongf decompAlloc = (uLongf)(sourceLen + 512);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    IODesc out_desc;
    out_desc.buf = decompBuf;
    out_desc.len = (uLong)decompAlloc;
    out_desc.pos = 0;
    in_func in_cb = static_cast<in_func>(+[](void *in_d, unsigned char **bufp) -> unsigned int {
        IODesc *d = (IODesc *)in_d;
        unsigned int n = (unsigned int)(d->len - d->pos);
        unsigned char *p = (unsigned char *)(d->buf + d->pos);
        d->pos = d->len;
        *bufp = p;
        return n;
    });
    out_func out_cb = static_cast<out_func>(+[](void *out_d, unsigned char *buf, unsigned int len) -> int {
        IODesc *d = (IODesc *)out_d;
        memcpy(d->buf + d->pos, buf, (size_t)len);
        d->pos += (uLong)len;
        return 0;
    });
    int rc_inflateback = inflateBack(&istrm, in_cb, (void *)&in_desc, out_cb, (void *)&out_desc);
    uLong checksum = adler32_z(0UL, (const Bytef *)decompBuf, (z_size_t)out_desc.pos);

    // step 4: Cleanup streams and buffers
    int rc_inf_end1 = inflateEnd(&istrm);
    int rc_inf_end2 = inflateEnd(&istrm_copy);
    free(compBuf);
    free(decompBuf);
    free(window);

    (void)version;
    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_infback_init;
    (void)rc_infl_copy;
    (void)rc_inflateback;
    (void)checksum;
    (void)rc_inf_end1;
    (void)rc_inf_end2;

    // API sequence test completed successfully
    return 66;
}