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
//<ID> 630
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into a buffer
    const char src[] = "zlib API sequence test payload for uncompress2 and gzfread";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLongf compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compDestLen = compAlloc;
    int rc_compress = compress2(compBuf, &compDestLen, source, sourceLen, 6);

    // step 2: Initialize an inflate stream with inflateInit2_
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));

    // step 3: Decompress using uncompress2 into an output buffer
    uLongf outAlloc = (uLongf)(sourceLen + 64);
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    uLongf destLen_uLongf = outAlloc;
    uLong srcLenForUncompress = (uLong)compDestLen;
    int rc_uncompress2 = uncompress2(outBuf, &destLen_uLongf, compBuf, &srcLenForUncompress);

    // step 4: Persist compressed bytes to a gzFile, read them back with gzfread, query gzerror, and cleanup
    gzFile gf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf_w, compBuf, (unsigned int)compDestLen);
    int rc_gzclose_w = gzclose(gf_w);
    gzFile gf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    z_size_t items_read = gzfread((voidp)compBuf, (z_size_t)1, (z_size_t)compDestLen, gf_r);
    int errnum = 0;
    const char *errstr = gzerror(gf_r, &errnum);
    int rc_gzclose_r = gzclose(gf_r);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)rc_compress;
    (void)rc_inf_init2;
    (void)rc_uncompress2;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)items_read;
    (void)errstr;
    (void)errnum;
    (void)rc_gzclose_r;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}