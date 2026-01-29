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
//<ID> 1203
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib sequence payload for gzdopen/gzwrite/adler32_z/deflateCopy/inflateInit_";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dsrc;
    z_stream ddest;
    z_stream istrm;
    memset(&dsrc, 0, (size_t)sizeof(z_stream));
    memset(&ddest, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize deflate source and inflate stream, then copy deflate)
    int rc_deflate_init = deflateInit_(&dsrc, 6, version, (int)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&ddest, &dsrc);
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Core operations (open gz on fd 1, write payload, compute adler checksum, check eof)
    gzFile gzf = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gzf, payload, (unsigned int)(payload_len ? payload_len : 0));
    uLong checksum = adler32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    int eof_flag = gzeof(gzf);

    // step 4: Validate and cleanup
    int rc_gzclose = gzclose(gzf);
    int rc_deflate_end_src = deflateEnd(&dsrc);
    int rc_deflate_end_dst = deflateEnd(&ddest);
    int rc_inflate_end = inflateEnd(&istrm);
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate_copy;
    (void)rc_inflate_init;
    (void)rc_gzwrite;
    (void)checksum;
    (void)eof_flag;
    (void)rc_gzclose;
    (void)rc_deflate_end_src;
    (void)rc_deflate_end_dst;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}