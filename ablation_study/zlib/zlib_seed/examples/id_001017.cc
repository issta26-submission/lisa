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
//<ID> 1017
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and Initialize
    char payload[] = "zlib API sequence payload exercising deflate, inflate, gzwrite and inflateBackEnd";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    Bytef *src_buf = (Bytef *)payload;
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong out_bound = deflateBound(&dstrm, (uLong)payload_len);
    Bytef *comp_buf = new Bytef[(out_bound ? out_bound : 1)];
    dstrm.next_in = src_buf;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)out_bound;
    int rc_deflate = deflate(&dstrm, 0);
    // step 2: Cleanup deflate and prepare inflate
    int rc_deflate_end = deflateEnd(&dstrm);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)dstrm.total_out;
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)payload_len;
    // step 3: Operate - inflate and auxiliary inflateBack init/end, and write gz file
    int rc_inflate = inflate(&istrm, 0);
    z_stream bstrm;
    memset(&bstrm, 0, (size_t)sizeof(z_stream));
    unsigned char window[256];
    int rc_inflate_back_init = inflateBackInit_(&bstrm, 8, window, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_back_end = inflateBackEnd(&bstrm);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, (unsigned int)payload_len);
    int rc_gzclose = gzclose(gf);
    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] comp_buf;
    delete [] uncmp_buf;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_back_init;
    (void)rc_inflate_back_end;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}