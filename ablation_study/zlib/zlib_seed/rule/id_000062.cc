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
//<ID> 62
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream dstrm;
    z_stream dstrm_copy;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&dstrm_copy, 0, sizeof(dstrm_copy));
    memset(&istrm, 0, sizeof(istrm));
    unsigned char input[128];
    unsigned char decompBuf[512];
    unsigned char window[32768];
    memset(input, 'A', sizeof(input));
    memset(decompBuf, 0, sizeof(decompBuf));
    memset(window, 0, sizeof(window));
    uLong sourceLen = (uLong)sizeof(input);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);
    memset(comp, 0, (size_t)cb);
    gzFile gz = 0;

    // step 2: Configure (initialize deflate and prepare buffers)
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)input;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = (Bytef *)comp;
    dstrm.avail_out = (uInt)cb;
    deflate(&dstrm, 0);
    unsigned int comp_len = (unsigned int)dstrm.total_out;
    memset(&dstrm_copy, 0, sizeof(dstrm_copy));
    deflateCopy(&dstrm_copy, &dstrm);

    // step 3: Operate & Validate (use inflateBack to decompress then write with gzputs)
    inflateBackInit_(&istrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    struct InDesc { unsigned char *ptr; unsigned int len; };
    struct OutDesc { unsigned char *buf; unsigned int pos; };
    InDesc in_desc = { comp, comp_len };
    OutDesc out_desc = { decompBuf, 0 };
    auto in_fn = +[](void *desc, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)desc;
        *buf = d->ptr;
        unsigned int n = d->len;
        d->ptr += n;
        d->len = 0;
        return n;
    };
    auto out_fn = +[](void *out_desc, unsigned char *out_buf, unsigned int out_len) -> int {
        OutDesc *d = (OutDesc *)out_desc;
        memcpy(d->buf + d->pos, out_buf, (size_t)out_len);
        d->pos += out_len;
        return 0;
    };
    inflateBack(&istrm, (in_func)in_fn, (void *)&in_desc, (out_func)out_fn, (void *)&out_desc);
    out_desc.buf[out_desc.pos < sizeof(decompBuf) ? out_desc.pos : (sizeof(decompBuf) - 1)] = 0;
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, (const char *)out_desc.buf);

    // step 4: Cleanup
    inflateBackEnd(&istrm);
    inflateEnd(&istrm);
    deflateEnd(&dstrm);
    deflateEnd(&dstrm_copy);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}