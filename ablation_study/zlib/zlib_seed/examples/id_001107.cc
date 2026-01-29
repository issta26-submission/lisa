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
//<ID> 1107
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload: deflateBound + inflateInit_ + inflateCopy";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    z_stream istrm_copy;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm_copy, 0, (size_t)sizeof(z_stream));

    // step 2: Initialize streams
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Configure, operate and copy state
    uLong bound = deflateBound(&dstrm, payload_len);
    Bytef *comp_buf = new Bytef[(bound ? (size_t)bound : 1)];
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&dstrm, 4); // Z_FINISH == 4
    uLong comp_size = dstrm.total_out;
    size_t out_len = (size_t)payload_len + 64;
    Bytef *out_buf = new Bytef[(out_len ? out_len : 1)];
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)out_len;
    int rc_inflate = inflate(&istrm, 0); // Z_NO_FLUSH == 0
    int rc_inflate_copy = inflateCopy(&istrm_copy, &istrm);

    // step 4: Validate and cleanup
    uLong adler_in = adler32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong crc_comp = crc32(0UL, comp_buf, (uInt)(comp_size ? comp_size : 0));
    uLong flags = zlibCompileFlags();
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_inflate_copy_end = inflateEnd(&istrm_copy);
    delete [] comp_buf;
    delete [] out_buf;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)bound;
    (void)rc_deflate;
    (void)comp_size;
    (void)rc_inflate;
    (void)rc_inflate_copy;
    (void)adler_in;
    (void)crc_comp;
    (void)flags;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    (void)rc_inflate_copy_end;
    // API sequence test completed successfully
    return 66;
}