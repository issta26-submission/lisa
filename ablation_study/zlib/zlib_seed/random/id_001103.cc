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
//<ID> 1103
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload: deflateBound + inflateInit_ + inflateCopy";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    z_stream istrm_copy;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm_copy, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize streams and query bounds)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];

    // step 3: Core operations (compress, inflate, copy state, checksums)
    uLongf comp_len = (uLongf)bound;
    int rc_compress = compress2(comp_buf, &comp_len, (const Bytef *)payload, payload_len, 6);
    Bytef *out_buf = new Bytef[((size_t)payload_len + 64) ? ((size_t)payload_len + 64) : 1];
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(payload_len + 64);
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_copy = inflateCopy(&istrm_copy, &istrm);
    uLong out_size = istrm.total_out;
    uLong crc_in = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong crc_out = crc32(0UL, out_buf, (uInt)out_size);
    uLong flags = zlibCompileFlags();

    // step 4: Validate and cleanup
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_inflate_copy_end = inflateEnd(&istrm_copy);
    delete [] comp_buf;
    delete [] out_buf;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)bound;
    (void)rc_compress;
    (void)rc_inflate;
    (void)rc_inflate_copy;
    (void)crc_in;
    (void)crc_out;
    (void)flags;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    (void)rc_inflate_copy_end;
    // API sequence test completed successfully
    return 66;
}