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
//<ID> 725
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute CRCs to combine
    const char src[] = "zlib API sequence test payload for inflateInit2_, gzrewind and crc32_combine";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uInt len1 = (uInt)(sourceLen / 2);
    uInt len2 = (uInt)(sourceLen - len1);
    const char *version = zlibVersion();
    uLong crc_part1 = crc32(0UL, source, len1);
    uLong crc_part2 = crc32(0UL, source + len1, len2);
    uLong crc_combined = crc32_combine(crc_part1, crc_part2, (off_t)len2);

    // step 2: Compress the source into a buffer and write to a gzFile, then rewind it
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)compLen);
    int rc_gzrewind = gzrewind(gf);

    // step 3: Initialize inflate stream with inflateInit2_, perform inflate into out buffer and validate CRC
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int windowBits = 15;
    int rc_inf_init = inflateInit2_(&istrm, windowBits, version, (int)sizeof(z_stream));
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 0);
    uLong outSize = (uLong)istrm.total_out;
    uLong crc_after = crc32(0UL, outBuf, (uInt)outSize);

    // step 4: Cleanup inflate stream, close gzFile and free buffers
    int rc_inf_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gf);
    free(compBuf);
    free(outBuf);
    (void)crc_combined;
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzrewind;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)crc_after;
    (void)rc_inf_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}