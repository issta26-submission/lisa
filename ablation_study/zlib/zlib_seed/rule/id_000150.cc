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
//<ID> 150
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def_strm;
    z_stream inf_init_strm;
    z_stream inf_back_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_init_strm, 0, sizeof(inf_init_strm));
    memset(&inf_back_strm, 0, sizeof(inf_back_strm));
    unsigned char input[128];
    unsigned char output[512];
    memset(input, 'A', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLong compLen = 0;
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags;

    // step 2: Setup (compress via deflate, initialize inflaters)
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)bound;
    deflate(&def_strm, 0);
    compLen = (uLong)def_strm.total_out;

    inflateInit2_(&inf_init_strm, 15, zlibVersion(), (int)sizeof(z_stream));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&inf_back_strm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate (use inflateBack with callbacks to decompress into output)
    struct InDesc { unsigned char *ptr; unsigned int left; };
    struct OutDesc { unsigned char *ptr; unsigned int cap; unsigned int used; };
    InDesc in_desc;
    in_desc.ptr = (unsigned char *)compBuf;
    in_desc.left = (unsigned int)compLen;
    OutDesc out_desc;
    out_desc.ptr = output;
    out_desc.cap = (unsigned int)sizeof(output);
    out_desc.used = 0;

    in_func in_cb = [](void *desc, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)desc;
        unsigned int n = d->left;
        *buf = d->ptr;
        d->ptr = d->ptr + n;
        d->left = 0;
        return n;
    };
    out_func out_cb = [](void *desc, unsigned char *data, unsigned int len) -> int {
        OutDesc *o = (OutDesc *)desc;
        memcpy(o->ptr + o->used, data, (size_t)len);
        o->used += len;
        return 0;
    };

    inf_back_strm.next_in = compBuf;
    inf_back_strm.avail_in = (uInt)compLen;
    inflateBack(&inf_back_strm, in_cb, &in_desc, out_cb, &out_desc);

    // step 4: Validate & Cleanup
    uLong checksum = adler32(0L, output, (uInt)out_desc.used);
    (void)checksum;
    inflateBackEnd(&inf_back_strm);
    inflateEnd(&inf_init_strm);
    deflateEnd(&def_strm);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}