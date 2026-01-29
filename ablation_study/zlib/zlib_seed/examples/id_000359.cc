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
//<ID> 359
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload, version, and buffers
    const char src[] = "zlib API sequence payload: deflate with dictionary, pending check, adler combine, gzwrite";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    const Bytef dict[] = "preset_dictionary";
    uInt dictLen = (uInt)(sizeof("preset_dictionary") - 1);
    uInt halfLen = (uInt)(sourceLen / 2);
    uInt secondLen = (uInt)(sourceLen - halfLen);

    // step 2: Compute adler checksums for halves and combine them
    uLong adler1 = adler32(1UL, source, halfLen);
    uLong adler2 = adler32(1UL, source + halfLen, secondLen);
    uLong adler_combined = adler32_combine(adler1, adler2, (off_t)secondLen);

    // step 3: Initialize deflate stream, set dictionary, perform deflate, query pending
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_set_dict = deflateSetDictionary(&dstrm, dict, dictLen);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_deflate = deflate(&dstrm, 0);
    unsigned int pending_bytes = 0;
    int pending_bits = 0;
    int rc_pending = deflatePending(&dstrm, &pending_bytes, &pending_bits);

    // step 4: Write compressed output to a gzip file, retrieve gz error info, cleanup
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)dstrm.total_out);
    int gz_errnum = 0;
    const char *gz_errstr = gzerror(gzf, &gz_errnum);
    int rc_def_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gzf);
    free(compBuf);

    (void)version;
    (void)compBound;
    (void)rc_def_init;
    (void)rc_set_dict;
    (void)rc_deflate;
    (void)pending_bytes;
    (void)pending_bits;
    (void)rc_pending;
    (void)adler1;
    (void)adler2;
    (void)adler_combined;
    (void)rc_gzwrite;
    (void)gz_errnum;
    (void)gz_errstr;
    (void)rc_def_end;
    (void)rc_gzclose;

    // API sequence test completed successfully
    return 66;
}