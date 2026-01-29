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
//<ID> 74
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream cstrm;
    z_stream istrm;
    memset(&cstrm, 0, sizeof(cstrm));
    memset(&istrm, 0, sizeof(istrm));
    unsigned char input[64];
    memset(input, 'Z', sizeof(input));
    uLong sourceLen = (uLong)sizeof(input);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);
    memset(comp, 0, (size_t)cb);
    unsigned char decomp[256];
    memset(decomp, 0, sizeof(decomp));

    // step 2: Configure (deflate/compress)
    deflateInit_(&cstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    cstrm.next_in = (Bytef *)input;
    cstrm.avail_in = (uInt)sourceLen;
    cstrm.next_out = (Bytef *)comp;
    cstrm.avail_out = (uInt)cb;
    deflate(&cstrm, 0);
    unsigned int comp_len = (unsigned int)cstrm.total_out;

    // step 3: Operate (inflateBack to decompress, then write with gzwrite)
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&istrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateReset(&istrm);
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
    inflateBack(&istrm, (in_func)in_fn, (void *)&in_desc, (out_func)out_fn, (void *)decomp);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)decomp, (unsigned int)sourceLen);

    // step 4: Cleanup
    inflateBackEnd(&istrm);
    deflateEnd(&cstrm);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}