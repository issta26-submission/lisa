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
//<ID> 350
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare data
    const char *version = zlibVersion();
    const Bytef dict[] = "example_dictionary";
    const Bytef part1[] = "Hello, ";
    const Bytef part2[] = "zlib!";
    uInt len1 = (uInt)(sizeof(part1) - 1);
    uInt len2 = (uInt)(sizeof(part2) - 1);
    Bytef *srcBuf = (Bytef *)malloc((size_t)(len1 + len2));
    memcpy(srcBuf, part1, len1);
    memcpy(srcBuf + len1, part2, len2);

    // step 2: Initialize and configure deflate stream, set dictionary, perform a deflate call and query pending
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_set_dict = deflateSetDictionary(&dstrm, dict, (uInt)(sizeof(dict) - 1));
    dstrm.next_in = srcBuf;
    dstrm.avail_in = (uInt)(len1 + len2);
    uInt outBufLen = 256;
    Bytef *outBuf = (Bytef *)malloc((size_t)outBufLen);
    memset(outBuf, 0, (size_t)outBufLen);
    dstrm.next_out = outBuf;
    dstrm.avail_out = outBufLen;
    int rc_deflate = deflate(&dstrm, 0);
    unsigned int pending_bytes = 0;
    int pending_bits = 0;
    int rc_pending = deflatePending(&dstrm, &pending_bytes, &pending_bits);

    // step 3: Validate with adler32 combine and exercise gzFile APIs including gzerror
    uLong ad1 = adler32(0L, srcBuf, len1);
    uLong ad2 = adler32(0L, srcBuf + len1, len2);
    uLong ad_combined = adler32_combine(ad1, ad2, (off_t)len2);
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile wfile = gzopen(fname, "wb");
    unsigned int produced = (unsigned int)(outBufLen - dstrm.avail_out);
    int rc_gzwrite = gzwrite(wfile, (voidpc)outBuf, produced);
    int gz_errnum1 = 0;
    const char *gz_errstr1 = gzerror(wfile, &gz_errnum1);
    int rc_gzclose_w = gzclose(wfile);
    gzFile rfile = gzopen(fname, "rb");
    Bytef *readBuf = (Bytef *)malloc(512);
    memset(readBuf, 0, 512);
    int rc_gzread = gzread(rfile, (voidp)readBuf, 512);
    int gz_errnum2 = 0;
    const char *gz_errstr2 = gzerror(rfile, &gz_errnum2);
    int rc_gzclose_r = gzclose(rfile);

    // step 4: Cleanup deflate stream and free resources
    int rc_def_end = deflateEnd(&dstrm);
    free(srcBuf);
    free(outBuf);
    free(readBuf);

    (void)version;
    (void)rc_def_init;
    (void)rc_set_dict;
    (void)rc_deflate;
    (void)rc_pending;
    (void)ad_combined;
    (void)pending_bytes;
    (void)pending_bits;
    (void)rc_gzwrite;
    (void)gz_errnum1;
    (void)gz_errstr1;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)gz_errnum2;
    (void)gz_errstr2;
    (void)rc_gzclose_r;
    (void)rc_def_end;

    // API sequence test completed successfully
    return 66;
}