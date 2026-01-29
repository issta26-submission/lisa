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
//<ID> 157
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLong compLen = bound;
    compress2(compBuf, &compLen, input, inputLen, 6);
    uLong compile_flags = zlibCompileFlags();
    const char *version = zlibVersion();

    // step 2: Setup (initialize inflater and descriptors)
    inflateInit2_(&inf_strm, 15, version, (int)sizeof(z_stream));
    struct InDesc { Bytef * buf; uLong len; uLong pos; };
    struct OutDesc { Bytef * buf; uLong cap; uLong pos; };
    InDesc inDesc;
    inDesc.buf = compBuf;
    inDesc.len = compLen;
    inDesc.pos = 0;
    OutDesc outDesc;
    outDesc.cap = inputLen * 2;
    outDesc.buf = (Bytef *)malloc((size_t)outDesc.cap);
    memset(outDesc.buf, 0, (size_t)outDesc.cap);
    outDesc.pos = 0;

    // step 3: Core operations (inflateBack using callbacks)
    in_func in_cb = [](void *desc, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)desc;
        unsigned int avail = (unsigned int)(d->len - d->pos);
        *buf = (unsigned char *)(d->buf + d->pos);
        d->pos += avail;
        return avail;
    };
    out_func out_cb = [](void *desc, unsigned char *buf, unsigned int len) -> int {
        OutDesc *d = (OutDesc *)desc;
        memcpy(d->buf + d->pos, buf, (size_t)len);
        d->pos += len;
        return 0;
    };
    inflateBack(&inf_strm, in_cb, &inDesc, out_cb, &outDesc);

    // step 4: Validate & Cleanup
    uLong checksum = adler32(0L, outDesc.buf, (uInt)outDesc.pos);
    inflateEnd(&inf_strm);
    free(compBuf);
    free(outDesc.buf);
    (void)compile_flags;
    (void)version;
    (void)checksum;
    // API sequence test completed successfully
    return 66;
}