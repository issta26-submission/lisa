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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    z_stream src;
    z_stream dst;
    z_stream defstrm;
    memset(&src, 0, sizeof(src));
    memset(&dst, 0, sizeof(dst));
    memset(&defstrm, 0, sizeof(defstrm));
    unsigned char input_buf[32];
    unsigned char output_buf[128];
    memset(input_buf, 'A', sizeof(input_buf));
    memset(output_buf, 0, sizeof(output_buf));
    inflateInit_(&src, zlibVersion(), (int)sizeof(z_stream));
    deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    src.next_in = input_buf;
    src.avail_in = (uInt)sizeof(input_buf);
    struct InDesc { unsigned char *ptr; unsigned int len; } in_desc;
    struct OutDesc { unsigned char *outptr; unsigned int pos; unsigned int cap; } out_desc;
    in_desc.ptr = input_buf;
    in_desc.len = (unsigned int)sizeof(input_buf);
    out_desc.outptr = output_buf;
    out_desc.pos = 0;
    out_desc.cap = (unsigned int)sizeof(output_buf);
    inflateCopy(&dst, &src);

    // step 3: Operate
    in_func in_cb = +[](void *d, unsigned char **buf)->unsigned int {
        InDesc *dd = (InDesc*)d;
        *buf = dd->ptr;
        unsigned int n = dd->len;
        dd->ptr += n;
        dd->len = 0;
        return n;
    };
    out_func out_cb = +[](void *d, unsigned char *buf, unsigned int len)->int {
        OutDesc *od = (OutDesc*)d;
        memcpy(od->outptr + od->pos, buf, len);
        od->pos += len;
        return (int)len;
    };
    inflateBack(&dst, in_cb, &in_desc, out_cb, &out_desc);
    inflateBackEnd(&dst);
    deflateResetKeep(&defstrm);

    // step 4: Validate & Cleanup
    gzFile gf = gzopen("/dev/null", "wb");
    gzclearerr(gf);
    gzclose(gf);
    inflateEnd(&src);
    inflateEnd(&dst);
    deflateEnd(&defstrm);

    // API sequence test completed successfully
    return 66;
}