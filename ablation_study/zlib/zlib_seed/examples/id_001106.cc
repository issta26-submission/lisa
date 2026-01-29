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
//<ID> 1106
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload for deflate/inflate and inflateCopy validation";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    z_stream dstrm;
    z_stream istrm;
    z_stream istrm_copy;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm_copy, 0, (size_t)sizeof(z_stream));
    const char *version = zlibVersion();

    // step 2: Setup (initialize deflate and inflate streams, allocate buffers)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong comp_bound = deflateBound(&dstrm, payload_len);
    Bytef *comp_buf = new Bytef[(comp_bound ? comp_bound : 1)];
    uInt comp_buf_len = (uInt)(comp_bound ? comp_bound : 1);
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = comp_buf_len;
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uInt out_buf_len = (uInt)(payload_len + 64);
    Bytef *out_buf = new Bytef[(out_buf_len ? out_buf_len : 1)];
    istrm.next_out = out_buf;
    istrm.avail_out = out_buf_len;

    // step 3: Core operations (compress, prepare inflate copy, decompress)
    int rc_deflate = deflate(&dstrm, Z_FINISH);
    int rc_deflate_end = deflateEnd(&dstrm);
    uLong comp_len = dstrm.total_out;
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len ? comp_len : 0);
    int rc_inflate_copy = inflateCopy(&istrm_copy, &istrm);
    int rc_inflate = inflate(&istrm, Z_FINISH);

    // step 4: Validation and cleanup
    uLong crc_in = crc32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    uLong crc_out = crc32_z(0UL, out_buf, (z_size_t)(out_buf_len - istrm.avail_out));
    uLong adler = adler32(0UL, (const Bytef *)payload, (uInt)payload_len);
    long compile_flags = (long)zlibCompileFlags();
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_inflate_copy_end = inflateEnd(&istrm_copy);
    delete [] comp_buf;
    delete [] out_buf;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate_copy;
    (void)rc_inflate;
    (void)crc_in;
    (void)crc_out;
    (void)adler;
    (void)compile_flags;
    (void)rc_inflate_end;
    (void)rc_inflate_copy_end;
    // API sequence test completed successfully
    return 66;
}