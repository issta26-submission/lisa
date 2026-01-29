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
//<ID> 635
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and buffers
    const char src[] = "zlib API sequence test payload for read/write and decompress";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLongf compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compDestLen = compAlloc;
    int rc_compress = compress2(compBuf, &compDestLen, source, sourceLen, 6);
    const char *version = zlibVersion();

    // step 2: Initialize an inflate stream with custom window bits
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compDestLen;
    uLongf outAlloc = (uLongf)(sourceLen + 128);
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;

    // step 3: One-shot uncompress2 and persist/read compressed bytes via gzFile
    uLong compLenForUn = (uLong)compDestLen;
    uLongf unDestLen = outAlloc;
    int rc_un2 = uncompress2(outBuf, &unDestLen, compBuf, &compLenForUn);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)compDestLen);
    int rc_gzclose_w = gzclose(gf);
    gzFile gf2 = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    z_size_t items_read = gzfread((voidp)outBuf, (z_size_t)1, (z_size_t)compDestLen, gf2);
    int errnum = 0;
    const char *errstr = gzerror(gf2, &errnum);

    // step 4: Validate & Cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose_r = gzclose(gf2);
    free(compBuf);
    free(outBuf);
    (void)rc_compress;
    (void)rc_inf_init2;
    (void)rc_un2;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)items_read;
    (void)errnum;
    (void)errstr;
    (void)rc_inflate_end;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}