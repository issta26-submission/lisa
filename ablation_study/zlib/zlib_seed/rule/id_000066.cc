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
//<ID> 66
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream strm_src;
    z_stream strm_copy;
    z_stream strm_inflate;
    memset(&strm_src, 0, sizeof(strm_src));
    memset(&strm_copy, 0, sizeof(strm_copy));
    memset(&strm_inflate, 0, sizeof(strm_inflate));
    unsigned char input_buf[128];
    unsigned char decomp_buf[512];
    memset(input_buf, 'A', sizeof(input_buf));
    memset(decomp_buf, 0, sizeof(decomp_buf));
    uLong sourceLen = (uLong)sizeof(input_buf);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);
    memset(comp, 0, (size_t)cb);
    unsigned int comp_len = 0;
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    gzFile gz = 0;

    // step 2: Configure (initialize deflate and perform compression)
    deflateInit_(&strm_src, 6, zlibVersion(), (int)sizeof(z_stream));
    strm_src.next_in = (Bytef *)input_buf;
    strm_src.avail_in = (uInt)sourceLen;
    strm_src.next_out = (Bytef *)comp;
    strm_src.avail_out = (uInt)cb;
    deflate(&strm_src, 0);
    comp_len = (unsigned int)strm_src.total_out;

    // step 3: Operate (copy deflate state, inflateBack to decompress, write with gzputs)
    deflateCopy(&strm_copy, &strm_src);
    inflateBackInit_(&strm_inflate, 15, window, zlibVersion(), (int)sizeof(z_stream));
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
    inflateBack(&strm_inflate, (in_func)in_fn, (void *)&in_desc, (out_func)out_fn, (void *)decomp_buf);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, (const char *)decomp_buf);

    // step 4: Cleanup
    inflateBackEnd(&strm_inflate);
    inflateEnd(&strm_inflate);
    deflateEnd(&strm_src);
    deflateEnd(&strm_copy);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}