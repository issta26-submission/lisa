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
//<ID> 980
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
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_comp = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);
    uLong comp_size = (uLong)comp_len_f;

    // step 2: Initialize streams (inflate init and deflate init) and copy deflate state
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size <= (uLong)~0u ? comp_size : (uLong)~0u);
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);

    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    z_stream dstrm_copy;
    memset(&dstrm_copy, 0, (size_t)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&dstrm_copy, &dstrm);

    // step 3: Operate (inflate compressed data, write original payload to gz file, reopen and close for read)
    int rc_inflate = inflate(&istrm, 0);
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gw, (voidpc)src_buf, (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u));
    int rc_gzclose_w = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int rc_gzclose_r = gzclose_r(gr);

    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end1 = deflateEnd(&dstrm);
    int rc_deflate_end2 = deflateEnd(&dstrm_copy);
    delete [] comp_buf;
    delete [] uncmp_buf;

    (void)rc_comp;
    (void)rc_inflate_init;
    (void)rc_deflate_init;
    (void)rc_deflate_copy;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzclose_r;
    (void)rc_inflate_end;
    (void)rc_deflate_end1;
    (void)rc_deflate_end2;
    // API sequence test completed successfully
    return 66;
}