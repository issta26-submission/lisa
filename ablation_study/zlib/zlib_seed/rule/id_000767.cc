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
//<ID> 767
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare input and compress it
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

    // step 2: Initialize inflateBack stream and window
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    const int WINDOW_BITS = 15;
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << WINDOW_BITS));
    memset(window, 0, (size_t)(1 << WINDOW_BITS));
    inflateBackInit_(&strm, WINDOW_BITS, window, ver, (int)sizeof(z_stream));

    // step 3: Setup descriptors and callbacks, call inflateGetHeader and inflateSetDictionary, run inflateBack
    struct InDesc { const Bytef *data; uLong size; uLong off; };
    struct OutDesc { Bytef *out; uLong size; uLong off; };
    InDesc in_desc;
    in_desc.data = compBuf;
    in_desc.size = (uLong)compLen;
    in_desc.off = 0;
    uLong OUT_LEN = INPUT_LEN * 2;
    OutDesc out_desc;
    out_desc.out = (Bytef *)malloc((size_t)OUT_LEN);
    memset(out_desc.out, 0, (size_t)OUT_LEN);
    out_desc.size = OUT_LEN;
    out_desc.off = 0;
    in_func in_cb = [](void *id, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)id;
        unsigned int avail = (unsigned int)(d->size - d->off);
        *buf = (unsigned char *)(d->data + d->off);
        d->off += (uLong)avail;
        return avail;
    };
    out_func out_cb = [](void *id, unsigned char *buf, unsigned int len) -> int {
        OutDesc *d = (OutDesc *)id;
        memcpy(d->out + d->off, buf, (size_t)len);
        d->off += (uLong)len;
        return 0;
    };
    gz_header head;
    memset(&head, 0, sizeof(head));
    inflateGetHeader(&strm, &head);
    Bytef dictBuf[32];
    memset(dictBuf, 'D', sizeof(dictBuf));
    inflateSetDictionary(&strm, dictBuf, (uInt)sizeof(dictBuf));
    inflateBack(&strm, in_cb, (void *)&in_desc, out_cb, (void *)&out_desc);

    // step 4: Teardown and cleanup
    inflateBackEnd(&strm);
    inflateEnd(&strm);
    free(window);
    free(input);
    free(compBuf);
    free(out_desc.out);
    (void)ver;
    (void)head;
    // API sequence test completed successfully
    return 66;
}