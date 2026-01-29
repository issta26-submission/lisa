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
//<ID> 988
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial compression
    const char payload[] = "zlib API sequence payload exercising deflateCopy, inflateInit_, gzclose_r, inflate and deflateEnd";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? (size_t)bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);
    uLong comp_size = (uLong)comp_len_f;

    // step 2: Initialize streams and perform a deflate stream copy
    z_stream defl_src;
    z_stream defl_copy;
    z_stream infl;
    memset(&defl_src, 0, (size_t)sizeof(z_stream));
    memset(&defl_copy, 0, (size_t)sizeof(z_stream));
    memset(&infl, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&defl_src, 6, zlibVersion(), (int)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&defl_copy, &defl_src);
    int rc_deflate_end_src = deflateEnd(&defl_src);

    // step 3: Initialize inflate, perform inflate, and exercise gz read-close
    int rc_inflate_init = inflateInit_(&infl, zlibVersion(), (int)sizeof(z_stream));
    infl.next_in = comp_buf;
    infl.avail_in = (uInt)(comp_size <= (uLong)~0u ? comp_size : (uLong)~0u);
    Bytef *uncmp_buf = new Bytef[(payload_len ? (size_t)payload_len : 1)];
    infl.next_out = uncmp_buf;
    infl.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate = inflate(&infl, 0);

    gzFile gf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf_w, (voidpc)src_buf, (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u));
    int rc_gzclose_w = gzclose(gf_w);
    gzFile gf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int rc_gzclose_r = gzclose_r(gf_r);

    // step 4: Cleanup and validation
    int rc_deflate_end_copy = deflateEnd(&defl_copy);
    int rc_inflate_end = inflateEnd(&infl);
    delete [] comp_buf;
    delete [] uncmp_buf;

    (void)rc_compress;
    (void)comp_size;
    (void)rc_deflate_init;
    (void)rc_deflate_copy;
    (void)rc_deflate_end_src;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzclose_r;
    (void)rc_deflate_end_copy;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}