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
//<ID> 1287
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    Bytef *buf = new Bytef[(size_t)srcLen];
    memset(buf, 0, (size_t)srcLen);
    memcpy(buf, payload, (size_t)srcLen);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize inflate stream and open gzip file for writing)
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = buf;
    istrm.avail_in = (uInt)srcLen;
    gzFile gzf = gzopen("zlib_api_seq_out.gz", "wb");

    // step 3: Core operations (compute crc, write to gzip, inspect offset, clear any gz errors)
    uInt len32 = (uInt)srcLen;
    uLong crc = crc32(0UL, buf, len32);
    int rc_gzwrite = gzwrite(gzf, (voidpc)buf, (unsigned int)srcLen);
    off64_t pos64 = gzoffset64(gzf);
    gzclearerr(gzf);

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] buf;
    (void)version;
    (void)rc_inflate_init;
    (void)crc;
    (void)rc_gzwrite;
    (void)pos64;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}