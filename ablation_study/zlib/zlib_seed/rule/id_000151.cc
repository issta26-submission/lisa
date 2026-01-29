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
//<ID> 151
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    unsigned char input[128];
    unsigned char output[256];
    memset(input, 'Z', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLong compLen = bound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    uLong flags = zlibCompileFlags();
    (void)flags;

    // step 2: Setup inflate stream
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    inflateInit2_(&strm, 15, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;

    // step 3: Operate using inflateBack with callbacks (no external helper functions)
    struct OutDesc { unsigned char *dst; unsigned int max; unsigned int written; };
    OutDesc outd;
    outd.dst = output;
    outd.max = (unsigned int)sizeof(output);
    outd.written = 0;
    in_func in_cb = [](void *in_desc, unsigned char **buf) -> unsigned int {
        z_stream *s = (z_stream *)in_desc;
        unsigned int have = s->avail_in;
        *buf = s->next_in;
        s->next_in = NULL;
        s->avail_in = 0;
        return have;
    };
    out_func out_cb = [](void *out_desc, unsigned char *buf, unsigned int len) -> int {
        OutDesc *d = (OutDesc *)out_desc;
        unsigned int space = d->max - d->written;
        unsigned int tocopy = (len <= space) ? len : space;
        memcpy(d->dst + d->written, buf, tocopy);
        d->written += tocopy;
        return (int)tocopy;
    };
    inflateBack(&strm, in_cb, &strm, out_cb, &outd);

    // step 4: Validate & Cleanup
    uLong checksum = adler32(0L, output, (uInt)outd.written);
    inflateEnd(&strm);
    free(compBuf);
    (void)checksum;
    // API sequence test completed successfully
    return 66;
}