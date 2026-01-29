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
//<ID> 636
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and buffers
    const char src[] = "zlib API sequence test payload";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_compress = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));

    // step 3: One-shot decompress using uncompress2
    uLongf outAlloc = (uLongf)(sourceLen + 64);
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    uLong compLenCopy = (uLong)compLen;
    int rc_uncompress = uncompress2(outBuf, &outAlloc, compBuf, &compLenCopy);

    // step 4: Persist original data to a gzFile, read back using gzfread, query gzerror, and cleanup
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, source, (unsigned int)sourceLen);
    int rc_gzclose1 = gzclose(gf);
    gzFile gf2 = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    z_size_t readItems = gzfread((voidp)outBuf, (z_size_t)1, (z_size_t)outAlloc, gf2);
    int errnum = 0;
    const char *errstr = gzerror(gf2, &errnum);
    int rc_gzclose2 = gzclose(gf2);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)rc_compress;
    (void)rc_inf_init;
    (void)rc_uncompress;
    (void)rc_gzwrite;
    (void)rc_gzclose1;
    (void)readItems;
    (void)errnum;
    (void)errstr;
    (void)rc_gzclose2;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}