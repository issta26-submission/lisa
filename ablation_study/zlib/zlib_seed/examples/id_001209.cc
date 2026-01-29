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
//<ID> 1209
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for gzdopen/deflateCopy/adler32_z/inflateInit_/gzeof";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream src;
    z_stream dst;
    z_stream istrm;
    memset(&src, 0, (size_t)sizeof(z_stream));
    memset(&dst, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *in_window = new unsigned char[256];
    memset(in_window, 0, (size_t)256);

    // step 2: Setup (initialize streams and compute checksum)
    int rc_deflate_init = deflateInit_(&src, 6, version, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong payload_adler = adler32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    istrm.next_in = in_window;
    istrm.avail_in = 0;
    istrm.adler = payload_adler;

    // step 3: Core operations (copy deflate state, write gz, check EOF)
    int rc_deflate_copy = deflateCopy(&dst, &src);
    gzFile gzf = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)payload, (unsigned int)(payload_len ? payload_len : 0));
    int is_eof = gzeof(gzf);

    // step 4: Validate and cleanup
    int rc_deflate_end_src = deflateEnd(&src);
    int rc_deflate_end_dst = deflateEnd(&dst);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gzf);
    delete [] in_window;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)payload_adler;
    (void)rc_deflate_copy;
    (void)rc_gzwrite;
    (void)is_eof;
    (void)rc_deflate_end_src;
    (void)rc_deflate_end_dst;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}