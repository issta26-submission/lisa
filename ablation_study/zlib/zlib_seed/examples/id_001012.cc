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
//<ID> 1012
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and Initialize
    const char payload[] = "zlib API sequence payload exercising deflateEnd, gzwrite, inflate and inflateBackEnd";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong comp_bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(comp_bound ? comp_bound : 1)];
    uLongf comp_len_f = (uLongf)comp_bound;
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);
    z_stream istrm;
    z_stream bstrm;
    z_stream dstrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&bstrm, 0, (size_t)sizeof(z_stream));
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    unsigned char *back_window = new unsigned char[(1 << 15)];
    int rc_inflate_back_init = inflateBackInit_(&bstrm, 15, back_window, zlibVersion(), (int)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len_f;
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)payload_len;
    dstrm.next_in = (Bytef *)src_buf;
    dstrm.avail_in = (uInt)payload_len;
    uLong d_out_bound = deflateBound(&dstrm, (uLong)payload_len);
    Bytef *d_out_buf = new Bytef[(d_out_bound ? d_out_bound : 1)];
    dstrm.next_out = d_out_buf;
    dstrm.avail_out = (uInt)d_out_bound;

    // step 3: Operate
    int rc_deflate = deflate(&dstrm, 0);
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_back_end = inflateBackEnd(&bstrm);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, (unsigned int)payload_len);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate and Cleanup
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] comp_buf;
    delete [] uncmp_buf;
    delete [] d_out_buf;
    delete [] back_window;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate_back_init;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_inflate;
    (void)rc_inflate_back_end;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}