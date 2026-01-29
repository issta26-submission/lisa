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
//<ID> 1311
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib_api_sequence_payload";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong ad = adler32(0UL, src, (uInt)srcLen);

    // step 2: Setup (initialize and configure inflate stream)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    istrm.adler = ad;
    int rc_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_reset2 = inflateReset2(&istrm, 15);
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);
    istrm.next_in = src;
    istrm.avail_in = (uInt)srcLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;

    // step 3: Core operations (inflate, write to gz file and seek)
    int rc_inflate = inflate(&istrm, 0);
    gzFile gzf = gzopen64("zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)outBuf, (unsigned int)srcLen);
    off_t pos = gzseek(gzf, (off_t)0, 0);

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] outBuf;
    (void)version;
    (void)ad;
    (void)rc_init;
    (void)rc_reset2;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)pos;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}