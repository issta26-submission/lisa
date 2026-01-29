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
//<ID> 981
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial compression (Initialize)
    const char payload[] = "zlib API sequence payload for deflateCopy, inflateInit_, gzclose_r, inflate and deflateEnd";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound(payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);
    uLong comp_size = (uLong)comp_len_f;

    // step 2: Initialize inflate stream and perform inflation (Configure -> Operate)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size <= (uLong)~0u ? comp_size : (uLong)~0u);
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate = inflate(&istrm, 0);

    // step 3: Initialize deflate stream, copy it, then end both deflate streams (Configure -> Operate -> Validate)
    z_stream dsrc;
    z_stream ddest;
    memset(&dsrc, 0, (size_t)sizeof(z_stream));
    memset(&ddest, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dsrc, 6, zlibVersion(), (int)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&ddest, &dsrc);
    int rc_deflate_end_dest = deflateEnd(&ddest);
    int rc_deflate_end_src = deflateEnd(&dsrc);

    // step 4: Write gz file, reopen for read and close with gzclose_r, then final cleanup (Operate -> Cleanup)
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzwrite = gzwrite(gw, (voidpc)src_buf, (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u));
    int rc_gzclose_w = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int rc_gzgetc = gzgetc(gr);
    int rc_gzclose_r = gzclose_r(gr);

    int rc_inflate_end = inflateEnd(&istrm);
    delete [] comp_buf;
    delete [] uncmp_buf;

    (void)rc_compress;
    (void)comp_size;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_deflate_init;
    (void)rc_deflate_copy;
    (void)rc_deflate_end_dest;
    (void)rc_deflate_end_src;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzgetc;
    (void)rc_gzclose_r;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}