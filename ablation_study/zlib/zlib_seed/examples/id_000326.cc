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
//<ID> 326
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress it
    const char src_c[] = "zlib API sequence payload: inflateBack with callbacks -> copy -> checksum -> cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate-back stream and buffers
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_infb_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    uLongf decompAlloc = (uLongf)(sourceLen + 512);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);

    // step 3: Setup input/output descriptors, callbacks, perform inflateBack and copy stream
    struct InputDesc { Bytef * data; unsigned int avail; unsigned int pos; };
    struct OutputDesc { Bytef * data; unsigned int capacity; unsigned int pos; };
    InputDesc inDesc;
    inDesc.data = compBuf;
    inDesc.avail = (unsigned int)compLen;
    inDesc.pos = 0;
    OutputDesc outDesc;
    outDesc.data = decompBuf;
    outDesc.capacity = (unsigned int)decompAlloc;
    outDesc.pos = 0;
    in_func in_cb = [](void * desc, unsigned char ** buf) -> unsigned int {
        InputDesc * d = (InputDesc *)desc;
        unsigned int n = d->avail;
        *buf = (unsigned char *)(d->data + d->pos);
        d->pos += n;
        d->avail = 0;
        return n;
    };
    out_func out_cb = [](void * desc, unsigned char * buf, unsigned int len) -> int {
        OutputDesc * o = (OutputDesc *)desc;
        memcpy(o->data + o->pos, buf, (size_t)len);
        o->pos += len;
        return 0;
    };
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(z_stream));
    int rc_infl_copy = inflateCopy(&istrm_copy, &istrm);
    int rc_inflate_back = inflateBack(&istrm, in_cb, &inDesc, out_cb, &outDesc);
    uLong checksum = adler32_z(0L, (const Bytef *)outDesc.data, (z_size_t)outDesc.pos);

    // step 4: Cleanup streams and buffers
    int rc_back_end_copy = inflateBackEnd(&istrm_copy);
    int rc_back_end = inflateBackEnd(&istrm);
    int rc_inf_end_copy = inflateEnd(&istrm_copy);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(decompBuf);
    free(window);

    (void)version;
    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_infb_init;
    (void)decompAlloc;
    (void)rc_infl_copy;
    (void)rc_inflate_back;
    (void)checksum;
    (void)rc_back_end_copy;
    (void)rc_back_end;
    (void)rc_inf_end_copy;
    (void)rc_inf_end;

    // API sequence test completed successfully
    return 66;
}