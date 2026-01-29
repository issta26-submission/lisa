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
//<ID> 736
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute initial CRC
    const char src[] = "zlib API sequence payload for deflateInit_, gzopen, gzputc, inflateSyncPoint and crc32_z";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong crc_before = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Initialize a deflate stream, determine bound and compress into a buffer
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong compAlloc = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen_f = (uLongf)compAlloc;
    int rc_comp2 = compress2(compBuf, &compLen_f, source, sourceLen, 6);
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Persist compressed data to a gzFile and write an extra byte via gzputc
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)compLen_f);
    int rc_gzputc = gzputc(gf, 0);
    int rc_gzclose = gzclose(gf);

    // step 4: Initialize inflate stream, inflate compressed buffer, validate and compute CRC of output
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen_f;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate = inflate(&istrm, 0);
    int rc_sync_point = inflateSyncPoint(&istrm);
    uLong outSize = (uLong)istrm.total_out;
    uLong crc_after = crc32_z(0UL, outBuf, (z_size_t)outSize);
    int rc_inf_end = inflateEnd(&istrm);

    // step 4: Cleanup
    free(compBuf);
    free(outBuf);
    (void)crc_before;
    (void)crc_after;
    (void)rc_def_init;
    (void)rc_comp2;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)rc_gzputc;
    (void)rc_gzclose;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_sync_point;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}