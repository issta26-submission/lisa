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
//<ID> 39
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
    z_stream istrm_copy;
    unsigned char input[128];
    unsigned char decompBuf[512];
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    memset(input, 'A', sizeof(input));
    memset(decompBuf, 0, sizeof(decompBuf));
    uLong sourceLen = (uLong)sizeof(input);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);
    memset(comp, 0, (size_t)cb);
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)cb;
    deflate(&dstrm, 0);
    deflateResetKeep(&dstrm);
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&istrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)sizeof(decompBuf);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)dstrm.total_out;

    // step 3: Operate & Validate
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
    inflateCopy(&istrm_copy, &istrm);
    inflateBack(&istrm_copy, (in_func)in_fn, (void *)&in_desc, (out_func)out_fn, (void *)decompBuf);

    // step 4: Cleanup
    inflateBackEnd(&istrm_copy);
    inflateEnd(&istrm);
    deflateEnd(&dstrm);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, decompBuf, (unsigned int)sizeof(decompBuf));
    gzflush(gz, 0);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}