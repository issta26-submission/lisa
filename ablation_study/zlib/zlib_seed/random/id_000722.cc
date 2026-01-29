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
//<ID> 722
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compute partial CRCs and compress into a buffer
    const char src[] = "zlib API sequence test payload demonstrating inflateInit2_, crc32_combine, gzrewind and inflateEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uInt halfLen = (uInt)(sourceLen / 2);
    uLong crc_part1 = crc32(0UL, source, halfLen);
    uLong crc_part2 = crc32(0UL, source + halfLen, (uInt)(sourceLen - halfLen));
    uLong crc_combined = crc32_combine(crc_part1, crc_part2, (off_t)(sourceLen - halfLen));
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream with inflateInit2_ and decompress the compressed buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&istrm, 0);
    uLong decompressedSize = (uLong)istrm.total_out;

    // step 3: Persist original source into a gzFile, close and reopen for reading, then read it back
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, source, (unsigned int)sourceLen);
    int rc_gzclose = gzclose(gf);
    gzFile gf2 = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(readBuf, 0, (size_t)sourceLen);
    int rc_gzread = gzread(gf2, readBuf, (unsigned int)sourceLen);
    uLong crc_read = crc32(0UL, readBuf, (uInt)rc_gzread);
    int rc_gzclose2 = gzclose(gf2);

    // step 4: Cleanup inflate and free buffers
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    free(readBuf);
    (void)crc_part1;
    (void)crc_part2;
    (void)crc_combined;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)decompressedSize;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_gzread;
    (void)crc_read;
    (void)rc_gzclose2;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}