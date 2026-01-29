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
//<ID> 734
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, compute CRC, and allocate buffers
    const char src[] = "zlib API sequence test payload for deflateInit_, gzputc, inflateSyncPoint and crc32_z";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong crc_before = crc32_z(0UL, source, (z_size_t)sourceLen);
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);

    // step 2: Initialize deflate stream and perform a single deflate call
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_call = deflate(&dstrm, 0);
    uLong compSize = dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Persist a byte of compressed output to a gzFile using gzopen and gzputc
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_put = 0;
    rc_gz_put = gzputc(gf, (compSize > 0) ? (int)compBuf[0] : 0);
    int rc_gz_close = gzclose(gf);

    // step 4: Initialize inflate stream, attempt inflate, validate sync point and cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compSize;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outAlloc;
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&istrm, 0);
    int rc_sync_point = inflateSyncPoint(&istrm);
    uLong inflatedSize = istrm.total_out;
    uLong crc_after = crc32_z(0UL, outBuf, (z_size_t)inflatedSize);
    int rc_inf_end = inflateEnd(&istrm);

    free(compBuf);
    free(outBuf);
    (void)crc_before;
    (void)crc_after;
    (void)rc_def_init;
    (void)rc_def_call;
    (void)rc_def_end;
    (void)rc_gz_put;
    (void)rc_gz_close;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_sync_point;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}