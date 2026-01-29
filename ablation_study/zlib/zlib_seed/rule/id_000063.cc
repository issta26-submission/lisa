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
//<ID> 63
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream dstrm;
    z_stream dstrm_copy;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&dstrm_copy, 0, sizeof(dstrm_copy));
    memset(&istrm, 0, sizeof(istrm));
    unsigned char input[128];
    unsigned char decompBuf[1024];
    memset(input, 'A', sizeof(input));
    memset(decompBuf, 0, sizeof(decompBuf));
    uLong sourceLen = (uLong)sizeof(input);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);
    unsigned char *comp2 = (unsigned char *)malloc((size_t)cb);
    unsigned char *comp_all = (unsigned char *)malloc((size_t)(cb * 2));
    memset(comp, 0, (size_t)cb);
    memset(comp2, 0, (size_t)cb);
    memset(comp_all, 0, (size_t)(cb * 2));
    unsigned int comp_len = 0;
    unsigned int comp2_len = 0;
    unsigned int total_comp_len = 0;

    // step 2: Configure (compress with deflate)
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)input;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = (Bytef *)comp;
    dstrm.avail_out = (uInt)cb;
    deflate(&dstrm, 0);
    comp_len = (unsigned int)dstrm.total_out;

    // step 3: Operate (copy deflate state, produce additional compressed bytes, then inflateBack and write with gzputs)
    deflateCopy(&dstrm_copy, &dstrm);
    dstrm_copy.next_out = (Bytef *)comp2;
    dstrm_copy.avail_out = (uInt)cb;
    deflate(&dstrm_copy, 0);
    comp2_len = (unsigned int)(dstrm_copy.total_out - dstrm.total_out);
    memcpy(comp_all, comp, (size_t)comp_len);
    memcpy(comp_all + comp_len, comp2, (size_t)comp2_len);
    total_comp_len = comp_len + comp2_len;

    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&istrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    struct InDesc { unsigned char *ptr; unsigned int len; };
    struct OutDesc { unsigned char *dst; unsigned int off; };
    InDesc in_desc = { comp_all, total_comp_len };
    OutDesc out_desc = { decompBuf, 0 };
    auto in_fn = +[](void *desc, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)desc;
        *buf = d->ptr;
        unsigned int n = d->len;
        d->ptr += n;
        d->len = 0;
        return n;
    };
    auto out_fn = +[](void *out_desc_ptr, unsigned char *out_buf, unsigned int out_len) -> int {
        OutDesc *od = (OutDesc *)out_desc_ptr;
        memcpy(od->dst + od->off, out_buf, (size_t)out_len);
        od->off += out_len;
        return 0;
    };
    inflateBack(&istrm, (in_func)in_fn, (void *)&in_desc, (out_func)out_fn, (void *)&out_desc);
    unsigned int decomp_len = out_desc.off;
    if (decomp_len < sizeof(decompBuf)) {
        decompBuf[decomp_len] = 0;
    } else {
        decompBuf[sizeof(decompBuf)-1] = 0;
    }
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, (const char *)decompBuf);

    // step 4: Cleanup
    inflateBackEnd(&istrm);
    inflateEnd(&istrm);
    deflateEnd(&dstrm);
    deflateEnd(&dstrm_copy);
    gzclose(gz);
    free(comp);
    free(comp2);
    free(comp_all);

    // API sequence test completed successfully
    return 66;
}