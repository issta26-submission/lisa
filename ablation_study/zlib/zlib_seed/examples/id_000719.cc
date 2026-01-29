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
//<ID> 719
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compute CRC, and compress into a buffer
    const char src[] = "zlib API sequence test payload for inflate, inflateValidate, gzopen and crc32_z";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong crc_before = crc32_z(0UL, source, (z_size_t)sourceLen);
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Persist compressed data to a gzFile
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)compLen);
    int rc_gzclose = gzclose(gf);

    // step 3: Initialize inflate stream, perform inflate, validate and compute CRC of output
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&istrm, 0);
    uLong outSize = (uLong)istrm.total_out;
    uLong crc_after = crc32_z(0UL, outBuf, (z_size_t)outSize);
    int rc_validate = inflateValidate(&istrm, 0);

    // step 4: Cleanup all resources
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)crc_before;
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)crc_after;
    (void)rc_validate;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}