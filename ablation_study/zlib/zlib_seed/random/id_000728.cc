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
//<ID> 728
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compute CRC parts and compress into a buffer
    const char src[] = "zlib API sequence test payload for inflateInit2_, gzrewind, crc32_combine and inflateEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uInt half = (uInt)(sourceLen / 2);
    uLong crc_part1 = crc32(0UL, source, half);
    uLong crc_part2 = crc32(0UL, source + half, (uInt)(sourceLen - half));
    uLong crc_combined = crc32_combine(crc_part1, crc_part2, (off_t)(sourceLen - half));
    uLong crc_total = crc32(0UL, source, (uInt)sourceLen);
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Persist compressed buffer to a gzFile, then reopen and rewind to read it back
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gw, compBuf, (unsigned int)compLen);
    int rc_gzclose_w = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int rc_gzrewind = gzrewind(gr);
    Bytef *readBuf = (Bytef *)malloc((size_t)compLen);
    memset(readBuf, 0, (size_t)compLen);
    int rc_gzread = gzread(gr, readBuf, (unsigned int)compLen);
    int rc_gzclose_r = gzclose(gr);

    // step 3: Initialize inflate stream with inflateInit2_, decompress the read buffer and compute CRC of output
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    istrm.next_in = readBuf;
    istrm.avail_in = (uInt)compLen;
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&istrm, 0);
    uLong outSize = (uLong)istrm.total_out;
    uLong crc_after = crc32(0UL, outBuf, (uInt)outSize);

    // step 4: Cleanup inflate stream and free buffers
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(readBuf);
    free(outBuf);
    (void)crc_part1;
    (void)crc_part2;
    (void)crc_combined;
    (void)crc_total;
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzrewind;
    (void)rc_gzread;
    (void)rc_gzclose_r;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)crc_after;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}