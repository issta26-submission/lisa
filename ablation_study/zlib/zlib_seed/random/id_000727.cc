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
//<ID> 727
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compute pieces' CRCs and compress source into a buffer
    const char src[] = "zlib API sequence test payload for inflateInit2_, gzrewind, crc32_combine and inflateEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong crc_part1 = crc32(0UL, source, (uInt)sourceLen);
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream with inflateInit2_ and inflate the compressed buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int windowBits = 15;
    int rc_inf_init = inflateInit2_(&istrm, windowBits, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&istrm, 0);
    uLong outSize = (uLong)istrm.total_out;
    int rc_inf_end = inflateEnd(&istrm);

    // step 3: Persist original source to a gzFile, rewind it, read back decompressed data
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gw, source, (unsigned int)sourceLen);
    int rc_gzclose_w = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int rc_gzrewind = gzrewind(gr);
    uLong gzBufAlloc = sourceLen + 16;
    Bytef *gzBuf = (Bytef *)malloc((size_t)gzBufAlloc);
    memset(gzBuf, 0, (size_t)gzBufAlloc);
    int rc_gzread = gzread(gr, gzBuf, (unsigned int)gzBufAlloc);
    int rc_gzclose_r = gzclose(gr);

    // step 4: Compute CRCs, combine them, cleanup all resources
    uLong crc_inflated = crc32(0UL, outBuf, (uInt)outSize);
    uLong crc_gzread = crc32(0UL, gzBuf, (uInt)rc_gzread);
    uLong crc_combined = crc32_combine(crc_inflated, crc_gzread, (off_t)rc_gzread);
    free(compBuf);
    free(outBuf);
    free(gzBuf);
    (void)crc_part1;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzrewind;
    (void)rc_gzread;
    (void)rc_gzclose_r;
    (void)crc_inflated;
    (void)crc_gzread;
    (void)crc_combined;
    // API sequence test completed successfully
    return 66;
}