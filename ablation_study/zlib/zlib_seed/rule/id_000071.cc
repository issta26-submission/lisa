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
//<ID> 71
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong sourceLen = (uLong)sizeof(input);
    uLong bound = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    unsigned char decompBuf[512];
    memset(decompBuf, 0, sizeof(decompBuf));

    // step 2: Configure (compress source into comp buffer and init inflateBack)
    compress2(comp, &compLen, input, sourceLen, 6);
    unsigned int comp_len_ui = (unsigned int)compLen;
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&istrm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate (use inflateBack with provided in/out callbacks, then write decompressed data to gz)
    struct InDesc { unsigned char *ptr; unsigned int len; };
    InDesc in_desc = { comp, comp_len_ui };
    struct OutDesc { unsigned char *ptr; unsigned int pos; };
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
        OutDesc *d = (OutDesc *)out_desc;
        memcpy(d->ptr + d->pos, out_buf, (size_t)out_len);
        d->pos += out_len;
        return 0;
    };
    inflateBack(&istrm, (in_func)in_fn, (void *)&in_desc, (out_func)out_fn, (void *)&out_desc);
    inflateReset(&istrm);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)decompBuf, (unsigned int)out_desc.pos);
    gzclose(gz);

    // step 4: Cleanup
    inflateBackEnd(&istrm);
    free(comp);

    // API sequence test completed successfully
    return 66;
}