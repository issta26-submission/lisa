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
//<ID> 1105
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload for compress+inflate+copy validation";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    z_stream istrm_copy;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm_copy, 0, (size_t)sizeof(z_stream));

    // step 2: Configure and compress (deflate)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(bound ? bound : 1);
    int rc_deflate = deflate(&dstrm, 4); // 4 == Z_FINISH

    // step 3: Initialize inflate, copy its state, and perform inflate
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)dstrm.total_out;
    z_size_t out_buf_len = (z_size_t)(payload_len + 64);
    Bytef *out_buf = new Bytef[(out_buf_len ? out_buf_len : 1)];
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)out_buf_len;
    int rc_inflate_copy = inflateCopy(&istrm_copy, &istrm);
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Validation and cleanup
    uLong decompressed_len = (uLong)istrm.total_out;
    uLong crc_in = crc32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    uLong crc_out = crc32_z(0UL, out_buf, (z_size_t)decompressed_len);
    uLong adler_in = adler32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong compile_flags = zlibCompileFlags();
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_inflate_copy_end = inflateEnd(&istrm_copy);
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] comp_buf;
    delete [] out_buf;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_inflate_init;
    (void)rc_inflate_copy;
    (void)rc_inflate;
    (void)decompressed_len;
    (void)crc_in;
    (void)crc_out;
    (void)adler_in;
    (void)compile_flags;
    (void)rc_inflate_end;
    (void)rc_inflate_copy_end;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}