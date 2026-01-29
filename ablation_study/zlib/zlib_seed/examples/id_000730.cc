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
//<ID> 730
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute initial CRC
    const char src[] = "zlib API sequence test payload for deflateInit_, gzputc, inflateSyncPoint and crc32_z";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong crc_before = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Initialize deflate stream and perform a single deflate call
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_deflate = deflate(&dstrm, 0);
    uLong compressedSize = (uLong)dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Persist a byte of the compressed output to a gzip file using gzopen and gzputc
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzputc = gzputc(gf, (int)compBuf[0]);
    int rc_gzclose = gzclose(gf);

    // step 4: Initialize inflate stream, perform inflate, validate sync point, compute CRC and cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compressedSize;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_sync = inflateSyncPoint(&istrm);
    uLong outSize = (uLong)istrm.total_out;
    uLong crc_after = crc32_z(0UL, outBuf, (z_size_t)outSize);
    int rc_inflate_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)crc_before;
    (void)crc_after;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_gzputc;
    (void)rc_gzclose;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_sync;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}