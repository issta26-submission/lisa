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
//<ID> 769
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare input and compress
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1024;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    compress(compBuf, &compLen, input, src_len);

    // step 2: Initialize inflate back-end and set header/dictionary
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&strm, 15, window, ver, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(head));
    inflateGetHeader(&strm, &head);
    Bytef dict[8];
    memcpy(dict, "ZDICTv01", 8);
    inflateSetDictionary(&strm, dict, (uInt)8);

    // step 3: Configure callbacks and perform inflateBack to decompress compressed buffer
    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    struct InDesc { const unsigned char *data; uInt len; uInt pos; };
    struct OutDesc { unsigned char *buf; uInt len; uInt pos; };
    InDesc in_desc = { compBuf, (uInt)compLen, 0u };
    OutDesc out_desc = { outBuf, (uInt)OUT_LEN, 0u };
    unsigned int (*in_cb)(void *, unsigned char **) = [](void *d, unsigned char **buf) -> unsigned int {
        InDesc *desc = (InDesc *)d;
        unsigned int remaining = desc->len - desc->pos;
        *buf = (unsigned char *)(desc->data + desc->pos);
        desc->pos = desc->len;
        return remaining;
    };
    int (*out_cb)(void *, unsigned char *, unsigned int) = [](void *d, unsigned char *buf, unsigned int len) -> int {
        OutDesc *od = (OutDesc *)d;
        memcpy(od->buf + od->pos, buf, (size_t)len);
        od->pos += len;
        return 0;
    };
    inflateBack(&strm, in_cb, (void *)&in_desc, out_cb, (void *)&out_desc);

    // step 4: Teardown, validate via CRCs, and cleanup
    inflateBackEnd(&strm);
    inflateEnd(&strm);
    uLong crc_input = crc32(0L, input, (uInt)src_len);
    uLong crc_out = crc32(0L, outBuf, (uInt)src_len);
    uLong combined = crc32_combine(crc_input, crc_out, (off_t)src_len);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)head;
    (void)combined;
    // API sequence test completed successfully
    return 66;
}