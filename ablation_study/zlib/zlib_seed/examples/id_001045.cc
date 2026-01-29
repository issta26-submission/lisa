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
//<ID> 1045
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and Initialization (prepare payload, compress)
    const char payload[] = "zlib sequence payload for compress2 -> uncompress -> inflatePrime -> inflateSyncPoint -> deflateEnd";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_comp = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);

    // step 2: Operate — Decompress via uncompress and compute checksum for validation
    Bytef *uncomp_buf = new Bytef[(payload_len ? payload_len : 1)];
    uLongf uncomp_len_f = (uLongf)payload_len;
    int rc_uncomp = uncompress(uncomp_buf, &uncomp_len_f, comp_buf, (uLong)comp_len_f);
    uLong crc_orig = crc32(0L, src_buf, (uInt)payload_len);
    uLong crc_uncomp = crc32(0L, uncomp_buf, (uInt)uncomp_len_f);

    // step 3: Configure and use inflate APIs (initialize, prime, sync point, inflate)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len_f;
    Bytef *uncomp_buf2 = new Bytef[(payload_len ? payload_len : 1)];
    istrm.next_out = uncomp_buf2;
    istrm.avail_out = (uInt)payload_len;
    int rc_inflate_prime = inflatePrime(&istrm, 0, 0);
    int rc_inflate_syncpt = inflateSyncPoint(&istrm);
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);
    uLong crc_uncomp2 = crc32(0L, uncomp_buf2, (uInt)istrm.total_out);

    // step 4: Cleanup and finalization (create deflate stream then end it, free buffers)
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] comp_buf;
    delete [] uncomp_buf;
    delete [] uncomp_buf2;

    (void)rc_comp;
    (void)rc_uncomp;
    (void)crc_orig;
    (void)crc_uncomp;
    (void)rc_inflate_init;
    (void)rc_inflate_prime;
    (void)rc_inflate_syncpt;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)crc_uncomp2;
    (void)rc_deflate_init;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}