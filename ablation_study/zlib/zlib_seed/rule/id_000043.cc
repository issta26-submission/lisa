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
//<ID> 43
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
    uLong sourceLen = (uLong)sizeof(input);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);
    memset(comp, 0, (size_t)cb);
    unsigned char decompBuf[512];
    memset(decompBuf, 0, sizeof(decompBuf));
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)cb;
    deflate(&dstrm, 0);
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&istrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)dstrm.total_out;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)sizeof(decompBuf);

    // step 3: Operate & Validate
    struct InDesc { unsigned char *ptr; unsigned int len; };
    struct OutDesc { unsigned char *ptr; unsigned int written; unsigned int capacity; };
    InDesc in_desc = { comp, (unsigned int)istrm.avail_in };
    OutDesc out_desc = { decompBuf, 0, (unsigned int)sizeof(decompBuf) };
    auto in_fn = +[](void *desc, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)desc;
        *buf = d->ptr;
        unsigned int n = d->len;
        d->ptr += n;
        d->len = 0;
        return n;
    };
    auto out_fn = +[](void *out_desc, unsigned char *out_buf, unsigned int out_len) -> int {
        OutDesc *o = (OutDesc *)out_desc;
        unsigned int copy_n = out_len;
        if (o->written + copy_n > o->capacity) {
            copy_n = o->capacity - o->written;
        }
        memcpy(o->ptr + o->written, out_buf, (size_t)copy_n);
        o->written += copy_n;
        return 0;
    };
    inflateBack(&istrm, (in_func)in_fn, (void *)&in_desc, (out_func)out_fn, (void *)&out_desc);
    inflateSync(&istrm);
    inflateInit2_(&istrm2, 15, zlibVersion(), (int)sizeof(z_stream));
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)decompBuf[0]);

    // step 4: Cleanup
    inflateBackEnd(&istrm);
    inflateEnd(&istrm);
    inflateEnd(&istrm2);
    deflateEnd(&dstrm);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}