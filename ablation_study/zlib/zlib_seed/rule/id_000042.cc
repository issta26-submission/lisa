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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream istrm_simple;
    z_stream istrm_back;
    memset(&istrm_simple, 0, sizeof(istrm_simple));
    memset(&istrm_back, 0, sizeof(istrm_back));
    unsigned char input[64];
    memset(input, 'Z', sizeof(input));
    uLong sourceLen = (uLong)sizeof(input);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);
    memset(comp, 0, (size_t)cb);
    unsigned char decomp[128];
    memset(decomp, 0, sizeof(decomp));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));

    // step 2: Setup
    uLongf compLen = (uLongf)cb;
    compress2(comp, &compLen, input, sourceLen, 6);
    istrm_simple.next_in = comp;
    istrm_simple.avail_in = (uInt)compLen;
    inflateInit2_(&istrm_simple, 15, zlibVersion(), (int)sizeof(z_stream));
    inflateBackInit_(&istrm_back, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate & Validate
    inflateSync(&istrm_simple);
    struct InDesc { unsigned char *ptr; unsigned int len; };
    InDesc in_desc = { comp, (unsigned int)compLen };
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
    istrm_back.next_in = comp;
    istrm_back.avail_in = (uInt)compLen;
    istrm_back.next_out = decomp;
    istrm_back.avail_out = (uInt)sizeof(decomp);
    inflateBack(&istrm_back, (in_func)in_fn, (void *)&in_desc, (out_func)out_fn, (void *)decomp);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)decomp[0]);

    // step 4: Cleanup
    inflateBackEnd(&istrm_back);
    inflateEnd(&istrm_simple);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}