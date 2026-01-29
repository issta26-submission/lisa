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
//<ID> 1100
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialization
    const char payload[] = "zlib API sequence payload for inflateCopy and deflateBound usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    z_stream istrm_copy;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm_copy, 0, (size_t)sizeof(z_stream));

    // step 2: Setup and configuration
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    z_size_t out_buf_len = (z_size_t)(payload_len + 64);
    Bytef *out_buf = new Bytef[(out_buf_len ? out_buf_len : 1)];
    istrm.next_in = (Bytef *)payload;
    istrm.avail_in = (uInt)payload_len;
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)out_buf_len;

    // step 3: Core operations (inflate + copy + second inflate)
    int rc_inflate_first = inflate(&istrm, 0);
    int rc_inflate_copy_create = inflateCopy(&istrm_copy, &istrm);
    istrm_copy.next_out = out_buf;
    istrm_copy.avail_out = (uInt)out_buf_len;
    int rc_inflate_second = inflate(&istrm_copy, 0);

    // step 4: Validation and cleanup
    uLong adler_orig = adler32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong compile_flags = zlibCompileFlags();
    int rc_inflate_end1 = inflateEnd(&istrm);
    int rc_inflate_end2 = inflateEnd(&istrm_copy);
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] out_buf;
    delete [] comp_buf;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)bound;
    (void)rc_inflate_first;
    (void)rc_inflate_copy_create;
    (void)rc_inflate_second;
    (void)adler_orig;
    (void)compile_flags;
    (void)rc_inflate_end1;
    (void)rc_inflate_end2;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}