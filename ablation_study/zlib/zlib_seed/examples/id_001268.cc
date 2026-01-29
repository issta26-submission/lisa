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
//<ID> 1268
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "Hello zlib payload for API sequence";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    uLong crc_val = crc32(0UL, src, (uInt)srcLen);
    uLong ad1 = adler32(1UL, src, (uInt)srcLen);
    uLong ad_combined = adler32_combine(ad1, ad1, (off_t)srcLen);

    // step 2: Setup (inflate stream and header)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    gz_header hdr;
    memset(&hdr, 0, (size_t)sizeof(gz_header));
    int rc_hdr = inflateGetHeader(&istrm, &hdr);

    // step 3: Core operations (write some bytes into a gz file using gzputc)
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_put0 = gzputc(gzf, (int)src[0]);
    int rc_put1 = gzputc(gzf, (int)src[1]);
    int rc_put2 = gzputc(gzf, (int)src[2]);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup and validation
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    (void)version;
    (void)crc_val;
    (void)ad1;
    (void)ad_combined;
    (void)rc_inflate_init;
    (void)rc_hdr;
    (void)rc_put0;
    (void)rc_put1;
    (void)rc_put2;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}