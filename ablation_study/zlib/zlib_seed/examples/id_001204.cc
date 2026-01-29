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
//<ID> 1204
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib sequence payload for gzdopen/gzeof/deflateCopy/adler32_z/inflateInit_ use";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream src;
    z_stream dst;
    z_stream istrm;
    memset(&src, 0, (size_t)sizeof(z_stream));
    memset(&dst, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize inflate and deflate source, then copy)
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&src, 6, version, (int)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&dst, &src);

    // step 3: Core operations (compute checksum, write to gz from fd, check eof)
    uLong checksum = adler32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    gzFile gzf = gzdopen(1, "wb"); /* use stdout file descriptor */
    int rc_gzputs = gzputs(gzf, payload);
    int rc_gzeof = gzeof(gzf);

    // step 4: Cleanup and validate
    int rc_deflate_end_dst = deflateEnd(&dst);
    int rc_deflate_end_src = deflateEnd(&src);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gzf);
    (void)version;
    (void)rc_inflate_init;
    (void)rc_deflate_init;
    (void)rc_deflate_copy;
    (void)checksum;
    (void)rc_gzputs;
    (void)rc_gzeof;
    (void)rc_deflate_end_dst;
    (void)rc_deflate_end_src;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}