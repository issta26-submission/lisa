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
//<ID> 1200
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for gzdopen/deflateCopy/adler32_z/gzeof/inflateInit_ demonstration";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream def_src;
    z_stream def_dst;
    z_stream istrm;
    memset(&def_src, 0, (size_t)sizeof(z_stream));
    memset(&def_dst, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize and copy deflate state, initialize inflate)
    int rc_deflate_init = deflateInit_(&def_src, 6, version, (int)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&def_dst, &def_src);
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Core operations (compute adler32, open gz on fd, write, check eof)
    uLong checksum = adler32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    gzFile gzf = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)payload, (unsigned int)(payload_len ? payload_len : 0));
    int rc_gzflush = gzflush(gzf, 0);
    int is_eof = gzeof(gzf);

    // step 4: Validate and cleanup
    int rc_gzclose = gzclose(gzf);
    int rc_deflate_end_src = deflateEnd(&def_src);
    int rc_deflate_end_dst = deflateEnd(&def_dst);
    int rc_inflate_end = inflateEnd(&istrm);
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate_copy;
    (void)rc_inflate_init;
    (void)checksum;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)is_eof;
    (void)rc_gzclose;
    (void)rc_deflate_end_src;
    (void)rc_deflate_end_dst;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}