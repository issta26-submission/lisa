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
//<ID> 729
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compute piecewise CRCs and compress
    const char src[] = "zlib API sequence test payload for inflateInit2_, gzrewind, inflateEnd and crc32_combine";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong split = (sourceLen > 1) ? (sourceLen / 2) : sourceLen; // single evaluation, no branching logic flow
    uLong part1 = split;
    uLong part2 = sourceLen - part1;
    uLong crc1 = crc32(0UL, source, (uInt)part1);
    uLong crc2 = crc32(0UL, source + part1, (uInt)part2);
    uLong crc_combined = crc32_combine(crc1, crc2, (off_t)part2);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Persist original data to a gzFile, rewind and read back decompressed content
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, source, (unsigned int)sourceLen);
    int rc_gzrew = gzrewind(gf);
    Bytef *gzReadBuf = (Bytef *)malloc((size_t)(sourceLen + 16));
    memset(gzReadBuf, 0, (size_t)(sourceLen + 16));
    int rc_gzread = gzread(gf, gzReadBuf, (unsigned int)sourceLen);
    int rc_gzclose = gzclose(gf);
    uLong crc_from_gz = crc32(0UL, gzReadBuf, (uInt)sourceLen);

    // step 3: Initialize inflate with custom windowBits, inflate compressed buffer into output
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int windowBits = 15; // standard zlib header decoding
    int rc_inf_init = inflateInit2_(&istrm, windowBits, version, (int)sizeof(z_stream));
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 0);
    uLong inflatedSize = (uLong)istrm.total_out;

    // step 4: Cleanup inflate state and free resources
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(gzReadBuf);
    free(outBuf);
    (void)crc1;
    (void)crc2;
    (void)crc_combined;
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzrew;
    (void)rc_gzread;
    (void)rc_gzclose;
    (void)crc_from_gz;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)inflatedSize;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}