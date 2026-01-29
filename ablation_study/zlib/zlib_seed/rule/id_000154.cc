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
//<ID> 154
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    unsigned char input[128];
    unsigned char output[256];
    memset(input, 'A', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);

    // step 2: Setup (deflate to produce compressed data; prepare inflater and descriptors)
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)bound;
    deflate(&def_strm, 0);

    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags;

    inflateInit2_(&inf_strm, 15, zlibVersion(), (int)sizeof(z_stream));

    struct InDesc { Bytef *ptr; uInt len; };
    InDesc in_desc;
    in_desc.ptr = compBuf;
    in_desc.len = (uInt)def_strm.total_out;

    struct OutDesc { Bytef *ptr; uInt left; };
    OutDesc out_desc;
    out_desc.ptr = (Bytef *)output;
    out_desc.left = (uInt)sizeof(output);

    // step 3: Core operations (use inflateBack with callbacks provided as non-capturing lambdas)
    in_func in_cb = (in_func)(+[](void *desc, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)desc;
        *buf = d->ptr;
        unsigned int n = d->len;
        d->ptr += n;
        d->len = 0;
        return n;
    });
    out_func out_cb = (out_func)(+[](void *desc, unsigned char *buf, unsigned int len) -> int {
        OutDesc *od = (OutDesc *)desc;
        memcpy(od->ptr, buf, (size_t)len);
        od->ptr += len;
        od->left -= len;
        return 0;
    });

    inflateBack(&inf_strm, in_cb, (void *)&in_desc, out_cb, (void *)&out_desc);

    // step 4: Validate & Cleanup
    uInt produced = (uInt)(sizeof(output) - out_desc.left);
    uLong checksum = adler32(0L, (const Bytef *)output, (uInt)produced);
    (void)checksum;
    deflateEnd(&def_strm);
    inflateBackEnd(&inf_strm);
    inflateEnd(&inf_strm);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}