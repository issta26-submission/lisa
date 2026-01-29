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
//<ID> 46
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
    z_stream backstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    memset(&backstrm, 0, sizeof(backstrm));
    unsigned char input[128];
    memset(input, 'X', sizeof(input));
    uLong sourceLen = (uLong)sizeof(input);
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure (compress input -> prepare inflate back)
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)sourceLen;
    uLong bound = deflateBound(&dstrm, sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 0);

    unsigned char decompBuf[512];
    memset(decompBuf, 0, sizeof(decompBuf));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateInit2_(&istrm, 15, zlibVersion(), (int)sizeof(z_stream));
    inflateBackInit_(&backstrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    backstrm.next_out = decompBuf;
    backstrm.avail_out = (uInt)sizeof(decompBuf);
    backstrm.next_in = comp;
    backstrm.avail_in = (uInt)dstrm.total_out;

    // step 3: Operate & Validate
    struct InDesc { unsigned char *ptr; unsigned int len; };
    InDesc in_desc = { comp, (unsigned int)backstrm.avail_in };
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
    inflateBack(&backstrm, (in_func)in_fn, (void *)&in_desc, (out_func)out_fn, (void *)decompBuf);
    inflateSync(&istrm);

    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)decompBuf[0]);
    gzputc(gz, (int)decompBuf[1]);

    // step 4: Cleanup
    inflateBackEnd(&backstrm);
    inflateEnd(&istrm);
    deflateEnd(&dstrm);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}