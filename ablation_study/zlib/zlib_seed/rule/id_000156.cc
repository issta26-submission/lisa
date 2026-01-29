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
//<ID> 156
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    const char plain[] = "Hello zlib! This is a small test payload.";
    uLong srcLen = (uLong)strlen(plain);
    uLong bound = compressBound(srcLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(comp, &compLen, (const Bytef *)plain, srcLen, 6);

    // step 2: Setup (query compile flags and initialize inflate stream with inflateInit2_)
    uLong flags = zlibCompileFlags();
    (void)flags;
    inflateInit2_(&strm, 15, zlibVersion(), (int)sizeof(z_stream));
    strm.zalloc = 0;
    strm.zfree = 0;
    strm.opaque = 0;
    strm.next_in = 0;
    strm.avail_in = 0;

    // step 3: Operate (use inflateBack with callbacks to stream decompression)
    struct InDesc { Bytef *data; uLong left; };
    struct OutDesc { Bytef *out; uLong pos; };
    InDesc in_desc;
    in_desc.data = comp;
    in_desc.left = (uLong)compLen;
    Bytef *out_buf = (Bytef *)malloc(1024);
    memset(out_buf, 0, 1024);
    OutDesc out_desc;
    out_desc.out = out_buf;
    out_desc.pos = 0;
    in_func in_cb = [](void *vdesc, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)vdesc;
        unsigned int n = (unsigned int)d->left;
        *buf = (unsigned char *)d->data;
        d->left = 0;
        return n;
    };
    out_func out_cb = [](void *vdesc, unsigned char *data, unsigned int len) -> int {
        OutDesc *od = (OutDesc *)vdesc;
        memcpy(od->out + od->pos, data, len);
        od->pos += len;
        return 0;
    };
    inflateBack(&strm, in_cb, &in_desc, out_cb, &out_desc);

    // step 4: Validate & Cleanup
    uLong checksum = adler32(0L, out_desc.out, (uInt)out_desc.pos);
    (void)checksum;
    const char *ver = zlibVersion();
    (void)ver;
    inflateEnd(&strm);
    free(comp);
    free(out_buf);

    // API sequence test completed successfully
    return 66;
}