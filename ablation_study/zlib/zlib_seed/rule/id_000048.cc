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
//<ID> 48
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
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong sourceLen = (uLong)sizeof(input);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);
    memset(comp, 0, (size_t)cb);
    unsigned char decomp[512];
    memset(decomp, 0, sizeof(decomp));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    gzFile gz = 0;

    // step 2: Configure (compress with deflate)
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)cb;
    deflate(&dstrm, 0);
    deflateEnd(&dstrm);

    // step 3: Operate & Validate (initialize inflate, run inflateBack, sync)
    inflateInit2_(&istrm, 15, zlibVersion(), (int)sizeof(z_stream));
    inflateBackInit_(&istrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)dstrm.total_out;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)sizeof(decomp);
    struct InDesc { unsigned char *ptr; unsigned int len; };
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
    inflateBack(&istrm, (in_func)in_fn, (void *)&in_desc, (out_func)out_fn, (void *)decomp);
    inflateSync(&istrm);

    // step 4: Cleanup (write a small gzip file and free resources)
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, 'O');
    gzputc(gz, 'K');
    gzputc(gz, '\n');
    gzclose(gz);
    inflateBackEnd(&istrm);
    inflateEnd(&istrm);
    free(comp);

    // API sequence test completed successfully
    return 66;
}