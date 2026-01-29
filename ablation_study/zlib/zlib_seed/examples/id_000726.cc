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
//<ID> 726
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, compute initial CRC and compress the source into a buffer
    const char src[] = "zlib API sequence test payload for inflateInit2_, gzrewind, crc32_combine and inflateEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong crc_source = crc32_z(0UL, source, (z_size_t)sourceLen);
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Persist original data to a gzFile, then reopen and use gzrewind/gzread to obtain the stored content and its CRC
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, source, (unsigned int)sourceLen);
    int rc_gzclose1 = gzclose(gf);
    gf = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int rc_gzrewind = gzrewind(gf);
    Bytef *gzOutBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(gzOutBuf, 0, (size_t)sourceLen);
    int rc_gzread = gzread(gf, gzOutBuf, (unsigned int)sourceLen);
    uLong crc_gzread = crc32_z(0UL, gzOutBuf, (z_size_t)rc_gzread);
    int rc_gzclose2 = gzclose(gf);

    // step 3: Initialize an inflate stream with inflateInit2_, perform inflate on the compressed buffer and compute CRC of inflate output
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int windowBits = 15; // standard zlib header/windowBits
    int rc_inf_init2 = inflateInit2_(&istrm, windowBits, version, (int)sizeof(z_stream));
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 0);
    uLong outSize = (uLong)istrm.total_out;
    uLong crc_inflate = crc32_z(0UL, outBuf, (z_size_t)outSize);

    // step 4: Combine CRCs meaningfully, cleanup inflate stream and free resources
    uLong combined_crc = crc32_combine(crc_gzread, crc_inflate, (off_t)outSize);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(gzOutBuf);
    free(outBuf);
    (void)crc_source;
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzclose1;
    (void)rc_gzrewind;
    (void)rc_gzread;
    (void)rc_gzclose2;
    (void)rc_inf_init2;
    (void)rc_inflate;
    (void)combined_crc;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}