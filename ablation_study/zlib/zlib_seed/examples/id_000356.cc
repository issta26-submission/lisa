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
//<ID> 356
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and buffers
    const char src[] = "zlib API sequence payload for deflate with dictionary and gz write";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char dict[] = "preset_dictionary";
    const Bytef *dictionary = (const Bytef *)dict;
    uLong dictLen = (uLong)(sizeof(dict) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);

    // step 2: Initialize deflate stream and set dictionary
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_setdict = deflateSetDictionary(&dstrm, dictionary, (uInt)dictLen);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;

    // step 3: Perform deflate, query pending bits/bytes, and finish deflate
    int rc_deflate = deflate(&dstrm, 0);
    unsigned int pending = 0;
    int bits = 0;
    int rc_def_pending = deflatePending(&dstrm, &pending, &bits);
    uLong compOutLen = dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 4: Write compressed data to gzip file, compute checksums, query gz error, cleanup
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)compOutLen);
    int gz_errnum = 0;
    const char *gz_errstr = gzerror(gzf, &gz_errnum);
    int rc_gzclose = gzclose(gzf);
    uLong a1 = adler32(0UL, source, (uInt)sourceLen);
    uLong a2 = adler32(0UL, dictionary, (uInt)dictLen);
    uLong a_combined = adler32_combine(a1, a2, (off_t)dictLen);

    free(compBuf);

    (void)version;
    (void)rc_def_init;
    (void)rc_def_setdict;
    (void)rc_deflate;
    (void)rc_def_pending;
    (void)pending;
    (void)bits;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)gz_errstr;
    (void)gz_errnum;
    (void)rc_gzclose;
    (void)a1;
    (void)a2;
    (void)a_combined;
    // API sequence test completed successfully
    return 66;
}