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
//<ID> 739
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute CRC
    const char src[] = "zlib API sequence test payload for deflateInit_, gzopen, gzputc, inflateSyncPoint and crc32_z";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong crc_before = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Initialize and configure a deflate stream, allocate output buffer and perform one deflate call
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_deflate = deflate(&dstrm, 0);
    uLong compProduced = (uLong)dstrm.total_out;

    // step 3: Open a gz file and write a few bytes using gzputc, then close it
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_put0 = gzputc(gf, (int)src[0]);
    int rc_gz_put1 = gzputc(gf, (int)src[1]);
    int rc_gz_put2 = gzputc(gf, (int)src[2]);
    int rc_gz_close = gzclose(gf);

    // step 4: Initialize an inflate stream to validate state with inflateSyncPoint, then cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_sync_point = inflateSyncPoint(&istrm);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    free(compBuf);
    (void)crc_before;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)compProduced;
    (void)rc_gz_put0;
    (void)rc_gz_put1;
    (void)rc_gz_put2;
    (void)rc_gz_close;
    (void)rc_inflate_init;
    (void)rc_sync_point;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}