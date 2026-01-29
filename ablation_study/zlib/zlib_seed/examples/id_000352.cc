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
//<ID> 352
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data and compute Adler-32 pieces
    const char payload[] = "zlib API sequence payload: partitioned adler32 -> compress -> gz I/O -> deflate dictionary";
    const Bytef *source = (const Bytef *)payload;
    uLong sourceLen = (uLong)(sizeof(payload) - 1);
    uInt halfLen = (uInt)(sourceLen / 2);
    uLong ad1 = adler32(0L, source, halfLen);
    uLong ad2 = adler32(0L, source + halfLen, (uInt)(sourceLen - halfLen));
    uLong combined_ad = adler32_combine(ad1, ad2, (off_t)(sourceLen - halfLen));
    const char *version = zlibVersion();

    // step 2: Compress the payload into a buffer
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Write original payload to a gzip file and read it back, check gzerror
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile wfile = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(wfile, (voidpc)source, (unsigned int)sourceLen);
    int rc_gzclose_w = gzclose(wfile);
    gzFile rfile = gzopen(fname, "rb");
    unsigned int readBufLen = (unsigned int)(sourceLen + 64);
    Bytef *readBuf = (Bytef *)malloc((size_t)readBufLen);
    memset(readBuf, 0, (size_t)readBufLen);
    int rc_gzread = gzread(rfile, (voidp)readBuf, readBufLen);
    int gz_errnum = 0;
    const char *gz_errstr = gzerror(rfile, &gz_errnum);

    // step 4: Initialize deflate stream, set a dictionary, query pending bytes, cleanup
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    const Bytef dict[] = "dict_data_for_deflate";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    int rc_def_setdict = deflateSetDictionary(&dstrm, dict, dictLen);
    unsigned int pending = 0;
    int bits = 0;
    int rc_def_pending = deflatePending(&dstrm, &pending, &bits);
    int rc_def_end = deflateEnd(&dstrm);
    int rc_gzclose_r = gzclose(rfile);

    free(compBuf);
    free(readBuf);

    (void)combined_ad;
    (void)version;
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)gz_errstr;
    (void)gz_errnum;
    (void)rc_def_init;
    (void)rc_def_setdict;
    (void)pending;
    (void)bits;
    (void)rc_def_pending;
    (void)rc_def_end;
    (void)rc_gzclose_r;

    // API sequence test completed successfully
    return 66;
}