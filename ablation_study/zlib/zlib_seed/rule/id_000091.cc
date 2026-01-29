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
//<ID> 91
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
    z_stream inf_src;
    z_stream inf_copy;
    memset(&def_src, 0, sizeof(def_src));
    memset(&def_copy, 0, sizeof(def_copy));
    memset(&inf_src, 0, sizeof(inf_src));
    memset(&inf_copy, 0, sizeof(inf_copy));
    unsigned char input[128];
    unsigned char decomp[128];
    memset(input, 'X', sizeof(input));
    memset(decomp, 0, sizeof(decomp));
    uLong inputLen = (uLong)sizeof(input);

    // step 2: Setup (deflate to create compressed data, prepare inflateBack)
    deflateInit_(&def_src, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_src, inputLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    def_src.next_in = (Bytef *)input;
    def_src.avail_in = (uInt)inputLen;
    def_src.next_out = (Bytef *)comp;
    def_src.avail_out = (uInt)bound;
    deflate(&def_src, 0);
    unsigned int comp_len = (unsigned int)def_src.total_out;
    deflateCopy(&def_copy, &def_src);
    deflateEnd(&def_src);
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&inf_src, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateReset(&inf_src);

    // step 3: Operate & Validate (use inflateBack to decompress, then copy & validate)
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
    inflateBack(&inf_src, (in_func)in_fn, (void *)&in_desc, (out_func)out_fn, (void *)decomp);
    inflateCopy(&inf_copy, &inf_src);
    inflateValidate(&inf_copy, 1);
    inflateUndermine(&inf_copy, 1);
    inflateBackEnd(&inf_copy);

    // step 4: Cleanup
    deflateEnd(&def_copy);
    inflateBackEnd(&inf_src);
    free(comp);

    // API sequence test completed successfully
    return 66;
}