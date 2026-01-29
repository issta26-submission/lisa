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
//<ID> 764
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it
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

    // step 2: Initialize inflateBack with a window buffer, get header and set a dictionary
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&strm, 15, window, ver, (int)sizeof(z_stream));
    gz_header hdr;
    memset(&hdr, 0, sizeof(hdr));
    inflateGetHeader(&strm, &hdr);
    Bytef *dict = input;
    uInt dictLen = (uInt)32;
    inflateSetDictionary(&strm, dict, dictLen);

    // step 3: Perform inflateBack using callbacks to feed compressed data and collect output
    const uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    struct InDesc { const Bytef *data; uInt len; uInt pos; };
    struct OutDesc { Bytef *out; uInt len; uInt pos; };
    InDesc inDesc;
    inDesc.data = compBuf;
    inDesc.len = (uInt)compLen;
    inDesc.pos = 0;
    OutDesc outDesc;
    outDesc.out = outBuf;
    outDesc.len = (uInt)OUT_LEN;
    outDesc.pos = 0;
    in_func in_cb = [](void *d, unsigned char **buf)->unsigned int {
        InDesc *id = (InDesc *)d;
        unsigned int avail = id->len - id->pos;
        *buf = (unsigned char *)(id->data + id->pos);
        id->pos += avail;
        return avail;
    };
    out_func out_cb = [](void *d, unsigned char *buf, unsigned int len)->int {
        OutDesc *od = (OutDesc *)d;
        memcpy(od->out + od->pos, buf, (size_t)len);
        od->pos += len;
        return (int)len;
    };
    inflateBack(&strm, in_cb, (void *)&inDesc, out_cb, (void *)&outDesc);
    inflateBackEnd(&strm);

    // step 4: Validate with CRC, cleanup resources
    uLong crc_out = crc32(0L, outBuf, (uInt)outDesc.pos);
    uLong crc_in = crc32(0L, input, (uInt)src_len);
    uLong combined_crc = crc32_combine(crc_out, crc_in, (off_t)src_len);
    free(window);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)hdr;
    (void)dictLen;
    (void)combined_crc;
    // API sequence test completed successfully
    return 66;
}