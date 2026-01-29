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
//<ID> 735
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute CRC
    const char src[] = "zlib API sequence payload for deflateInit_, gzputc, inflateSyncPoint and crc32_z";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong crc_before = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Initialize deflate stream and compress source into a buffer
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_deflate = deflate(&dstrm, 4); // Z_FINISH == 4
    uLong compLen = dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Persist some compressed bytes into a gz file using gzopen and gzputc
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzpc1 = gzputc(gf, (int)compBuf[0]);
    int rc_gzpc2 = gzputc(gf, (int)compBuf[(compLen > 1) ? 1 : 0]);
    int rc_gzclose = gzclose(gf);

    // step 4: Initialize inflate, perform inflate, validate sync point, compute CRC of output and cleanup
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
    uLong inflatedSize = (uLong)istrm.total_out;
    uLong crc_after = crc32_z(0UL, outBuf, (z_size_t)inflatedSize);
    int rc_inf_end = inflateEnd(&istrm);

    // cleanup
    free(compBuf);
    free(outBuf);
    (void)crc_before;
    (void)crc_after;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_gzpc1;
    (void)rc_gzpc2;
    (void)rc_gzclose;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_sync;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}