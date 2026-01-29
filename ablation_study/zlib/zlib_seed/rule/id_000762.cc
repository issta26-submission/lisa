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
//<ID> 762
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & compress source data
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 512;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    compress(compBuf, &compLen, input, src_len);

    // step 2: Initialize inflate stream and configure header/dictionary
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inf.zalloc = NULL;
    inf.zfree = NULL;
    inf.opaque = NULL;
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(head));
    inflateGetHeader(&inf, &head);
    Bytef dictBuf[16];
    memset(dictBuf, 'D', sizeof(dictBuf));
    inflateSetDictionary(&inf, dictBuf, (uInt)sizeof(dictBuf));

    // step 3: Prepare callbacks and run inflateBack with provided compressed buffer
    struct InDesc { Bytef *ptr; uInt len; };
    struct OutDesc { Bytef *ptr; uInt pos; uInt max; };
    InDesc in_desc;
    in_desc.ptr = compBuf;
    in_desc.len = (uInt)compLen;
    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    OutDesc out_desc;
    out_desc.ptr = outBuf;
    out_desc.pos = 0;
    out_desc.max = (uInt)OUT_LEN;
    in_func in_cb = [](void *desc, unsigned char **buf)->unsigned int {
        InDesc *d = (InDesc *)desc;
        unsigned int n = d->len;
        *buf = (unsigned char *)d->ptr;
        d->ptr += n;
        d->len = 0;
        return n;
    };
    out_func out_cb = [](void *out_desc, unsigned char *buf, unsigned int len)->int {
        OutDesc *od = (OutDesc *)out_desc;
        memcpy(od->ptr + od->pos, buf, (size_t)len);
        od->pos += len;
        return 0;
    };
    inflateBack(&inf, in_cb, (void *)&in_desc, out_cb, (void *)&out_desc);

    // step 4: Validate (simple CRC) and cleanup
    uLong crc_out = crc32(0L, outBuf, (uInt)src_len);
    uLong crc_in = crc32(0L, input, (uInt)src_len);
    uLong combined = crc32_combine(crc_out, crc_in, (off_t)src_len);
    inflateEnd(&inf);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)head;
    (void)combined;
    // API sequence test completed successfully
    return 66;
}