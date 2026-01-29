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
//<ID> 67
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream src;
    z_stream copy;
    z_stream infl;
    memset(&src, 0, sizeof(src));
    memset(&copy, 0, sizeof(copy));
    memset(&infl, 0, sizeof(infl));
    unsigned char input[128];
    unsigned char decompBuf[512];
    memset(input, 'A', sizeof(input));
    memset(decompBuf, 0, sizeof(decompBuf));
    uLong sourceLen = (uLong)sizeof(input);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);
    memset(comp, 0, (size_t)cb);
    unsigned int comp_len = 0;
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    gzFile gz = 0;

    // step 2: Configure (compress with deflate, copy deflate state)
    deflateInit_(&src, 6, zlibVersion(), (int)sizeof(z_stream));
    src.next_in = (Bytef *)input;
    src.avail_in = (uInt)sourceLen;
    src.next_out = (Bytef *)comp;
    src.avail_out = (uInt)cb;
    deflate(&src, 0);
    comp_len = (unsigned int)src.total_out;
    deflateCopy(&copy, &src);

    // step 3: Operate & Validate (inflateBack to decompress and write with gzputs)
    inflateBackInit_(&infl, 15, window, zlibVersion(), (int)sizeof(z_stream));
    struct InDesc { unsigned char *ptr; unsigned int len; };
    struct OutDesc { unsigned char *buf; unsigned int written; };
    InDesc in_desc = { comp, comp_len };
    OutDesc out_desc = { decompBuf, 0 };
    auto in_fn = +[](void *desc, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)desc;
        *buf = d->ptr;
        unsigned int n = d->len;
        d->ptr += n;
        d->len = 0;
        return n;
    };
    auto out_fn = +[](void *out_desc, unsigned char *out_buf, unsigned int out_len) -> int {
        OutDesc *od = (OutDesc *)out_desc;
        memcpy(od->buf + od->written, out_buf, (size_t)out_len);
        od->written += out_len;
        return 0;
    };
    inflateBack(&infl, (in_func)in_fn, (void *)&in_desc, (out_func)out_fn, (void *)&out_desc);
    if (out_desc.written < sizeof(decompBuf)) {
        decompBuf[out_desc.written] = '\0';
    } else {
        decompBuf[sizeof(decompBuf) - 1] = '\0';
    }
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, (const char *)decompBuf);

    // step 4: Cleanup
    inflateBackEnd(&infl);
    inflateEnd(&infl);
    deflateEnd(&src);
    deflateEnd(&copy);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}