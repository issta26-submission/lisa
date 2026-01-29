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
//<ID> 356
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    unsigned char input_buf[128];
    memset(input_buf, 'A', sizeof(input_buf));
    uLong inputLen = (uLong)sizeof(input_buf);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)input_buf, inputLen, 6);

    // step 2: Setup inflateBack
    unsigned char * window = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window, 0, (size_t)(1 << 15));
    struct InDesc { const unsigned char * ptr; uLong len; };
    struct OutDesc { unsigned char * out; uLong out_sz; uLong pos; };
    InDesc in_desc;
    in_desc.ptr = compBuf;
    in_desc.len = compLen;
    OutDesc out_desc;
    out_desc.out = (unsigned char *)malloc((size_t)inputLen);
    memset(out_desc.out, 0, (size_t)inputLen);
    out_desc.out_sz = inputLen;
    out_desc.pos = 0;
    in_func inf = [](void * desc, unsigned char ** buf) -> unsigned int {
        InDesc * d = (InDesc *)desc;
        unsigned int n = (unsigned int)d->len;
        *buf = (unsigned char *)d->ptr;
        d->ptr += n;
        d->len -= n;
        return n;
    };
    out_func outf = [](void * desc, unsigned char * buf, unsigned int len) -> int {
        OutDesc * d = (OutDesc *)desc;
        memcpy(d->out + (size_t)d->pos, buf, (size_t)len);
        d->pos += (uLong)len;
        return 0;
    };
    inflateBackInit_(&strm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateUndermine(&strm, 1);

    // step 3: Operate (inflateBack) & Validate
    inflateBack(&strm, inf, &in_desc, outf, &out_desc);
    inflateBackEnd(&strm);
    int cmp = memcmp(out_desc.out, input_buf, (size_t)inputLen);
    (void)cmp;

    // step 4: Write compressed data to gz file and Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compLen);
    gzclose(gz);
    free(compBuf);
    free(window);
    free(out_desc.out);
    return 66;
    // API sequence test completed successfully
}