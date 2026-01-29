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
//<ID> 65
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream src;
    z_stream dst;
    z_stream infl;
    memset(&src, 0, sizeof(src));
    memset(&dst, 0, sizeof(dst));
    memset(&infl, 0, sizeof(infl));
    unsigned char input[128];
    unsigned char decompBuf[512];
    memset(input, 'A', sizeof(input));
    memset(decompBuf, 0, sizeof(decompBuf));
    uLong sourceLen = (uLong)sizeof(input);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);
    memset(comp, 0, (size_t)cb);

    // step 2: Configure (initialize deflate and prepare output)
    deflateInit_(&src, 6, zlibVersion(), (int)sizeof(z_stream));
    src.next_in = (Bytef *)input;
    src.avail_in = (uInt)sourceLen;
    src.next_out = (Bytef *)comp;
    src.avail_out = (uInt)cb;
    deflate(&src, 0);
    unsigned int comp_len = (unsigned int)src.total_out;

    // step 3: Operate & Validate (copy deflate state, inflateBack to decompress, write via gzputs)
    deflateCopy(&dst, &src);
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&infl, 15, window, zlibVersion(), (int)sizeof(z_stream));
    struct InDesc { unsigned char *ptr; unsigned int len; };
    InDesc in_desc = { comp, comp_len };
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
    inflateBack(&infl, (in_func)in_fn, (void *)&in_desc, (out_func)out_fn, (void *)decompBuf);
    decompBuf[(comp_len < sizeof(decompBuf) ? comp_len : (unsigned int)sizeof(decompBuf) - 1u)] = 0;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, (const char *)decompBuf);

    // step 4: Cleanup
    inflateBackEnd(&infl);
    deflateEnd(&src);
    deflateEnd(&dst);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}