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
//<ID> 44
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream istrm;
    z_stream back_istrm;
    memset(&istrm, 0, sizeof(istrm));
    memset(&back_istrm, 0, sizeof(back_istrm));
    unsigned char plain[64];
    memset(plain, 'A', sizeof(plain));
    uLong sourceLen = (uLong)sizeof(plain);
    uLong compBound = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    uLong compLen = compBound;
    compress2(comp, (uLongf *)&compLen, plain, sourceLen, 6);

    // step 2: Configure
    unsigned char decomp1[128];
    unsigned char decomp2[128];
    memset(decomp1, 0, sizeof(decomp1));
    memset(decomp2, 0, sizeof(decomp2));
    inflateInit2_(&istrm, 15, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = decomp1;
    istrm.avail_out = (uInt)sizeof(decomp1);
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&back_istrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    struct InDesc { unsigned char *ptr; unsigned int len; };
    InDesc in_desc = { comp, (unsigned int)compLen };
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

    // step 3: Operate & Validate
    inflateBack(&back_istrm, (in_func)in_fn, (void *)&in_desc, (out_func)out_fn, (void *)decomp2);
    inflate(&istrm, 0);
    inflateSync(&istrm);

    // step 4: Cleanup
    inflateBackEnd(&back_istrm);
    inflateEnd(&istrm);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)decomp2[0]);
    gzputc(gz, (int)decomp2[1]);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}