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
//<ID> 64
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defstrm;
    z_stream defcopy;
    z_stream istrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&defcopy, 0, sizeof(defcopy));
    memset(&istrm, 0, sizeof(istrm));
    unsigned char input[128];
    unsigned char decompBuf[1024];
    unsigned char window[32768];
    memset(input, 'A', sizeof(input));
    memset(decompBuf, 0, sizeof(decompBuf));
    memset(window, 0, sizeof(window));
    uLong sourceLen = (uLong)sizeof(input);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);
    memset(comp, 0, (size_t)cb);
    gzFile gz = 0;

    // step 2: Configure (compress with deflate and copy the deflate state)
    deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)input;
    defstrm.avail_in = (uInt)sourceLen;
    defstrm.next_out = (Bytef *)comp;
    defstrm.avail_out = (uInt)cb;
    deflate(&defstrm, 0);
    deflateCopy(&defcopy, &defstrm);
    unsigned int comp_len = (unsigned int)defstrm.total_out;

    // step 3: Operate (use inflateBack to decompress using in/out callbacks, then write string with gzputs)
    struct InDesc { unsigned char *ptr; unsigned int len; };
    struct OutDesc { unsigned char *base; unsigned char *ptr; unsigned int cap; unsigned int written; };
    InDesc in_desc = { comp, comp_len };
    OutDesc out_desc = { decompBuf, decompBuf, (unsigned int)sizeof(decompBuf), 0 };
    inflateBackInit_(&istrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    auto in_fn = +[](void *desc, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)desc;
        *buf = d->ptr;
        unsigned int n = d->len;
        d->ptr += n;
        d->len = 0;
        return n;
    };
    auto out_fn = +[](void *out_desc, unsigned char *out_buf, unsigned int out_len) -> int {
        OutDesc *od = (OutDesc *)out_desc;
        unsigned int to_copy = out_len;
        if (to_copy > od->cap - od->written) {
            to_copy = od->cap - od->written;
        }
        memcpy(od->ptr, out_buf, (size_t)to_copy);
        od->ptr += to_copy;
        od->written += to_copy;
        return 0;
    };
    inflateBack(&istrm, (in_func)in_fn, (void *)&in_desc, (out_func)out_fn, (void *)&out_desc);
    if (out_desc.written < out_desc.cap) {
        out_desc.ptr[0] = 0;
    } else {
        out_desc.base[out_desc.cap - 1] = 0;
    }
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, (const char *)out_desc.base);

    // step 4: Cleanup
    inflateBackEnd(&istrm);
    deflateEnd(&defstrm);
    deflateEnd(&defcopy);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}