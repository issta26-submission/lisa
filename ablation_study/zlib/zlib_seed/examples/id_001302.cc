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
//<ID> 1302
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload with dictionary and gzip";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    Bytef dict_data[] = { 'd', 'i', 'c', 't', '_', 'v', '1' };
    uInt dictLen = (uInt)(sizeof(dict_data));
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize deflate and inflate streams and set dictionary)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_set_dict = deflateSetDictionary(&dstrm, dict_data, dictLen);
    uLong bound = deflateBound(&dstrm, srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;

    // step 3: Core operations (compress with deflate, write to gzip, inflate back, compute adler combine)
    int rc_deflate = deflate(&dstrm, 4);
    uLong compUsed = dstrm.total_out;
    gzFile gzf = gzopen("zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)compUsed);
    int rc_gzclose = gzclose(gzf);
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compUsed;
    Bytef *decBuf = new Bytef[(size_t)srcLen];
    memset(decBuf, 0, (size_t)srcLen);
    istrm.next_out = decBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate = inflate(&istrm, 4);
    uLong ad1 = adler32(0UL, src, (uInt)srcLen);
    uLong ad2 = adler32(0UL, dict_data, dictLen);
    uLong combined = adler32_combine(ad1, ad2, (off_t)dictLen);

    // step 4: Cleanup and finalization
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] compBuf;
    delete [] decBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_set_dict;
    (void)bound;
    (void)rc_deflate;
    (void)compUsed;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)ad1;
    (void)ad2;
    (void)combined;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}