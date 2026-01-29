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
//<ID> 985
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and Initialize (compression stream)
    const char payload[] = "zlib API sequence payload exercising deflateCopy, inflateInit_, gzclose_r, inflate and deflateEnd";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;

    z_stream dstrm_src;
    memset(&dstrm_src, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm_src, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm_src.next_in = (Bytef *)src_buf;
    dstrm_src.avail_in = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);

    uLong comp_bound = deflateBound(&dstrm_src, payload_len);
    Bytef *comp_buf = new Bytef[(comp_bound ? comp_bound : 1)];
    dstrm_src.next_out = comp_buf;
    dstrm_src.avail_out = (uInt)(comp_bound <= (uLong)~0u ? comp_bound : (uLong)~0u);

    int rc_deflate = deflate(&dstrm_src, 0);

    // step 2: Configure (copy deflate state)
    z_stream dstrm_copy;
    memset(&dstrm_copy, 0, (size_t)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&dstrm_copy, &dstrm_src);

    // step 3: Operate (inflate the compressed output and perform gz file read/write)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    uLong comp_size = dstrm_src.total_out;
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size <= (uLong)~0u ? comp_size : (uLong)~0u);
    Bytef *uncomp_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = uncomp_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate = inflate(&istrm, 0);

    gzFile gf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf_w, (voidpc)src_buf, (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u));
    int rc_gzclose_w = gzclose(gf_w);
    gzFile gf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int rc_gzclose_r = gzclose_r(gf_r);

    // step 4: Validate and Cleanup
    int rc_deflate_end_copy = deflateEnd(&dstrm_copy);
    int rc_deflate_end_src = deflateEnd(&dstrm_src);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] comp_buf;
    delete [] uncomp_buf;

    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_copy;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzclose_r;
    (void)rc_deflate_end_copy;
    (void)rc_deflate_end_src;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}