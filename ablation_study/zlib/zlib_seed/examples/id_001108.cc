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
//<ID> 1108
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for deflateBound + inflateInit_ + inflateCopy";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    z_stream copystrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&copystrm, 0, (size_t)sizeof(z_stream));
    uInt out_buf_len = (uInt)(payload_len + 64);
    Bytef *out_buf = new Bytef[(out_buf_len ? (size_t)out_buf_len : 1)];

    // step 2: Setup (initialize deflate and inflate streams and compute bounds)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, payload_len);
    Bytef *comp_buf = new Bytef[(bound ? (size_t)bound : 1)];
    uLongf comp_len = (uLongf)bound;
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)out_buf_len;

    // step 3: Core operations (compress then prepare inflate and copy inflate stream)
    int rc_compress = compress2(comp_buf, &comp_len, (const Bytef *)payload, payload_len, 6);
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len;
    int rc_inflate_copy = inflateCopy(&copystrm, &istrm);
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Validate and cleanup
    uLong crc_input = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong crc_output = crc32(0UL, out_buf, (uInt)istrm.total_out);
    uLong flags = zlibCompileFlags();
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_inflate_end_copy = inflateEnd(&copystrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] out_buf;
    delete [] comp_buf;
    (void)version;
    (void)rc_deflate_init;
    (void)bound;
    (void)rc_inflate_init;
    (void)rc_compress;
    (void)rc_inflate_copy;
    (void)rc_inflate;
    (void)crc_input;
    (void)crc_output;
    (void)flags;
    (void)rc_inflate_end;
    (void)rc_inflate_end_copy;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}