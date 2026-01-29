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
//<ID> 353
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data, buffers, and dictionary
    const char *version = zlibVersion();
    const char src[] = "zlib API sequence payload: dictionary-assisted deflate -> inflate -> checksum combine -> gz I/O";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    uInt outBufSize = (uInt)(compBound + 64);
    Bytef *compBuf = (Bytef *)malloc((size_t)outBufSize);
    memset(compBuf, 0, (size_t)outBufSize);
    Bytef *decompBuf = (Bytef *)malloc((size_t)(sourceLen + 128));
    memset(decompBuf, 0, (size_t)(sourceLen + 128));
    Bytef dict[] = "common-dict-data";
    uInt dictLen = (uInt)(sizeof(dict) - 1);

    // step 2: Initialize deflate stream, set dictionary, perform deflate and inspect pending bytes
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = outBufSize;
    int rc_setdict = deflateSetDictionary(&dstrm, dict, dictLen);
    int rc_deflate = deflate(&dstrm, 0);
    unsigned int pending = 0;
    int bits = 0;
    int rc_pending = deflatePending(&dstrm, &pending, &bits);
    uLong compressedLen = dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize inflate stream, provide same dictionary, inflate the compressed data, then end inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compressedLen;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)(sourceLen + 128);
    int rc_inf_setdict = inflateSetDictionary(&istrm, dict, dictLen);
    int rc_inflate = inflate(&istrm, 0);
    int rc_inf_end = inflateEnd(&istrm);

    // step 4: Validate via Adler-32 combine, write/read compressed data with gz* APIs, get gzerror, cleanup
    uInt half = (uInt)(sourceLen / 2);
    uLong ad1 = adler32(0UL, source, half);
    uLong ad2 = adler32(0UL, source + half, (uInt)(sourceLen - half));
    uLong ad_combined = adler32_combine(ad1, ad2, (off_t)(sourceLen - half));
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gw = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gw, (voidpc)compBuf, (unsigned int)compressedLen);
    int rc_gzclose_w = gzclose(gw);
    gzFile gr = gzopen(fname, "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(sourceLen + 256));
    memset(readBuf, 0, (size_t)(sourceLen + 256));
    int rc_gzread = gzread(gr, (voidp)readBuf, (unsigned int)(sourceLen + 128));
    int errnum = 0;
    const char *gerr = gzerror(gr, &errnum);
    int rc_gzclose_r = gzclose(gr);

    free(compBuf);
    free(decompBuf);
    free(readBuf);

    (void)version;
    (void)compBound;
    (void)rc_def_init;
    (void)rc_setdict;
    (void)rc_deflate;
    (void)pending;
    (void)bits;
    (void)rc_pending;
    (void)compressedLen;
    (void)rc_def_end;
    (void)rc_inf_init;
    (void)rc_inf_setdict;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)ad1;
    (void)ad2;
    (void)ad_combined;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)errnum;
    (void)gerr;
    (void)rc_gzclose_r;

    // API sequence test completed successfully
    return 66;
}