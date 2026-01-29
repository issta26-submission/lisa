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
//<ID> 1216
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib sequence payload for crc32/adler32/deflateInit_/inflateBackInit_/gzclose";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *in_window = new unsigned char[256];
    memset(in_window, 0, (size_t)256);

    // step 2: Setup (initialize streams and compute checksums)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_inflate_back_init = inflateBackInit_(&istrm, 15, in_window, version, (int)sizeof(z_stream));
    uLong payload_crc = crc32(0UL, (const Bytef *)payload, (uInt)payload_len);
    uLong payload_adler = adler32(0UL, (const Bytef *)payload, (uInt)payload_len);
    dstrm.adler = payload_adler;
    dstrm.total_in = payload_len;
    istrm.adler = payload_crc;

    // step 3: Core operations (write payload to a gzip file)
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)payload, (unsigned int)(payload_len ? payload_len : 0));

    // step 4: Validate and cleanup
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_back_end = inflateBackEnd(&istrm);
    int rc_gzclose = gzclose(gzf);
    delete [] in_window;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_inflate_back_init;
    (void)payload_crc;
    (void)payload_adler;
    (void)rc_gzwrite;
    (void)rc_deflate_end;
    (void)rc_inflate_back_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}