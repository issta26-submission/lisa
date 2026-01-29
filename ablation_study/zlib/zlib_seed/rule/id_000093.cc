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
//<ID> 93
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
    z_stream istrm_copy;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&dstrm_copy, 0, sizeof(dstrm_copy));
    memset(&istrm, 0, sizeof(istrm));
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    unsigned char input[128];
    unsigned char decomp[128];
    memset(input, 'X', sizeof(input));
    memset(decomp, 0, sizeof(decomp));
    uLong sourceLen = (uLong)sizeof(input);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);
    memset(comp, 0, (size_t)cb);

    // step 2: Configure (deflate to produce compressed data, prepare inflateBack)
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)input;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = (Bytef *)comp;
    dstrm.avail_out = (uInt)cb;
    deflate(&dstrm, 0);
    unsigned int comp_len = (unsigned int)dstrm.total_out;
    deflateCopy(&dstrm_copy, &dstrm);
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&istrm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate & Validate (use inflateBack to decompress, then exercise inflateCopy/validate/undermine/backend)
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
    inflateBack(&istrm,
                (unsigned int(*)(void*, unsigned char**))in_fn,
                (void *)&in_desc,
                (int(*)(void*, unsigned char*, unsigned int))out_fn,
                (void *)decomp);
    inflateCopy(&istrm_copy, &istrm);
    inflateValidate(&istrm, 1);
    inflateUndermine(&istrm, 0);
    inflateBackEnd(&istrm);

    // step 4: Cleanup
    deflateEnd(&dstrm);
    deflateEnd(&dstrm_copy);
    inflateEnd(&istrm_copy);
    free(comp);

    // API sequence test completed successfully
    return 66;
}