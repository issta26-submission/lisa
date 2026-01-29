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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream istrm;
    z_stream bstrm;
    memset(&istrm, 0, sizeof(istrm));
    memset(&bstrm, 0, sizeof(bstrm));
    unsigned char input[64];
    memset(input, 'A', sizeof(input));
    uLong sourceLen = (uLong)sizeof(input);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);
    memset(comp, 0, (size_t)cb);
    uLongf comp_len = (uLongf)cb;
    compress2(comp, &comp_len, input, sourceLen, 6);
    unsigned char *decompBuf = (unsigned char *)malloc(512);
    memset(decompBuf, 0, 512);

    // step 2: Setup
    inflateInit2_(&istrm, 15, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = (Bytef *)comp;
    istrm.avail_in = (uInt)comp_len;
    inflateSync(&istrm);
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&bstrm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Core operations & Validate
    struct InDesc { unsigned char *ptr; unsigned int len; };
    InDesc in_desc = { comp, (unsigned int)comp_len };
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
    inflateBack(&bstrm, (in_func)in_fn, (void *)&in_desc, (out_func)out_fn, (void *)decompBuf);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)decompBuf[0]);

    // step 4: Cleanup
    inflateBackEnd(&bstrm);
    inflateEnd(&istrm);
    gzclose(gz);
    free(comp);
    free(decompBuf);

    // API sequence test completed successfully
    return 66;
}