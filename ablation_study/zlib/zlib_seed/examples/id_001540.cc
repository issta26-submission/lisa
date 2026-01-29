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
//<ID> 1540
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload: sample data for inflateBack, gzopen64 and dictionary retrieval";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    uLong compBound = compressBound(srcLen);
    Bytef *comp = new Bytef[(size_t)compBound];
    memset(comp, 0, (size_t)compBound);
    uLong compLen = compBound;
    int rc_comp = compress(comp, &compLen, src, srcLen);
    (void)rc_comp;

    // step 2: Configure (create gz file and write original payload)
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    int written = gzwrite(gzf, src, (unsigned int)srcLen);
    int rc_close1 = gzclose(gzf);
    (void)written;
    (void)rc_close1;

    // step 3: Core operations (inflateBack using callbacks, then get dictionary)
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    unsigned char *window = new unsigned char[1 << 15];
    memset(window, 0, (size_t)(1 << 15));
    const char *ver = zlibVersion();
    int rc_init_back = inflateBackInit_(&strm, 15, window, ver, (int)sizeof(z_stream));
    (void)rc_init_back;

    struct InDesc { Bytef *data; uInt len; uInt pos; };
    struct OutDesc { Bytef *buf; uInt size; uInt pos; };

    InDesc inDesc;
    inDesc.data = comp;
    inDesc.len = (uInt)compLen;
    inDesc.pos = 0U;

    OutDesc outDesc;
    outDesc.size = (uInt)srcLen;
    outDesc.buf = new Bytef[outDesc.size];
    memset(outDesc.buf, 0, (size_t)outDesc.size);
    outDesc.pos = 0U;

    in_func in_cb = +[](void *in_d, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc*)in_d;
        unsigned int remaining = (d->len > d->pos) ? (d->len - d->pos) : 0u;
        *buf = (remaining ? (unsigned char*)(d->data + d->pos) : (unsigned char*)NULL);
        d->pos = d->pos + remaining;
        return remaining;
    };
    out_func out_cb = +[](void *out_d, unsigned char *buf, unsigned int len) -> int {
        OutDesc *d = (OutDesc*)out_d;
        unsigned int to_copy = (d->size > d->pos) ? ((d->size - d->pos) >= len ? len : (d->size - d->pos)) : 0u;
        if (to_copy) {
            memcpy(d->buf + d->pos, buf, to_copy);
            d->pos += to_copy;
        }
        return 0;
    };

    int rc_back = inflateBack(&strm, in_cb, (void*)&inDesc, out_cb, (void*)&outDesc);
    const char *err_after_back = zError(rc_back);
    (void)err_after_back;

    uInt dictBufLen = 32U;
    Bytef *dictBuf = new Bytef[dictBufLen];
    memset(dictBuf, 0, (size_t)dictBufLen);
    int rc_getdict = inflateGetDictionary(&strm, dictBuf, &dictBufLen);
    (void)rc_getdict;

    int rc_back_end = inflateBackEnd(&strm);
    const char *err_after_end = zError(rc_back_end);
    (void)err_after_end;

    // step 4: Validate and Cleanup
    int rc_close2 = 0;
    (void)rc_close2;
    delete [] src;
    delete [] comp;
    delete [] window;
    delete [] outDesc.buf;
    delete [] dictBuf;
    (void)outDesc.pos;
    (void)inDesc.pos;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}