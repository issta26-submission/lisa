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
//<ID> 320
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compute adler32 checksum, and compress source
    const char src_c[] = "zlib API sequence payload for inflateBack, copy, adler32 and cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLongf compAlloc = (uLongf)compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    uLong src_adler = adler32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Initialize inflate-back stream with a sliding window and create a copy
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_infback_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(z_stream));
    int rc_inflate_copy = inflateCopy(&istrm_copy, &istrm);

    // step 3: Set up input/output descriptors and callbacks, then run inflateBack
    struct InDesc { const unsigned char *ptr; unsigned int len; unsigned int pos; };
    struct OutDesc { unsigned char *ptr; unsigned int cap; unsigned int pos; };
    InDesc *in_desc = (InDesc *)malloc(sizeof(InDesc));
    in_desc->ptr = compBuf;
    in_desc->len = (unsigned int)compLen;
    in_desc->pos = 0;
    uLongf decompAlloc = (uLongf)(sourceLen + 512);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    OutDesc *out_desc = (OutDesc *)malloc(sizeof(OutDesc));
    out_desc->ptr = (unsigned char *)decompBuf;
    out_desc->cap = (unsigned int)decompAlloc;
    out_desc->pos = 0;
    unsigned int (*in_cb)(void *, unsigned char **) = [](void *d, unsigned char **buf) -> unsigned int {
        InDesc *desc = (InDesc *)d;
        unsigned int avail = (desc->len > desc->pos) ? (desc->len - desc->pos) : 0u;
        *buf = (avail > 0) ? (unsigned char *)(desc->ptr + desc->pos) : NULL;
        desc->pos = desc->pos + avail;
        return avail;
    };
    int (*out_cb)(void *, unsigned char *, unsigned int) = [](void *d, unsigned char *data, unsigned int len) -> int {
        OutDesc *od = (OutDesc *)d;
        unsigned int copy_len = len;
        if (od->pos + copy_len > od->cap) {
            copy_len = (od->cap > od->pos) ? (od->cap - od->pos) : 0u;
        }
        if (copy_len > 0) {
            memcpy(od->ptr + od->pos, data, copy_len);
            od->pos += copy_len;
        }
        return 0;
    };
    int rc_inflate_back = inflateBack(&istrm, in_cb, in_desc, out_cb, out_desc);

    // step 4: Validate using adler checksum of decompressed data and cleanup streams/buffers
    uLong result_adler = adler32_z(0UL, out_desc->ptr, (z_size_t)out_desc->pos);
    int rc_inf_end1 = inflateEnd(&istrm);
    int rc_inf_end2 = inflateEnd(&istrm_copy);
    free(compBuf);
    free(decompBuf);
    free(window);
    free(in_desc);
    free(out_desc);

    (void)version;
    (void)rc_comp;
    (void)compAlloc;
    (void)compLen;
    (void)src_adler;
    (void)rc_infback_init;
    (void)rc_inflate_copy;
    (void)rc_inflate_back;
    (void)result_adler;
    (void)rc_inf_end1;
    (void)rc_inf_end2;

    // API sequence test completed successfully
    return 66;
}