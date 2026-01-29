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
//<ID> 1205
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize streams
    const char payload[] = "zlib sequence payload for gzdopen/deflateCopy/adler32_z/inflateInit_ usage";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream src;
    z_stream dst;
    z_stream istrm;
    memset(&src, 0, (size_t)sizeof(z_stream));
    memset(&dst, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *outbuf = new unsigned char[2048];
    memset(outbuf, 0, (size_t)2048);

    int rc_deflate_init = deflateInit_(&src, 6, version, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 2: Configure deflate source and copy to destination
    src.next_in = (Bytef *)payload;
    src.avail_in = (uInt)payload_len;
    src.next_out = outbuf;
    src.avail_out = (uInt)2048;
    int rc_deflate_copy = deflateCopy(&dst, &src);

    // step 3: Core operations — compute adler, open gz via fd, write payload
    uLong adler = adler32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    gzFile gzf = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)payload, (unsigned int)(payload_len ? payload_len : 0));

    // step 4: Validate and cleanup
    int rc_gzeof = gzeof(gzf);
    int rc_deflate_end_src = deflateEnd(&src);
    int rc_deflate_end_dst = deflateEnd(&dst);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gzf);
    delete [] outbuf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)rc_deflate_copy;
    (void)adler;
    (void)rc_gzwrite;
    (void)rc_gzeof;
    (void)rc_deflate_end_src;
    (void)rc_deflate_end_dst;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}