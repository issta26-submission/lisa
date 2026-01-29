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
//<ID> 1303
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib fuzz payload with dictionary";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    const char *version = zlibVersion();
    uLong bound = compressBound(srcLen);
    Bytef *outBuf = new Bytef[(size_t)(srcLen + bound + 16)];
    memset(outBuf, 0, (size_t)(srcLen + bound + 16));

    // step 2: Setup (initialize deflate and inflate streams and open gzip file)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    gzFile gzf = gzopen("zlib_api_seq_out.gz", "wb");

    // step 3: Core operations (configure dictionary, checksum combine, write and attempt inflate)
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    int rc_setdict = deflateSetDictionary(&dstrm, src, (uInt)srcLen);
    uLong ad1 = adler32(0UL, src, (uInt)srcLen);
    uLong ad2 = adler32(0UL, src, (uInt)srcLen);
    uLong combined_ad = adler32_combine(ad1, ad2, (off_t)srcLen);
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    istrm.next_in = src;
    istrm.avail_in = (uInt)srcLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)(srcLen + bound + 16);
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] src;
    delete [] outBuf;
    (void)version;
    (void)bound;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)rc_setdict;
    (void)combined_ad;
    (void)rc_gzwrite;
    (void)rc_inflate;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}