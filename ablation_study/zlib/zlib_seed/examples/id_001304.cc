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
//<ID> 1304
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
    uLong dictLen = srcLen;
    Bytef *dict = new Bytef[(size_t)dictLen];
    memset(dict, 0, (size_t)dictLen);
    memcpy(dict, payload, (size_t)dictLen);
    const char *version = zlibVersion();
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    z_stream istrm;
    z_stream dstrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&dstrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = src;
    istrm.avail_in = (uInt)srcLen;
    istrm.next_out = compBuf;
    istrm.avail_out = (uInt)bound;
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = dict;
    dstrm.avail_in = (uInt)dictLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;

    // step 3: Core operations
    int rc_deflate_set_dict = deflateSetDictionary(&dstrm, dict, (uInt)dictLen);
    int rc_inflate = inflate(&istrm, 0);
    uLong ad1 = adler32(0UL, src, (uInt)srcLen);
    uLong ad2 = adler32(0UL, dict, (uInt)dictLen);
    off_t combine_len = (off_t)srcLen;
    uLong combined = adler32_combine(ad1, ad2, combine_len);
    gzFile gzf = gzopen("zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)bound);

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] src;
    delete [] dict;
    delete [] compBuf;
    (void)version;
    (void)bound;
    (void)rc_inflate_init;
    (void)rc_deflate_init;
    (void)rc_deflate_set_dict;
    (void)rc_inflate;
    (void)ad1;
    (void)ad2;
    (void)combined;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}