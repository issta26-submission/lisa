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
//<ID> 1308
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib payload for deflateSetDictionary + inflate + gz";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *buf = new Bytef[(size_t)srcLen];
    memset(buf, 0, (size_t)srcLen);
    memcpy(buf, payload, (size_t)srcLen);
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize deflate and inflate streams)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Core operations (configure dictionary for deflate, write gzip, attempt inflate, combine adlers)
    int rc_set_dict = deflateSetDictionary(&dstrm, buf, (uInt)srcLen);
    gzFile gzf = gzopen("zlib_api_seq_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)buf, (unsigned int)srcLen);
    istrm.next_in = buf;
    istrm.avail_in = (uInt)srcLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate = inflate(&istrm, 0);
    uLong ad1 = adler32(0UL, buf, (uInt)srcLen);
    uLong ad2 = adler32(0UL, outBuf, (uInt)srcLen);
    uLong ad_comb = adler32_combine(ad1, ad2, (off_t)srcLen);

    // step 4: Cleanup and finalization
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gzf);
    delete [] buf;
    delete [] outBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)rc_set_dict;
    (void)rc_gzwrite;
    (void)rc_inflate;
    (void)ad1;
    (void)ad2;
    (void)ad_comb;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}