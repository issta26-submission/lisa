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
//<ID> 355
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload, dictionary, and version
    const char payload[] = "zlib API sequence payload: small sample data for deflate/inflate with dictionary";
    const Bytef *source = (const Bytef *)payload;
    uLong sourceLen = (uLong)(sizeof(payload) - 1);
    Bytef dict[] = "sample_dictionary";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);

    // step 2: Initialize deflate stream, set dictionary, and perform a deflate operation
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    deflateSetDictionary(&dstrm, dict, dictLen);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    deflate(&dstrm, 0);
    uLong compSize = dstrm.total_out;

    // step 3: Inspect pending bits, compute checksum combination, and write compressed data to a gzip file
    unsigned int pending = 0;
    int bits = 0;
    deflatePending(&dstrm, &pending, &bits);
    uInt halfLen = (uInt)(sourceLen / 2);
    uLong ad1 = adler32(1UL, source, halfLen);
    uLong ad2 = adler32(1UL, source + halfLen, (uInt)(sourceLen - halfLen));
    uLong combined_adler = adler32_combine(ad1, ad2, (off_t)(sourceLen - halfLen));
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)compSize);
    int gz_errnum = 0;
    const char *gz_errstr = gzerror(gzf, &gz_errnum);
    int rc_gzclose_w = gzclose(gzf);

    // step 4: Initialize inflate stream, decompress the data, cleanup streams and buffers
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compSize;
    uLong decompAlloc = sourceLen + 64;
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompAlloc);
    memset(decompBuf, 0, (size_t)decompAlloc);
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompAlloc;
    inflate(&istrm, 0);
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(decompBuf);

    (void)version;
    (void)compBound;
    (void)compSize;
    (void)pending;
    (void)bits;
    (void)combined_adler;
    (void)rc_gzwrite;
    (void)gz_errstr;
    (void)gz_errnum;
    (void)rc_gzclose_w;
    (void)rc_inf_end;
    (void)rc_def_end;

    // API sequence test completed successfully
    return 66;
}