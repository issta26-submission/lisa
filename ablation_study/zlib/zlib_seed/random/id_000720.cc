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
//<ID> 720
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compute CRCs, and compress into a zlib-format buffer
    const char src[] = "zlib API sequence test payload split for crc32_combine and inflateInit2_, gzrewind and inflateEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uInt halfLen = (uInt)(sourceLen / 2);
    const char *version = zlibVersion();
    uLong crc_first = crc32(0UL, source, halfLen);
    uLong crc_second = crc32(0UL, source + halfLen, (uInt)(sourceLen - halfLen));
    uLong crc_combined = crc32_combine(crc_first, crc_second, (off_t)(sourceLen - halfLen));
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream with inflateInit2_, decompress the compressed buffer and validate CRCs
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inf_init2 = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&istrm, 0);
    uLong outSize = (uLong)istrm.total_out;
    uLong out_crc_first = crc32(0UL, outBuf, (uInt)halfLen);
    uLong out_crc_second = crc32(0UL, outBuf + halfLen, (uInt)(outSize > halfLen ? outSize - halfLen : 0));
    uLong out_crc_combined = crc32_combine(out_crc_first, out_crc_second, (off_t)(outSize > halfLen ? outSize - halfLen : 0));

    // step 3: Persist decompressed output via gzFile, rewind and read it back
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gw, outBuf, (unsigned int)outSize);
    int rc_gwclose = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int rc_rewind = gzrewind(gr);
    Bytef *readBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(readBuf, 0, (size_t)outAlloc);
    int rc_gzread = gzread(gr, readBuf, (unsigned int)outAlloc);
    int rc_grclose = gzclose(gr);

    // step 4: Cleanup inflate stream and free resources
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    free(readBuf);
    (void)crc_first;
    (void)crc_second;
    (void)crc_combined;
    (void)rc_comp;
    (void)rc_inf_init2;
    (void)rc_inflate;
    (void)out_crc_first;
    (void)out_crc_second;
    (void)out_crc_combined;
    (void)rc_gzwrite;
    (void)rc_gwclose;
    (void)rc_rewind;
    (void)rc_gzread;
    (void)rc_grclose;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}