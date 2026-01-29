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
//<ID> 639
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and buffers
    const char src[] = "zlib API sequence test payload for read/decompress";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLongf compAlloc = (uLongf)compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = compAlloc;

    // step 2: Compress the source and persist to a gzFile
    int rc_compress = compress2(compBuf, &compLen, source, sourceLen, 6);
    gzFile gf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf_w, compBuf, (unsigned int)compLen);
    int rc_gzclose_w = gzclose(gf_w);

    // step 3: Read compressed bytes back via gzfread and query gz error
    gzFile gf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    z_size_t itemsRead = gzfread((voidp)compBuf, (z_size_t)1, (z_size_t)compLen, gf_r);
    int errnum = 0;
    const char *gzerrstr = gzerror(gf_r, &errnum);
    int rc_gzclose_r = gzclose(gf_r);

    // step 4: Decompress read bytes with uncompress2 and initialize an inflate stream
    uLongf outAlloc = (uLongf)(sourceLen + 64);
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    uLongf destLen = outAlloc;
    uLong srcLenForUncomp = (uLong)compLen;
    int rc_uncompress = uncompress2(outBuf, &destLen, compBuf, &srcLenForUncomp);
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inflate_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    int rc_inflate_end = inflateEnd(&istrm);

    free(compBuf);
    free(outBuf);
    (void)rc_compress;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)itemsRead;
    (void)gzerrstr;
    (void)errnum;
    (void)rc_gzclose_r;
    (void)rc_uncompress;
    (void)rc_inflate_init2;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}