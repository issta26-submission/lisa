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
//<ID> 1306
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib fuzz payload sequence";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize streams and prepare buffers / dictionary)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_defdict = deflateSetDictionary(&dstrm, src, (uInt)srcLen);
    Bytef *out = new Bytef[(size_t)srcLen];
    memset(out, 0, (size_t)srcLen);
    istrm.next_in = src;
    istrm.avail_in = (uInt)srcLen;
    istrm.next_out = out;
    istrm.avail_out = (uInt)srcLen;

    // step 3: Core operations (inflate attempt, gzip write, compute and combine adler32)
    int rc_inflate = inflate(&istrm, 0);
    gzFile gzf = gzopen("zlib_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    uLong a1 = adler32(0UL, src, (uInt)(srcLen / 2));
    uLong a2 = adler32(0UL, src + (srcLen / 2), (uInt)(srcLen - (srcLen / 2)));
    uLong combined_adler = adler32_combine(a1, a2, (off_t)(srcLen - (srcLen / 2)));

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] out;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)rc_defdict;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)combined_adler;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}