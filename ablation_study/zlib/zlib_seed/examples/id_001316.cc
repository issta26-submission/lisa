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
//<ID> 1316
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for inflate and gz operations";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;

    // step 2: Setup (compress source, init inflate, configure window)
    int rc_compress = compress2(compBuf, &compLen_f, src, srcLen, 6);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen_f;
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_reset2 = inflateReset2(&istrm, 15);

    // step 3: Core operations (inflate, compute adler32, write gz file and seek)
    int rc_inflate = inflate(&istrm, 0);
    uLong checksum = adler32(0UL, src, (uInt)srcLen);
    gzFile gzf = gzopen64("zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)compLen_f);
    off_t pos = gzseek(gzf, 0, 0);

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)version;
    (void)bound;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_reset2;
    (void)rc_inflate;
    (void)checksum;
    (void)rc_gzwrite;
    (void)pos;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}