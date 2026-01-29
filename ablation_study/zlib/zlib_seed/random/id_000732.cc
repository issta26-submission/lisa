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
//<ID> 732
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute initial CRC
    const char src[] = "zlib API sequence payload for deflateInit_, gzputc, inflateSyncPoint and crc32_z";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong crc_before = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Initialize deflate stream and compress into a buffer
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def = deflate(&dstrm, 0);
    uLong compLen = (uLong)dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Persist a few compressed bytes to a gzFile, then initialize inflate and decompress
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_put0 = gzputc(gf, (int)compBuf[0]);
    int rc_put1 = gzputc(gf, (int)compBuf[1]);
    int rc_put2 = gzputc(gf, (int)compBuf[2]);
    int rc_gzclose = gzclose(gf);

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
    int rc_sync = inflateSyncPoint(&istrm);
    uLong outSize = (uLong)istrm.total_out;
    uLong crc_after = crc32_z(0UL, outBuf, (z_size_t)outSize);
    int rc_inf_end = inflateEnd(&istrm);

    // step 4: Cleanup resources
    free(compBuf);
    free(outBuf);
    (void)crc_before;
    (void)crc_after;
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_def_end;
    (void)rc_put0;
    (void)rc_put1;
    (void)rc_put2;
    (void)rc_gzclose;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_sync;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}