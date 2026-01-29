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
//<ID> 357
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data and query zlib version
    const char payload[] = "zlib API sequence payload: deflateSetDictionary -> deflatePending -> adler32_combine -> gzerror";
    const Bytef *src = (const Bytef *)payload;
    uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();

    // step 2: Initialize a deflate stream and set a dictionary, then query pending bits
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    Bytef dict[] = "example-dictionary";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    int rc_setdict = deflateSetDictionary(&dstrm, dict, dictLen);
    unsigned int pending = 0;
    int bits = 0;
    int rc_pending = deflatePending(&dstrm, &pending, &bits);

    // step 3: Compute two Adler-32 checksums for halves of the payload and combine them
    uInt len1 = (uInt)(srcLen / 2);
    uInt len2 = (uInt)(srcLen - len1);
    uLong ad1 = adler32(1UL, src, len1);
    uLong ad2 = adler32(1UL, src + len1, len2);
    uLong ad_combined = adler32_combine(ad1, ad2, (off_t)len2);

    // step 4: Write payload to a gzip file, read it back, query gzerror, and cleanup
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile wfile = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(wfile, (voidpc)src, (unsigned int)srcLen);
    int rc_gzclose_w = gzclose(wfile);
    gzFile rfile = gzopen(fname, "rb");
    unsigned int readBufLen = (unsigned int)(srcLen + 16);
    Bytef *readBuf = (Bytef *)malloc((size_t)readBufLen);
    memset(readBuf, 0, (size_t)readBufLen);
    int rc_gzread = gzread(rfile, (voidp)readBuf, readBufLen);
    int errnum = 0;
    const char *errstr = gzerror(rfile, &errnum);
    int rc_gzclose_r = gzclose(rfile);
    int rc_def_end = deflateEnd(&dstrm);
    free(readBuf);

    (void)version;
    (void)rc_def_init;
    (void)rc_setdict;
    (void)pending;
    (void)bits;
    (void)rc_pending;
    (void)ad1;
    (void)ad2;
    (void)ad_combined;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)errnum;
    (void)errstr;
    (void)rc_gzclose_r;
    (void)rc_def_end;

    // API sequence test completed successfully
    return 66;
}