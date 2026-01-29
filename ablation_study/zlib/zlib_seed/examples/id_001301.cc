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
//<ID> 1301
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence: dictionary + inflate + gzopen + adler combine";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    uLong ad1 = adler32(1UL, src, (uInt)(srcLen / 2));
    uLong ad2 = adler32(1UL, src + (srcLen / 2), (uInt)(srcLen - (srcLen / 2)));
    uLong combined_ad = adler32_combine(ad1, ad2, (off_t)(srcLen - (srcLen / 2)));

    // step 2: Setup (initialize deflate and set dictionary)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_set_dict = deflateSetDictionary(&dstrm, src, (uInt)srcLen);

    // step 3: Core operations (initialize inflate, attempt inflate, open gz and write)
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = src;
    istrm.avail_in = (uInt)srcLen;
    int rc_inflate = inflate(&istrm, 0);
    gzFile gzf = gzopen("zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    (void)version;
    (void)ad1;
    (void)ad2;
    (void)combined_ad;
    (void)rc_deflate_init;
    (void)rc_set_dict;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}