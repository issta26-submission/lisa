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
//<ID> 1546
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload: sample data for inflateBack and gzopen64";
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
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));

    // step 2: Configure (open gz file via gzopen64 and write original payload)
    gzFile gzf = gzopen64("test_zlib_api_sequence_output.gz", "wb");
    int written = gzwrite(gzf, src, (unsigned int)srcLen);
    int rc_gzclose1 = gzclose(gzf);

    // step 3: Operate (use inflateGetDictionary, initialize inflateBack, run inflateBack)
    Bytef dictbuf[32];
    memset(dictbuf, 0, sizeof(dictbuf));
    uInt dictLen = (uInt)sizeof(dictbuf);
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    int rc_getdict = inflateGetDictionary(&strm, dictbuf, &dictLen);

    unsigned char *window = new unsigned char[1 << 15];
    memset(window, 0, (size_t)(1 << 15));
    int rc_inflateback_init = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));

    struct InDesc { Bytef *data; uLong len; uLong pos; };
    struct OutDesc { Bytef *buf; uLong cap; uLong pos; };
    InDesc inDesc;
    inDesc.data = comp;
    inDesc.len = compLen;
    inDesc.pos = 0;
    OutDesc outDesc;
    outDesc.cap = srcLen;
    outDesc.buf = new Bytef[(size_t)outDesc.cap];
    memset(outDesc.buf, 0, (size_t)outDesc.cap);
    outDesc.pos = 0;

    in_func in_cb = [](void *d, unsigned char **buf) -> unsigned int {
        InDesc *desc = (InDesc *)d;
        unsigned int avail = (unsigned int)(desc->len - desc->pos);
        *buf = desc->data + desc->pos;
        desc->pos += avail;
        return avail;
    };
    out_func out_cb = [](void *d, unsigned char *buf, unsigned int len) -> int {
        OutDesc *od = (OutDesc *)d;
        memcpy(od->buf + od->pos, buf, (size_t)len);
        od->pos += (uLong)len;
        return (int)len;
    };

    int rc_inflateback = inflateBack(&strm, in_cb, (void *)&inDesc, out_cb, (void *)&outDesc);
    int rc_inflateback_end = inflateBackEnd(&strm);

    // step 4: Validate and Cleanup
    const char *errstr = zError(rc_inflateback);
    int rc_inflate_end = inflateEnd(&strm);
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_getdict;
    (void)rc_inflateback_init;
    (void)rc_inflateback_end;
    (void)rc_inflate_end;
    (void)errstr;
    (void)written;
    (void)rc_gzclose1;

    gzFile gzf_read = gzopen64("test_zlib_api_sequence_output.gz", "rb");
    Bytef *gzbuf = new Bytef[payloadLen];
    memset(gzbuf, 0, payloadLen);
    int read = gzread(gzf_read, gzbuf, (unsigned int)srcLen);
    int rc_gzclose2 = gzclose(gzf_read);
    (void)read;
    (void)rc_gzclose2;

    delete [] src;
    delete [] comp;
    delete [] window;
    delete [] outDesc.buf;
    delete [] gzbuf;

    // API sequence test completed successfully
    return 66;
}