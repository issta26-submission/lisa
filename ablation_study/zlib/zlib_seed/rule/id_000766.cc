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
//<ID> 766
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare input data and compress it with compress()
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1024;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    compress(compBuf, &compLen, input, src_len);

    // step 2: Initialize inflateBack stream and related buffers
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc((size_t)(1u << 15));
    memset(window, 0, (size_t)(1u << 15));
    inflateBackInit_(&strm, 15, window, ver, (int)sizeof(z_stream));

    // step 3: Setup callbacks and dictionaries, then run inflateBack()
    struct InCtx { const Bytef *data; uLong size; uLong pos; };
    struct OutCtx { Bytef *out; uLong size; uLong pos; };
    InCtx in_ctx;
    in_ctx.data = compBuf;
    in_ctx.size = (uLong)compLen;
    in_ctx.pos = 0;
    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    OutCtx out_ctx;
    out_ctx.out = outBuf;
    out_ctx.size = OUT_LEN;
    out_ctx.pos = 0;
    in_func in_cb = [](void *desc, unsigned char **buf) -> unsigned int {
        InCtx *c = (InCtx *)desc;
        unsigned int avail = (unsigned int)(c->size - c->pos);
        *buf = (unsigned char *)(c->data + c->pos);
        c->pos += (uLong)avail;
        return avail;
    };
    out_func out_cb = [](void *desc, unsigned char *buf, unsigned int len) -> int {
        OutCtx *o = (OutCtx *)desc;
        memcpy(o->out + o->pos, buf, (size_t)len);
        o->pos += (uLong)len;
        return 0;
    };
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&strm, &header);
    Bytef dictBuf[32];
    memset(dictBuf, 'D', sizeof(dictBuf));
    inflateSetDictionary(&strm, dictBuf, (uInt)sizeof(dictBuf));
    inflateBack(&strm, in_cb, &in_ctx, out_cb, &out_ctx);

    // step 4: Teardown, validate minimally with crc32, and cleanup
    inflateBackEnd(&strm);
    inflateEnd(&strm);
    uLong crc_out = crc32(0L, outBuf, (uInt)out_ctx.pos);
    uLong crc_in = crc32(0L, input, (uInt)src_len);
    uLong combined = crc32_combine(crc_out, crc_in, (off_t)src_len);
    free(input);
    free(compBuf);
    free(outBuf);
    free(window);
    (void)ver;
    (void)crc_out;
    (void)crc_in;
    (void)combined;
    // API sequence test completed successfully
    return 66;
}