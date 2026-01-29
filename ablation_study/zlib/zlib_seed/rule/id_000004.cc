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
//<ID> 4
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1
    z_stream src;
    z_stream dst;
    memset(&src, 0, sizeof(src));
    memset(&dst, 0, sizeof(dst));
    unsigned char inbuf[64];
    unsigned char outbuf[128];
    memset(inbuf, 0xA5, sizeof(inbuf));
    memset(outbuf, 0, sizeof(outbuf));
    src.next_in = inbuf;
    src.avail_in = (uInt)sizeof(inbuf);
    src.next_out = outbuf;
    src.avail_out = (uInt)sizeof(outbuf);
    int r_init = inflateInit_(&src, zlibVersion(), (int)sizeof(z_stream));
    // step 2
    int r_copy = inflateCopy(&dst, &src);
    int r_resetkeep = deflateResetKeep(&dst);
    // step 3
    int r_inflateback = inflateBack(&dst, (in_func)0, (void *)0, (out_func)0, (void *)0);
    int r_inflatebackend = inflateBackEnd(&dst);
    // step 4
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    int wrote = 0;
    wrote = gzwrite(gf, outbuf, 1);
    gzclearerr(gf);
    int r_end_src = inflateEnd(&src);
    int r_end_dst = inflateEnd(&dst);
    gzclose(gf);
    // API sequence test completed successfully
    (void)r_init; (void)r_copy; (void)r_resetkeep; (void)r_inflateback; (void)r_inflatebackend;
    (void)r_end_src; (void)r_end_dst; (void)wrote;
    return 66;
}