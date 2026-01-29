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
//<ID> 1207
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for deflateCopy/adler32_z/gzdopen/inflateInit_/gzeof usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dsrc;
    z_stream dcopy;
    z_stream istrm;
    memset(&dsrc, 0, (size_t)sizeof(z_stream));
    memset(&dcopy, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    Bytef *in_buf = new Byte[(size_t)payload_len ? (size_t)payload_len : 1];
    Bytef *out_buf = new Byte[256];

    // step 2: Setup (initialize deflate source and inflate stream)
    for (z_size_t i = 0; i < (z_size_t)payload_len; ++i) { in_buf[i] = (Byte)payload[i]; } // small initialization to populate input
    dsrc.next_in = in_buf;
    dsrc.avail_in = (uInt)payload_len;
    dsrc.next_out = out_buf;
    dsrc.avail_out = (uInt)256;
    int rc_deflate_init = deflateInit_(&dsrc, 6, version, (int)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&dcopy, &dsrc);
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Core operations (compute adler, attach to inflate, open gz on fd and check eof)
    uLong adl = adler32_z(0UL, (const Bytef *)in_buf, (z_size_t)payload_len);
    istrm.adler = adl;
    gzFile gzf = gzdopen(1, "wb");
    int eof_before = gzeof(gzf);

    // step 4: Validate and cleanup
    int rc_gzclose = gzclose(gzf);
    int rc_deflate_end_copy = deflateEnd(&dcopy);
    int rc_deflate_end_src = deflateEnd(&dsrc);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] in_buf;
    delete [] out_buf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate_copy;
    (void)rc_inflate_init;
    (void)adl;
    (void)eof_before;
    (void)rc_gzclose;
    (void)rc_deflate_end_copy;
    (void)rc_deflate_end_src;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}