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
//<ID> 1011
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
    uLong bound_comp = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound_comp ? bound_comp : 1)];
    uLongf comp_len_f = (uLongf)bound_comp;
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);

    // step 2: Setup inflate and deflate streams
    z_stream istrm;
    z_stream dstrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate (inflate compressed buffer, perform a deflate, and write gz file)
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len_f;
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)payload_len;
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_back_end = inflateBackEnd(&istrm);

    uLong out_bound = deflateBound(&dstrm, (uLong)payload_len);
    Bytef *out_buf = new Bytef[(out_bound ? out_bound : 1)];
    dstrm.next_in = (Bytef *)src_buf;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = out_buf;
    dstrm.avail_out = (uInt)out_bound;
    int rc_deflate = deflate(&dstrm, 0);
    int rc_deflate_end = deflateEnd(&dstrm);

    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, (unsigned int)payload_len);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate and Cleanup
    delete [] comp_buf;
    delete [] uncmp_buf;
    delete [] out_buf;

    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_deflate_init;
    (void)rc_inflate;
    (void)rc_inflate_back_end;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}