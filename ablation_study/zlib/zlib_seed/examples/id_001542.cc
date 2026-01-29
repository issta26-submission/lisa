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
//<ID> 1542
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload: example data for inflateBack and gz I/O";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, payloadLen);
    uLong compBound = compressBound(srcLen);
    Bytef *comp = new Bytef[(size_t)compBound];
    memset(comp, 0, (size_t)compBound);
    uLong compLen = compBound;
    int rc_compress = compress2(comp, &compLen, src, srcLen, 6);

    // step 2: Configure (create gz file and write original payload)
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    int written = gzwrite(gzf, src, (unsigned int)srcLen);
    int rc_close1 = gzclose(gzf);
    gzFile gzf_read = gzopen64("test_zlib_api_sequence.gz", "rb");

    // step 3: Operate (read gz, then inflate via inflateBack with callbacks)
    Bytef *gzbuf = new Bytef[(size_t)srcLen];
    memset(gzbuf, 0, (size_t)srcLen);
    int read = gzread(gzf_read, gzbuf, (unsigned int)srcLen);

    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    const char *version = zlibVersion();
    unsigned char *window = new unsigned char[1 << 15];
    memset(window, 0, (size_t)(1 << 15));

    struct InDesc { Bytef *buf; uLong len; uLong pos; };
    struct OutDesc { Bytef *out; uLong out_len; uLong pos; };

    InDesc in_desc;
    in_desc.buf = comp;
    in_desc.len = compLen;
    in_desc.pos = 0;

    OutDesc out_desc;
    out_desc.out = new Bytef[(size_t)(srcLen * 2)];
    out_desc.out_len = srcLen * 2;
    out_desc.pos = 0;
    memset(out_desc.out, 0, (size_t)(srcLen * 2));

    in_func in_cb = [](void *d, unsigned char **buf) -> unsigned int {
        InDesc *desc = (InDesc *)d;
        unsigned int avail = (unsigned int)(desc->len - desc->pos);
        *buf = (unsigned char *)(desc->buf + desc->pos);
        desc->pos = desc->len;
        return avail;
    };

    out_func out_cb = [](void *d, unsigned char *buf, unsigned int len) -> int {
        OutDesc *od = (OutDesc *)d;
        memcpy(od->out + od->pos, buf, (size_t)len);
        od->pos += (uLong)len;
        return 0;
    };

    int rc_inflate_back_init = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));
    int rc_inflate_back = inflateBack(&strm, in_cb, (void *)&in_desc, out_cb, (void *)&out_desc);
    int rc_inflate_back_end = inflateBackEnd(&strm);

    // use inflateGetDictionary (may return an error or zero depending on stream state)
    uInt dictLen = 64u;
    Bytef *dictBuf = new Bytef[(size_t)dictLen];
    memset(dictBuf, 0, (size_t)dictLen);
    int rc_getdict = inflateGetDictionary(&strm, dictBuf, &dictLen);

    // obtain an error string for one of the return codes
    const char *errstr = zError(rc_inflate_back);

    // step 4: Validate and Cleanup
    int rc_close2 = gzclose(gzf_read);
    delete [] src;
    delete [] comp;
    delete [] gzbuf;
    delete [] out_desc.out;
    delete [] window;
    delete [] dictBuf;
    (void)rc_compress;
    (void)written;
    (void)rc_close1;
    (void)read;
    (void)rc_inflate_back_init;
    (void)rc_inflate_back;
    (void)rc_inflate_back_end;
    (void)rc_getdict;
    (void)errstr;
    (void)rc_close2;
    // API sequence test completed successfully
    return 66;
}