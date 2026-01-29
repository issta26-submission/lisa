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
//<ID> 352
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & compress source data with deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    unsigned char src[128];
    memset(src, 'A', sizeof(src));
    uLong src_len = (uLong)sizeof(src);
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, src_len);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    dstrm.next_in = (Bytef *)src;
    dstrm.avail_in = (uInt)src_len;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong compLen = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 2: Initialize inflateBack
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    inflateBackInit_(&istrm, 15, (unsigned char *)0, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Provide in/out callbacks and perform inflateBack, then call inflateUndermine
    struct BufDesc { const unsigned char * buf; unsigned int len; unsigned int pos; unsigned char * out; unsigned int out_len; unsigned int out_pos; };
    static BufDesc inDesc;
    static BufDesc outDesc;
    inDesc.buf = compBuf;
    inDesc.len = (unsigned int)compLen;
    inDesc.pos = 0;
    outDesc.out = (unsigned char *)malloc((size_t)src_len);
    memset(outDesc.out, 0, (size_t)src_len);
    outDesc.out_len = (unsigned int)src_len;
    outDesc.out_pos = 0;
    in_func in_cb = (in_func)+[](void * desc, unsigned char ** buf) -> unsigned int {
        BufDesc * d = (BufDesc *)desc;
        unsigned int ret = d->len - d->pos;
        *buf = (unsigned char *)(d->buf + d->pos);
        d->pos = d->len;
        return ret;
    };
    out_func out_cb = (out_func)+[](void * desc, unsigned char * buf, unsigned int len) -> int {
        BufDesc * d = (BufDesc *)desc;
        memcpy(d->out + d->out_pos, buf, (size_t)len);
        d->out_pos += len;
        return (int)len;
    };
    inflateBack(&istrm, in_cb, &inDesc, out_cb, &outDesc);
    inflateUndermine(&istrm, 1);
    inflateBackEnd(&istrm);

    // step 4: Write compressed data to gz file and Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compLen);
    gzclose(gz);
    free(compBuf);
    free(outDesc.out);
    return 66;
    // API sequence test completed successfully
}