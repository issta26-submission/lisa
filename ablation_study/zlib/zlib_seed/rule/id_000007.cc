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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialization
    z_stream src;
    z_stream dst;
    z_stream defstrm;
    memset(&src, 0, sizeof(z_stream));
    memset(&dst, 0, sizeof(z_stream));
    memset(&defstrm, 0, sizeof(z_stream));
    int rc_src_init = inflateInit_(&src, zlibVersion(), sizeof(z_stream));
    int rc_def_init = deflateInit_(&defstrm, 6, zlibVersion(), sizeof(z_stream));
    (void)rc_src_init;
    (void)rc_def_init;

    // step 2: Configuration (copy inflate state and prepare buffers)
    int rc_inflate_copy = inflateCopy(&dst, &src);
    (void)rc_inflate_copy;
    unsigned char input_data[16];
    memset(input_data, 0, sizeof(input_data));
    src.next_in = input_data;
    src.avail_in = (uInt)sizeof(input_data);

    struct OutDesc { unsigned char *buf; unsigned int cap; unsigned int written; };
    unsigned char out_buffer[128];
    OutDesc out_desc;
    out_desc.buf = out_buffer;
    out_desc.cap = (unsigned int)sizeof(out_buffer);
    out_desc.written = 0;

    // step 3: Operate and validate (use inflateBack with callbacks, then clean inflateBack)
    in_func in_cb = +[](void *in_desc, unsigned char **buf) -> unsigned int {
        z_stream *s = (z_stream *)in_desc;
        *buf = s->next_in;
        unsigned int n = s->avail_in;
        s->avail_in = 0;
        return n;
    };
    out_func out_cb = +[](void *out_desc, unsigned char *buf, unsigned int len) -> int {
        OutDesc *od = (OutDesc *)out_desc;
        unsigned int avail = od->cap > od->written ? od->cap - od->written : 0;
        unsigned int tocopy = len <= avail ? len : avail;
        if (tocopy) {
            memcpy(od->buf + od->written, buf, tocopy);
            od->written += tocopy;
        }
        return 0;
    };
    int rc_inflate_back = inflateBack(&dst, in_cb, &src, out_cb, &out_desc);
    (void)rc_inflate_back;
    int rc_inflate_back_end = inflateBackEnd(&dst);
    (void)rc_inflate_back_end;

    // step 4: Cleanup (reset deflate keep, clear gz errors, and finish streams)
    int rc_def_reset_keep = deflateResetKeep(&defstrm);
    (void)rc_def_reset_keep;
    gzFile gz = gzopen("test_zlib_api_sequence.tmp", "wb");
    gzclearerr(gz);
    gzclose(gz);
    int rc_inflate_end_src = inflateEnd(&src);
    int rc_inflate_end_dst = inflateEnd(&dst);
    int rc_def_end = deflateEnd(&defstrm);
    (void)rc_inflate_end_src;
    (void)rc_inflate_end_dst;
    (void)rc_def_end;

    // API sequence test completed successfully
    return 66;
}