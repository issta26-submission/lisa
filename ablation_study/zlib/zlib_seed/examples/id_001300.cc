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
//<ID> 1300
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for inflate/deflate/dictionary";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    uInt dictLen = (uInt)(srcLen >= 4 ? 4 : srcLen);
    Bytef *dict = new Bytef[(size_t)dictLen];
    memset(dict, 0, (size_t)dictLen);
    memcpy(dict, src, (size_t)dictLen);
    const char *version = zlibVersion();

    // step 2: Setup (initialize deflate stream and set dictionary, prepare compression buffer)
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_set_dict = deflateSetDictionary(&dstrm, dict, dictLen);
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;
    int rc_compress = compress2(compBuf, &compLen_f, src, srcLen, 6);

    // step 3: Core operations (open gzip, write compressed data, initialize inflate and run inflate, combine adler32 checksums)
    gzFile gzf = gzopen("zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)compLen_f);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen_f;
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate = inflate(&istrm, 0);
    uInt part1 = (uInt)(srcLen / 2);
    uInt part2 = (uInt)(srcLen - part1);
    uLong ad1 = adler32(0UL, src, part1);
    uLong ad2 = adler32(0UL, src + part1, part2);
    uLong ad_combined = adler32_combine(ad1, ad2, (off_t)part2);

    // step 4: Cleanup and finalization
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    delete [] dict;
    delete [] compBuf;
    delete [] outBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_set_dict;
    (void)bound;
    (void)rc_compress;
    (void)rc_gzwrite;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)ad1;
    (void)ad2;
    (void)ad_combined;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}