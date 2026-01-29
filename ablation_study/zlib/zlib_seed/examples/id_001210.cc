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
//<ID> 1210
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload1[] = "Hello zlib world";
    const char payload2[] = "Streaming compression test";
    const uLong len1 = (uLong)(sizeof(payload1) - 1);
    const uLong len2 = (uLong)(sizeof(payload2) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *outbuf = new unsigned char[512];
    memset(outbuf, 0, (size_t)512);

    // step 2: Setup (initialize streams and compute checksums)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong crc_val = crc32(0UL, (const Bytef *)payload2, (uInt)len2);
    uLong adler_val = adler32(0UL, (const Bytef *)payload1, (uInt)len1);
    dstrm.next_in = (Bytef *)payload1;
    dstrm.avail_in = (uInt)len1;
    dstrm.next_out = outbuf;
    dstrm.avail_out = (uInt)512;
    dstrm.adler = adler_val;

    // step 3: Core operations (write compressed-like data to gz, run inflate backend)
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int rc_write1 = gzwrite(gzf, (voidpc)payload1, (unsigned int)(len1 ? len1 : 0));
    int rc_write2 = gzwrite(gzf, (voidpc)payload2, (unsigned int)(len2 ? len2 : 0));
    int rc_inflate_back_end = inflateBackEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 4: Cleanup and validation
    int rc_gzclose = gzclose(gzf);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] outbuf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)crc_val;
    (void)adler_val;
    (void)rc_write1;
    (void)rc_write2;
    (void)rc_inflate_back_end;
    (void)rc_deflate_end;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}