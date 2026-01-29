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
//<ID> 983
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload exercising deflateCopy, inflateInit_, gzclose_r, inflate and deflateEnd";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    Bytef *decomp_buf = new Bytef[(payload_len ? payload_len : 1)];
    z_stream dstrm;
    z_stream dstrm_copy;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&dstrm_copy, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Initialize (Initialize)
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate and Validate (Configure -> Operate -> Validate)
    dstrm.next_in = (Bytef *)src_buf;
    dstrm.avail_in = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(bound <= (uLong)~0u ? bound : (uLong)~0u);
    int rc_deflate_finish = deflate(&dstrm, 4); /* 4 == Z_FINISH */
    uLong comp_size = dstrm.total_out;
    int rc_deflate_copy = deflateCopy(&dstrm_copy, &dstrm);

    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size <= (uLong)~0u ? comp_size : (uLong)~0u);
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate = inflate(&istrm, 0);

    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = 0;
    int rc_gzclose_w = 0;
    if (gw) {
        rc_gzwrite = gzwrite(gw, (voidpc)src_buf, (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u));
        rc_gzclose_w = gzclose(gw);
    }
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int rc_gzclose_r = gzclose_r(gr);

    // step 4: Cleanup
    int rc_deflate_end_src = deflateEnd(&dstrm);
    int rc_deflate_end_copy = deflateEnd(&dstrm_copy);
    int rc_inflate_end = 0;
    rc_inflate_end = inflateEnd(&istrm);
    delete [] comp_buf;
    delete [] decomp_buf;

    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)rc_deflate_finish;
    (void)rc_deflate_copy;
    (void)comp_size;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzclose_r;
    (void)rc_deflate_end_src;
    (void)rc_deflate_end_copy;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}