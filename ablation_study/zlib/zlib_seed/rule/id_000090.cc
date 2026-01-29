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
//<ID> 90
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def_src;
    z_stream def_copy;
    z_stream inf_back;
    z_stream inf_copy;
    memset(&def_src, 0, sizeof(def_src));
    memset(&def_copy, 0, sizeof(def_copy));
    memset(&inf_back, 0, sizeof(inf_back));
    memset(&inf_copy, 0, sizeof(inf_copy));
    unsigned char input[128];
    unsigned char decomp_buf[128];
    memset(input, 'A', sizeof(input));
    memset(decomp_buf, 0, sizeof(decomp_buf));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);

    // step 2: Configure
    deflateInit_(&def_src, 6, zlibVersion(), (int)sizeof(z_stream));
    def_src.next_in = (Bytef *)input;
    def_src.avail_in = (uInt)inputLen;
    def_src.next_out = (Bytef *)comp;
    def_src.avail_out = (uInt)compBound;
    deflate(&def_src, 0);
    unsigned int comp_len = (unsigned int)def_src.total_out;
    deflateCopy(&def_copy, &def_src);

    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&inf_back, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateReset(&inf_back);
    inflateCopy(&inf_copy, &inf_back);

    // step 3: Operate & Validate
    inflateValidate(&inf_back, 1);
    inflateUndermine(&inf_back, 1);
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
    inflateBack(&inf_back, (in_func)in_fn, (void *)&in_desc, (out_func)out_fn, (void *)decomp_buf);
    inflateValidate(&inf_copy, 0);
    inflateUndermine(&inf_copy, 0);

    // step 4: Cleanup
    inflateBackEnd(&inf_back);
    inflateBackEnd(&inf_copy);
    deflateEnd(&def_src);
    deflateEnd(&def_copy);
    free(comp);

    // API sequence test completed successfully
    return 66;
}