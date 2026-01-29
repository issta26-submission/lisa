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
//<ID> 632
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compress it, and allocate buffers
    const char src[] = "zlib API sequence test payload for combined APIs and gz I/O";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compDestLen = (uLongf)compAlloc;
    int rc_compress = compress2(compBuf, &compDestLen, source, sourceLen, 6);

    // step 2: One-shot uncompress using uncompress2 and initialize an inflate stream with inflateInit2_
    uLongf destAlloc = (uLongf)(sourceLen + 64);
    Bytef *destBuf = (Bytef *)malloc((size_t)destAlloc);
    memset(destBuf, 0, (size_t)destAlloc);
    uLong compSrcLenUL = (uLong)compDestLen;
    int rc_uncompress2 = uncompress2(destBuf, &destAlloc, compBuf, &compSrcLenUL);
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inflate_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compSrcLenUL;
    uLongf altOutAlloc = (uLongf)(sourceLen + 32);
    Bytef *altOutBuf = (Bytef *)malloc((size_t)altOutAlloc);
    memset(altOutBuf, 0, (size_t)altOutAlloc);
    istrm.next_out = altOutBuf;
    istrm.avail_out = (uInt)altOutAlloc;
    int rc_inflate = inflate(&istrm, 4); /* Z_FINISH == 4 */
    int rc_inflate_end = inflateEnd(&istrm);

    // step 3: Persist original data via gzFile, read it back with gzfread and query error string
    gzFile gfw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gfw, source, (unsigned int)sourceLen);
    int rc_gzclose_w = gzclose(gfw);
    gzFile gfr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    uLongf readAlloc = (uLongf)(sourceLen + 32);
    Bytef *readBuf = (Bytef *)malloc((size_t)readAlloc);
    memset(readBuf, 0, (size_t)readAlloc);
    z_size_t items_read = gzfread(readBuf, (z_size_t)1, (z_size_t)sourceLen, gfr);
    int gz_errnum = 0;
    const char *gz_errstr = gzerror(gfr, &gz_errnum);
    int rc_gzclose_r = gzclose(gfr);

    // step 4: Cleanup
    free(compBuf);
    free(destBuf);
    free(altOutBuf);
    free(readBuf);
    (void)rc_compress;
    (void)rc_uncompress2;
    (void)rc_inflate_init2;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)items_read;
    (void)gz_errnum;
    (void)gz_errstr;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}