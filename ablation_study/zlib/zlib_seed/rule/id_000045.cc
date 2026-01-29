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
//<ID> 45
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
    z_stream istrm_back;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    memset(&istrm_back, 0, sizeof(istrm_back));
    unsigned char input[128];
    unsigned char decompBuf[256];
    memset(input, 'A', sizeof(input));
    memset(decompBuf, 0, sizeof(decompBuf));
    uLong inputLen = (uLong)sizeof(input);
    uLong compCap = compressBound(inputLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)compCap);
    memset(comp, 0, (size_t)compCap);

    // step 2: Configure
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)inputLen;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)compCap;
    deflate(&dstrm, 0);
    unsigned int compLen = (unsigned int)dstrm.total_out;
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateInit2_(&istrm, 15, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)sizeof(decompBuf);
    inflateBackInit_(&istrm_back, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate & Validate
    struct InDesc { unsigned char *ptr; unsigned int len; };
    InDesc in_desc = { comp, compLen };
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
    inflateBack(&istrm_back, (in_func)in_fn, (void *)&in_desc, (out_func)out_fn, (void *)decompBuf);
    int sync_res = inflateSync(&istrm);
    (void)sync_res;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)decompBuf[0]);

    // step 4: Cleanup
    inflateBackEnd(&istrm_back);
    inflateEnd(&istrm);
    deflateEnd(&dstrm);
    gzclose(gz);
    free(comp);
    // API sequence test completed successfully
    return 66;
}