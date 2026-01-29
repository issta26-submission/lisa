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
//<ID> 115
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defstrm;
    z_stream infstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&infstrm, 0, sizeof(infstrm));
    unsigned char source[64];
    memset(source, 'A', sizeof(source));
    uLong sourceLen = (uLong)sizeof(source);
    uLong bound = compressBound(sourceLen);
    unsigned char * compBuf = (unsigned char *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Compress (high-level) and init a deflate stream for demonstration
    compress((Bytef *)compBuf, &compLen, (const Bytef *)source, sourceLen);
    deflateInit2_(&defstrm, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Prepare inflateBack and callbacks
    struct InDesc { unsigned char * buf; uLong len; uLong pos; };
    struct OutDesc { unsigned char * buf; uLong cap; uLong pos; };
    InDesc inDesc;
    OutDesc outDesc;
    inDesc.buf = compBuf;
    inDesc.len = (uLong)compLen;
    inDesc.pos = 0;
    outDesc.cap = (uLong)65536;
    outDesc.buf = (unsigned char *)malloc((size_t)outDesc.cap);
    memset(outDesc.buf, 0, (size_t)outDesc.cap);
    outDesc.pos = 0;
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&infstrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    in_func in_cb = +[](void * desc, unsigned char ** buf) -> unsigned int {
        InDesc * d = (InDesc *)desc;
        unsigned int avail = (unsigned int)(d->len - d->pos);
        *buf = d->buf + d->pos;
        d->pos = d->len;
        return avail;
    };
    out_func out_cb = +[](void * desc, unsigned char * outbuf, unsigned int outlen) -> int {
        OutDesc * d = (OutDesc *)desc;
        unsigned int copy_len = outlen;
        memcpy(d->buf + d->pos, outbuf, (size_t)copy_len);
        d->pos += copy_len;
        return (int)copy_len;
    };
    inflateBack(&infstrm, in_cb, &inDesc, out_cb, &outDesc);

    // step 4: Write out metadata with gzputs, cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, "zlib-back-demo\n");
    gzclose(gz);
    inflateBackEnd(&infstrm);
    deflateEnd(&defstrm);
    free(compBuf);
    free(outDesc.buf);
    free(window);

    // API sequence test completed successfully
    return 66;
}