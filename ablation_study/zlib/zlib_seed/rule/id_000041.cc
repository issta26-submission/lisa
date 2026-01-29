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
//<ID> 41
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream dstrm;
    z_stream istrm;
    z_stream istrm2;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    memset(&istrm2, 0, sizeof(istrm2));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    unsigned char decomp_buf[256];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    uLong sourceLen = (uLong)sizeof(input);
    uLong compCap = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)compCap);
    memset(comp, 0, (size_t)compCap);
    unsigned char window[32768];
    memset(window, 0, sizeof(window));

    // step 2: Configure
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)compCap;
    deflate(&dstrm, 0);
    deflateEnd(&dstrm);

    inflateBackInit_(&istrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)dstrm.total_out;
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)sizeof(decomp_buf);

    inflateInit2_(&istrm2, 15, zlibVersion(), (int)sizeof(z_stream));
    istrm2.next_in = comp;
    istrm2.avail_in = (uInt)dstrm.total_out;

    // step 3: Operate & Validate
    typedef struct InDesc { unsigned char *ptr; unsigned int len; } InDesc;
    InDesc in_desc = { comp, (unsigned int)istrm.avail_in };
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
    inflateSync(&istrm2);
    inflateBack(&istrm, (in_func)in_fn, (void *)&in_desc, (out_func)out_fn, (void *)decomp_buf);

    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)decomp_buf[0]);

    // step 4: Cleanup
    inflateBackEnd(&istrm);
    inflateEnd(&istrm2);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}