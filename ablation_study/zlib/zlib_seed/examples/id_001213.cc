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
//<ID> 1213
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "Example payload for zlib sequence";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char outbuf[512];
    memset(outbuf, 0, (size_t)sizeof(outbuf));
    gzFile gzf = gzopen("zlib_seq_output.gz", "wb");
    uLong crc = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong adl = adler32(0UL, (const Bytef *)payload, (uInt)payload_len);

    // step 2: Setup (initialize streams)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)payload;
    dstrm.avail_in = (uInt)payload_len;
    dstrm.next_out = outbuf;
    dstrm.avail_out = (uInt)sizeof(outbuf);

    // step 3: Core operations
    int rc_gzwrite = gzwrite(gzf, (voidpc)payload, (unsigned int)(payload_len ? payload_len : 0));
    int rc_inflate_back_end = inflateBackEnd(&istrm);

    // step 4: Cleanup and validation
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gzf);
    (void)version;
    (void)crc;
    (void)adl;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)rc_gzwrite;
    (void)rc_inflate_back_end;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}