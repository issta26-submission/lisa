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
//<ID> 1041
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial compression + decompression
    const char payload[] = "zlib API sequence payload exercising inflatePrime, inflateSyncPoint, compress2, uncompress, deflateEnd";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_comp = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);
    Bytef *uncomp_buf = new Bytef[(payload_len ? payload_len : 1)];
    uLongf uncomp_len = (uLongf)payload_len;
    int rc_uncomp = uncompress(uncomp_buf, &uncomp_len, comp_buf, (uLong)comp_len_f);

    // step 2: Setup inflate stream for operating on the compressed buffer
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len_f;
    Bytef *inflate_out = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = inflate_out;
    istrm.avail_out = (uInt)payload_len;

    // step 3: Operate: prime the bit buffer, run inflate, and check sync point
    int rc_inflate_prime = inflatePrime(&istrm, 3, 1);
    int rc_inflate = inflate(&istrm, 0);
    int rc_syncpoint = inflateSyncPoint(&istrm);

    // step 4: Configure a deflate stream (to exercise deflateEnd) and cleanup
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong bound_estimate = deflateBound(&dstrm, (uLong)istrm.total_out);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);

    delete [] comp_buf;
    delete [] uncomp_buf;
    delete [] inflate_out;

    (void)rc_comp;
    (void)rc_uncomp;
    (void)rc_inflate_init;
    (void)rc_inflate_prime;
    (void)rc_inflate;
    (void)rc_syncpoint;
    (void)rc_deflate_init;
    (void)bound_estimate;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}