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
//<ID> 1206
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "Example payload for zlib API sequence using gzdopen/deflateCopy/adler32_z/inflateInit_";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dsrc;
    z_stream ddest;
    z_stream istrm;
    memset(&dsrc, 0, (size_t)sizeof(z_stream));
    memset(&ddest, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    Bytef *work_buf = new Byte[(size_t)128];
    memset(work_buf, 0, (size_t)128);

    // step 2: Setup (initialize deflate source and inflate)
    int rc_deflate_init = deflateInit_(&dsrc, 6, version, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Core operations (copy deflate state, compute adler, open gz on fd and check eof)
    int rc_deflate_copy = deflateCopy(&ddest, &dsrc);
    uLong checksum = adler32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    gzFile gzf = gzdopen(1, "rb");
    int is_eof = gzeof(gzf);

    // step 4: Validate and cleanup
    int rc_gzclose = gzclose(gzf);
    int rc_deflate_end_src = deflateEnd(&dsrc);
    int rc_deflate_end_dest = deflateEnd(&ddest);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] work_buf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)rc_deflate_copy;
    (void)checksum;
    (void)is_eof;
    (void)rc_gzclose;
    (void)rc_deflate_end_src;
    (void)rc_deflate_end_dest;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}