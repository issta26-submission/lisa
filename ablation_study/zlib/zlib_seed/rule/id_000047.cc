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
//<ID> 47
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream istrm1;
    z_stream istrm2;
    memset(&istrm1, 0, sizeof(istrm1));
    memset(&istrm2, 0, sizeof(istrm2));
    unsigned char src[128];
    memset(src, 'X', sizeof(src));
    uLong sourceLen = (uLong)sizeof(src);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);
    uLongf compLen = (uLongf)cb;
    compress2(comp, &compLen, src, sourceLen, 6);

    // step 2: Configure inflate streams
    unsigned char decompBuf[256];
    memset(decompBuf, 0, sizeof(decompBuf));
    istrm1.next_in = comp;
    istrm1.avail_in = (uInt)compLen;
    istrm1.next_out = decompBuf;
    istrm1.avail_out = (uInt)sizeof(decompBuf);
    inflateInit2_(&istrm1, 15, zlibVersion(), (int)sizeof(z_stream));

    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&istrm2, 15, window, zlibVersion(), (int)sizeof(z_stream));
    istrm2.next_in = comp;
    istrm2.avail_in = (uInt)compLen;
    istrm2.next_out = decompBuf;
    istrm2.avail_out = (uInt)sizeof(decompBuf);

    // step 3: Operate & Validate
    inflateSync(&istrm1);
    struct InDesc { unsigned char *ptr; unsigned int len; };
    InDesc in_desc = { comp, (unsigned int)istrm2.avail_in };
    auto in_fn = +[](void *desc, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)desc;
        *buf = d->ptr;
        unsigned int n = d->len;
        d->ptr += n;
        d->len = 0;
        return n;
    };
    auto out_fn = +[](void *out_desc, unsigned char *out_buf, unsigned int out_len) -> int {
        unsigned char *dst = (unsigned char *)out_desc;
        memcpy(dst, out_buf, (size_t)out_len);
        return 0;
    };
    inflateBack(&istrm2, (in_func)in_fn, (void *)&in_desc, (out_func)out_fn, (void *)decompBuf);

    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)decompBuf[0]);

    // step 4: Cleanup
    inflateBackEnd(&istrm2);
    inflateEnd(&istrm1);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}