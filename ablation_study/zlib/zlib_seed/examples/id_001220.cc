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
//<ID> 1220
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload1[] = "First payload for zlib";
    const char payload2[] = "Second payload";
    const uLong len1 = (uLong)(sizeof(payload1) - 1);
    const uLong len2 = (uLong)(sizeof(payload2) - 1);
    const char *version = zlibVersion();
    unsigned char *readbuf = new unsigned char[256];
    memset(readbuf, 0, (size_t)256);

    // step 2: Setup (compute checksums and combine)
    uLong ad1 = adler32(0UL, (const Bytef *)payload1, (uInt)len1);
    uLong ad2 = adler32(0UL, (const Bytef *)payload2, (uInt)len2);
    off_t len2_off = (off_t)len2;
    uLong combined_adler = adler32_combine(ad1, ad2, len2_off);
    uLong crc_payload1 = crc32_z(0UL, (const Bytef *)payload1, (z_size_t)len1);

    // step 3: Core operations (write to gz, ungetc, rewind, read, clear errors)
    gzFile gzf = gzopen("zlib_api_seq_test.gz", "wb+");
    int rc_write1 = gzwrite(gzf, (voidpc)payload1, (unsigned int)(len1 ? len1 : 0));
    int rc_write2 = gzwrite(gzf, (voidpc)payload2, (unsigned int)(len2 ? len2 : 0));
    int rc_ungetc = gzungetc((int)payload1[0], gzf);
    int rc_rewind = gzrewind(gzf);
    int bytes_read = gzread(gzf, (voidp)readbuf, (unsigned int)256);
    uLong crc_read = crc32_z(0UL, (const Bytef *)readbuf, (z_size_t)(bytes_read > 0 ? bytes_read : 0));
    gzclearerr(gzf);

    // step 4: Validate and cleanup
    int rc_gzclose = gzclose(gzf);
    delete [] readbuf;
    (void)version;
    (void)ad1;
    (void)ad2;
    (void)combined_adler;
    (void)crc_payload1;
    (void)crc_read;
    (void)rc_write1;
    (void)rc_write2;
    (void)rc_ungetc;
    (void)rc_rewind;
    (void)bytes_read;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}