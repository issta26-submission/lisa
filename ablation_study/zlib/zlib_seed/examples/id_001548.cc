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
//<ID> 1548
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload: sample data for inflateBack and gz I/O";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    uLong compBound = compressBound(srcLen);
    Bytef *comp = new Bytef[(size_t)compBound];
    memset(comp, 0, (size_t)compBound);
    uLong compLen = compBound;
    int rc_compress = compress(comp, &compLen, src, srcLen);
    const char *version = zlibVersion();

    // step 2: Configure (create gz file and write compressed buffer)
    gzFile gzf = gzopen64("test_zlib_api_sequence_out.gz", "wb");
    int written = gzwrite(gzf, comp, (unsigned int)compLen);
    int rc_close1 = gzclose(gzf);

    // step 3: Operate (initialize inflateBack state, run inflateBack with callbacks, get dictionary)
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    unsigned char *window = new unsigned char[1 << 15];
    memset(window, 0, (size_t)(1 << 15));
    int rc_init_back = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));

    struct InDesc { const unsigned char *data; unsigned int len; unsigned int pos; };
    struct OutDesc { unsigned char *buf; unsigned int cap; unsigned int pos; };
    InDesc inDesc;
    inDesc.data = comp;
    inDesc.len = (unsigned int)compLen;
    inDesc.pos = 0;
    OutDesc outDesc;
    outDesc.cap = (unsigned int)srcLen;
    outDesc.buf = new unsigned char[outDesc.cap];
    outDesc.pos = 0;
    memset(outDesc.buf, 0, (size_t)outDesc.cap);

    in_func in_cb = (in_func) +[](void *desc, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)desc;
        unsigned int remaining = d->len - d->pos;
        *buf = (unsigned char *)(d->data + d->pos);
        d->pos = d->len;
        return remaining;
    };
    out_func out_cb = (out_func) +[](void *desc, unsigned char *data, unsigned int len) -> int {
        OutDesc *o = (OutDesc *)desc;
        memcpy(o->buf + o->pos, data, len);
        o->pos += len;
        return 0;
    };

    int rc_inflate_back = inflateBack(&strm, in_cb, &inDesc, out_cb, &outDesc);
    uInt dictLen = 256u;
    Bytef *dictBuf = new Bytef[dictLen];
    memset(dictBuf, 0, (size_t)dictLen);
    int rc_getdict = inflateGetDictionary(&strm, dictBuf, &dictLen);
    int rc_back_end = inflateBackEnd(&strm);
    const char *errstr = zError(rc_inflate_back);

    // step 4: Validate and Cleanup
    (void)rc_compress;
    (void)written;
    (void)rc_close1;
    (void)rc_init_back;
    (void)rc_inflate_back;
    (void)rc_getdict;
    (void)rc_back_end;
    (void)errstr;
    (void)version;
    delete [] src;
    delete [] comp;
    delete [] window;
    delete [] outDesc.buf;
    delete [] dictBuf;
    // API sequence test completed successfully
    return 66;
}