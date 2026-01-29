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
//<ID> 1307
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib fuzz payload for api sequence";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    Bytef *dict = new Bytef[8];
    memset(dict, 0, 8);
    memcpy(dict, "dictdata", 8);
    uInt dictLen = (uInt)8;
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    Bytef *out = new Bytef[(size_t)srcLen];
    memset(out, 0, (size_t)srcLen);

    // step 2: Setup (initialize deflate and set dictionary)
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    int rc_def_setdict = deflateSetDictionary(&dstrm, dict, dictLen);

    // step 3: Core operations (open gzip, compute and combine adler32 checksums, write out, initialize inflate and run)
    gzFile gzf = gzopen("zlib_api_seq_out.gz", "wb");
    uLong ad1 = adler32(0UL, src, (uInt)srcLen);
    uLong ad2 = adler32(1UL, src, (uInt)srcLen);
    uLong ad_comb = adler32_combine(ad1, ad2, (off_t)srcLen);
    int rc_gzwrite = gzwrite(gzf, (voidpc)&ad_comb, (unsigned int)sizeof(ad_comb));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = src;
    istrm.avail_in = (uInt)srcLen;
    istrm.next_out = out;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    delete [] src;
    delete [] dict;
    delete [] out;
    (void)version;
    (void)rc_def_init;
    (void)rc_def_setdict;
    (void)ad1;
    (void)ad2;
    (void)ad_comb;
    (void)rc_gzwrite;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_gzclose;
    (void)rc_def_end;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}