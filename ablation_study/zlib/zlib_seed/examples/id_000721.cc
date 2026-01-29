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
//<ID> 721
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compute CRCs for two halves, then combine CRCs
    const char src[] = "zlib API sequence test payload for inflateInit2_, crc32_combine, gzrewind and inflateEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong len1 = sourceLen / 2;
    uLong len2 = sourceLen - len1;
    const Bytef *firstPart = source;
    const Bytef *secondPart = source + len1;
    uLong crc1 = crc32(0UL, firstPart, (uInt)len1);
    uLong crc2 = crc32(0UL, secondPart, (uInt)len2);
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)len2);

    // step 2: Compress the full source into a buffer and write original data to a gzFile, then rewind it
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, source, (unsigned int)sourceLen);
    int rc_gzrewind = gzrewind(gf);
    int rc_gzclose = gzclose(gf);

    // step 3: Initialize an inflate stream with inflateInit2_, set buffers and perform inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int windowBits = 15;
    int rc_inf_init = inflateInit2_(&istrm, windowBits, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&istrm, 0);
    uLong outSize = (uLong)istrm.total_out;
    uLong crc_out = crc32(0UL, outBuf, (uInt)outSize);

    // step 4: Cleanup inflate stream and free resources
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)combined_crc;
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzrewind;
    (void)rc_gzclose;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)crc_out;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}