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
//<ID> 986
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and Initialization
    const char payload[] = "zlib API sequence payload exercising deflateCopy, inflateInit_, gzclose_r, inflate and deflateEnd";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong comp_bound = compressBound(payload_len);
    Bytef *comp_buf = new Bytef[(comp_bound ? comp_bound : 1)];
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    z_stream dstrm;
    z_stream dstrm_copy;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&dstrm_copy, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure - prepare and run a deflate operation, then copy the deflate stream
    dstrm.next_in = (Bytef *)src_buf;
    dstrm.avail_in = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(comp_bound <= (uLong)~0u ? comp_bound : (uLong)~0u);
    int rc_deflate = deflate(&dstrm, 0);
    int rc_deflate_copy = deflateCopy(&dstrm_copy, &dstrm);

    // step 3: Operate - use the produced compressed data to inflate, and exercise gzclose_r on a gzFile
    uLong comp_size = dstrm.total_out;
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size <= (uLong)~0u ? comp_size : (uLong)~0u);
    istrm.next_out = uncmp_buf;
    istrm.avail_out = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate = inflate(&istrm, 0);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzclose_r = gzclose_r(gf);

    // step 4: Cleanup
    int rc_defend_copy = deflateEnd(&dstrm_copy);
    int rc_defend = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] comp_buf;
    delete [] uncmp_buf;
    (void)rc_def_init;
    (void)rc_inflate_init;
    (void)rc_deflate;
    (void)rc_deflate_copy;
    (void)comp_size;
    (void)rc_inflate;
    (void)rc_gzclose_r;
    (void)rc_defend_copy;
    (void)rc_defend;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}