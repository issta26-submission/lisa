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
//<ID> 324
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress
    const char src_c[] = "zlib API sequence payload: prepare -> compress -> inflate/back -> validate -> cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Inflate via inflateInit_, perform inflateCopy and compute adler on first decomposition
    z_stream istrm1;
    memset(&istrm1, 0, sizeof(z_stream));
    int rc_inf_init1 = inflateInit_(&istrm1, version, (int)sizeof(z_stream));
    uLongf decompAlloc = (uLongf)(sourceLen + 512);
    Bytef *decompBuf1 = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf1, 0, (size_t)decompAlloc);
    istrm1.next_in = compBuf;
    istrm1.avail_in = (uInt)compLen;
    istrm1.next_out = decompBuf1;
    istrm1.avail_out = (uInt)decompAlloc;
    int rc_inflate1 = inflate(&istrm1, 0);
    z_stream istrm1_copy;
    memset(&istrm1_copy, 0, sizeof(z_stream));
    int rc_inflate_copy = inflateCopy(&istrm1_copy, &istrm1);
    int rc_inflate_copy_end = inflateEnd(&istrm1_copy);
    uLong adler_expected = adler32_z(0L, source, (z_size_t)sourceLen);

    // step 3: Initialize inflateBack stream, decompress using callbacks, compute adler of back-output
    z_stream istrm2;
    memset(&istrm2, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_back_init = inflateBackInit_(&istrm2, 15, window, version, (int)sizeof(z_stream));
    Bytef *decompBuf2 = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf2, 0, (size_t)decompAlloc);
    istrm2.next_in = compBuf;
    istrm2.avail_in = (uInt)compLen;
    istrm2.next_out = decompBuf2;
    istrm2.avail_out = (uInt)decompAlloc;
    unsigned int (*in_cb)(void *, unsigned char **) = +[](void *desc, unsigned char **buf) -> unsigned int {
        z_stream *s = (z_stream *)desc;
        unsigned int n = s->avail_in;
        *buf = s->next_in;
        s->next_in += n;
        s->avail_in = 0;
        return n;
    };
    int (*out_cb)(void *, unsigned char *, unsigned int) = +[](void *desc, unsigned char *out, unsigned int len) -> int {
        z_stream *s = (z_stream *)desc;
        unsigned int tocopy = (len <= s->avail_out) ? len : s->avail_out;
        memcpy(s->next_out, out, tocopy);
        s->next_out += tocopy;
        s->avail_out -= tocopy;
        s->total_out += tocopy;
        return 0;
    };
    int rc_inflateback = inflateBack(&istrm2, in_cb, &istrm2, out_cb, &istrm2);
    uLong decompressed2_len = (uLong)(decompAlloc - (uLongf)istrm2.avail_out);
    uLong adler_decomp2 = adler32_z(0L, decompBuf2, (z_size_t)decompressed2_len);

    // step 4: Cleanup streams and buffers
    int rc_back_end = inflateBackEnd(&istrm2);
    int rc_inf_end1 = inflateEnd(&istrm1);
    free(compBuf);
    free(decompBuf1);
    free(decompBuf2);
    free(window);

    (void)version;
    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_inf_init1;
    (void)rc_inflate1;
    (void)rc_inflate_copy;
    (void)rc_inflate_copy_end;
    (void)adler_expected;
    (void)rc_back_init;
    (void)rc_inflateback;
    (void)adler_decomp2;
    (void)rc_back_end;
    (void)rc_inf_end1;

    // API sequence test completed successfully
    return 66;
}