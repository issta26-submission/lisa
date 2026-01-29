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
//<ID> 1543
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload for inflateBack and gz I/O";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    uLong compBound = compressBound(srcLen);
    Bytef *comp = new Bytef[(size_t)compBound];
    memset(comp, 0, (size_t)compBound);
    uLong compLen = compBound;
    int rc_compress = compress2(comp, &compLen, src, srcLen, 6);
    (void)rc_compress;

    // step 2: Configure (write compressed data into a gz file using gzopen64)
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    int written = gzwrite(gzf, comp, (unsigned int)compLen);
    int rc_gzclose = gzclose(gzf);
    (void)written;
    (void)rc_gzclose;

    // step 3: Operate (initialize inflateBack, run inflateBack with callbacks, query dictionary and zError)
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    unsigned char *window = new unsigned char[1 << 15];
    memset(window, 0, (size_t)(1 << 15));
    int rc_inflate_back_init = inflateBackInit_(&strm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    (void)rc_inflate_back_init;

    struct InDesc { Bytef * data; uLong len; uLong pos; };
    struct OutDesc { Bytef * out; uLong pos; uLong total; };
    InDesc inDesc;
    inDesc.data = comp;
    inDesc.len = compLen;
    inDesc.pos = 0;
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);
    OutDesc outDesc;
    outDesc.out = outBuf;
    outDesc.pos = 0;
    outDesc.total = 0;

    in_func in_cb = +[](void * in_desc, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)in_desc;
        unsigned int rem = (unsigned int)(d->len - d->pos);
        *buf = (unsigned char *)(d->data + d->pos);
        d->pos = d->len;
        return rem;
    };
    out_func out_cb = +[](void * out_desc, unsigned char *buf, unsigned int len) -> int {
        OutDesc *o = (OutDesc *)out_desc;
        memcpy(o->out + o->pos, buf, (size_t)len);
        o->pos += (uLong)len;
        o->total += (uLong)len;
        return 0;
    };

    int rc_inflate_back = inflateBack(&strm, in_cb, (void *)&inDesc, out_cb, (void *)&outDesc);
    uInt dictLen = (uInt)16;
    Bytef dictBuf[16];
    memset(dictBuf, 0, 16);
    int rc_getdict = inflateGetDictionary(&strm, dictBuf, &dictLen);
    const char *err_msg = zError(rc_inflate_back);
    (void)rc_inflate_back;
    (void)rc_getdict;
    (void)err_msg;

    // step 4: Validate and Cleanup
    int rc_back_end = inflateBackEnd(&strm);
    delete [] src;
    delete [] comp;
    delete [] window;
    delete [] outBuf;
    (void)rc_back_end;
    // API sequence test completed successfully
    return 66;
}