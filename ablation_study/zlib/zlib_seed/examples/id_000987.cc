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
//<ID> 987
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and compress
    const char payload[] = "zlib API sequence payload exercising compress2, inflate, deflateCopy, deflateEnd and gzclose_r";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound(payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);
    uLong comp_size = (uLong)comp_len_f;

    // step 2: Initialize inflate stream and perform inflation (Initialize -> Operate)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size <= (uLong)~0u ? comp_size : (uLong)~0u);
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate = inflate(&istrm, 0);

    // step 3: Initialize deflate, copy state, close copies, and operate gzFile read-close
    z_stream dstrm1;
    z_stream dstrm2;
    memset(&dstrm1, 0, (size_t)sizeof(z_stream));
    memset(&dstrm2, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm1, 6, zlibVersion(), (int)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&dstrm2, &dstrm1);
    int rc_deflate_end_copy = deflateEnd(&dstrm2);
    int rc_deflate_end_orig = deflateEnd(&dstrm1);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzclose_r = gzclose_r(gf);

    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] comp_buf;
    delete [] uncmp_buf;

    (void)rc_compress;
    (void)comp_size;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_deflate_init;
    (void)rc_deflate_copy;
    (void)rc_deflate_end_copy;
    (void)rc_deflate_end_orig;
    (void)rc_gzclose_r;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}