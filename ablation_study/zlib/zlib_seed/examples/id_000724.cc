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
//<ID> 724
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compress into a buffer
    const char src[] = "zlib API sequence test payload for inflateInit2_, gzrewind, crc32_combine and inflateEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Persist compressed data to a gzFile, then reopen and rewind for reading
    gzFile gf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf_w, compBuf, (unsigned int)compLen);
    int rc_gzclose_w = gzclose(gf_w);
    gzFile gf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int rc_gzrewind = gzrewind(gf_r);
    Bytef *readBuf = (Bytef *)malloc((size_t)compLen);
    memset(readBuf, 0, (size_t)compLen);
    int bytes_read = gzread(gf_r, readBuf, (unsigned int)compLen);
    uLong readLen = (bytes_read > 0) ? (uLong)bytes_read : 0;

    // step 3: Initialize inflate with custom windowBits, inflate the data, compute CRCs and combine
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int windowBits = 15;
    int rc_inf_init = inflateInit2_(&istrm, windowBits, version, (int)sizeof(z_stream));
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = readBuf;
    istrm.avail_in = (uInt)readLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 0);
    uLong outSize = (uLong)istrm.total_out;
    uLong half = outSize / 2;
    uLong part1_len = half;
    uLong part2_len = outSize - half;
    uLong crc1 = crc32_z(0UL, outBuf, (z_size_t)part1_len);
    uLong crc2 = crc32_z(0UL, outBuf + part1_len, (z_size_t)part2_len);
    uLong crc_combined = crc32_combine(crc1, crc2, (off_t)part2_len);

    // step 4: Cleanup inflate, gzFile and free buffers
    int rc_inf_end = inflateEnd(&istrm);
    int rc_gzclose_r = gzclose(gf_r);
    free(compBuf);
    free(readBuf);
    free(outBuf);
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzrewind;
    (void)bytes_read;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)crc1;
    (void)crc2;
    (void)crc_combined;
    (void)rc_inf_end;
    (void)rc_gzclose_r;
    // API sequence test completed successfully
    return 66;
}