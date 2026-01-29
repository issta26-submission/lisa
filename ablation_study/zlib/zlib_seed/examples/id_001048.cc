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
//<ID> 1048
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload exercising compress2, uncompress, inflatePrime, inflateSyncPoint, deflateEnd";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len = (uLongf)bound;
    int rc_comp = compress2(comp_buf, &comp_len, src_buf, (uLong)payload_len, 6);

    // step 2: Decompress (uncompress) into a destination buffer
    Bytef *uncomp_buf = new Bytef[(payload_len ? payload_len : 1)];
    uLongf uncomp_len = (uLongf)payload_len;
    int rc_uncomp = uncompress(uncomp_buf, &uncomp_len, comp_buf, (uLong)comp_len);

    // step 3: Initialize and configure inflate stream, then use inflatePrime and inflateSyncPoint
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len;
    Bytef *out_buf = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)payload_len;
    int rc_inflate_prime = inflatePrime(&istrm, 0, 0);
    int rc_inflate_syncpt = inflateSyncPoint(&istrm);

    // step 4: Initialize a deflate stream to exercise deflateEnd, then cleanup and validate
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] comp_buf;
    delete [] uncomp_buf;
    delete [] out_buf;

    (void)rc_comp;
    (void)rc_uncomp;
    (void)rc_inflate_init;
    (void)rc_inflate_prime;
    (void)rc_inflate_syncpt;
    (void)rc_deflate_init;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}