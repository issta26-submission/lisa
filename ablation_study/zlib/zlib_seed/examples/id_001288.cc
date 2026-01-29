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
//<ID> 1288
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for crc, gzwrite and inflate";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uInt len32 = (uInt)srcLen;
    uLong crc_before = crc32(0UL, src, len32);

    // step 2: Setup (open gzip file and write data)
    gzFile gzf = gzopen("zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)len32);
    off64_t pos_after_write = gzoffset64(gzf);
    gzclearerr(gzf);

    // step 3: Core operations (initialize inflate, attempt inflate, compute crc of output)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = src;
    istrm.avail_in = len32;
    Bytef *out = new Bytef[(size_t)(srcLen + 16UL)];
    memset(out, 0, (size_t)(srcLen + 16UL));
    istrm.next_out = out;
    istrm.avail_out = (uInt)(srcLen + 16UL);
    int rc_inflate = inflate(&istrm, 0);
    uLong crc_after = crc32(crc_before, out, (uInt)srcLen);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    delete [] out;
    (void)version;
    (void)crc_before;
    (void)crc_after;
    (void)rc_gzwrite;
    (void)pos_after_write;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}