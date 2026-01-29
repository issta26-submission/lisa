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
//<ID> 61
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream src;
    z_stream copy_stream;
    z_stream back_stream;
    memset(&src, 0, sizeof(src));
    memset(&copy_stream, 0, sizeof(copy_stream));
    memset(&back_stream, 0, sizeof(back_stream));
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

    // step 2: Configure (initialize deflate and prepare streams)
    deflateInit_(&src, 6, zlibVersion(), (int)sizeof(z_stream));
    src.next_in = (Bytef *)input;
    src.avail_in = (uInt)sourceLen;
    src.next_out = (Bytef *)comp;
    src.avail_out = (uInt)cb;
    deflate(&src, 0);
    comp_len = (unsigned int)src.total_out;
    memset(&copy_stream, 0, sizeof(copy_stream));
    deflateCopy(&copy_stream, &src);
    copy_stream.next_out = (Bytef *)(comp + comp_len);
    copy_stream.avail_out = (uInt)(cb - comp_len);

    // step 3: Operate & Validate (continue deflate on copy, inflateBack, write with gzputs)
    deflate(&copy_stream, 0);
    inflateBackInit_(&back_stream, 15, window, zlibVersion(), (int)sizeof(z_stream));
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
    inflateBack(&back_stream, (in_func)in_fn, (void *)&in_desc, (out_func)out_fn, (void *)decompBuf);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, (const char *)decompBuf);

    // step 4: Cleanup
    inflateBackEnd(&back_stream);
    inflateEnd(&back_stream);
    deflateEnd(&src);
    deflateEnd(&copy_stream);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}