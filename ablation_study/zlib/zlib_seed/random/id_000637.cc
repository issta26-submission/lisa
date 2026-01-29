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
//<ID> 637
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compress into a buffer
    const char src[] = "zlib API sequence test payload for gzfread and uncompress2";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLongf compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compDestLen = compAlloc;
    int rc_compress2 = compress2(compBuf, &compDestLen, source, sourceLen, 6);

    // step 2: Persist compressed bytes to a gzFile and read them back with gzfread
    gzFile gf_write = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf_write, compBuf, (unsigned int)compDestLen);
    int rc_gzclose_write = gzclose(gf_write);
    gzFile gf_read = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)compDestLen);
    memset(readBuf, 0, (size_t)compDestLen);
    z_size_t items_read = gzfread((voidp)readBuf, (z_size_t)1, (z_size_t)compDestLen, gf_read);
    int gz_errnum = 0;
    const char *gz_errstr = gzerror(gf_read, &gz_errnum);
    int rc_gzclose_read = gzclose(gf_read);

    // step 3: Initialize an inflate stream (inflateInit2_) and one-shot uncompress2 from read buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inflate_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    uLong srcReadLen = (uLong)items_read;
    uLongf outAlloc = (uLongf)(sourceLen + 64);
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    uLongf outLen = outAlloc;
    int rc_uncompress2 = uncompress2(outBuf, &outLen, (const Bytef *)readBuf, &srcReadLen);

    // step 4: Validate & Cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    free(compBuf);
    free(readBuf);
    free(outBuf);
    (void)rc_compress2;
    (void)rc_gzwrite;
    (void)rc_gzclose_write;
    (void)items_read;
    (void)gz_errnum;
    (void)gz_errstr;
    (void)rc_gzclose_read;
    (void)rc_inflate_init2;
    (void)rc_uncompress2;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}