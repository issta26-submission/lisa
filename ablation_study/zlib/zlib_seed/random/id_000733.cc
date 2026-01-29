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
//<ID> 733
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source buffer and compute CRC
    const char src[] = "zlib API sequence straight-line test payload";
    const Bytef * source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong crc_before = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Initialize deflate stream and compress into a buffer
    uLong compAlloc = sourceLen + 64;
    Bytef * compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_def_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    int rc_def = deflate(&dstrm, 0);
    uLong compSize = (uLong)dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Persist first bytes of compressed output to a gzFile using gzopen and gzputc
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzput0 = gzputc(gf, (int)compBuf[0]);
    int rc_gzput1 = gzputc(gf, (int)compBuf[1]);
    int rc_gzput2 = gzputc(gf, (int)compBuf[2]);
    int rc_gzclose = gzclose(gf);

    // step 4: Initialize inflate stream, check sync point, cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compAlloc;
    istrm.next_out = compBuf; /* reuse buffer for simplicity */
    istrm.avail_out = (uInt)compAlloc;
    int rc_inf_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    int rc_sync = inflateSyncPoint(&istrm);
    int rc_inf_end = inflateEnd(&istrm);

    free(compBuf);
    (void)crc_before;
    (void)rc_def_init;
    (void)rc_def;
    (void)compSize;
    (void)rc_def_end;
    (void)rc_gzput0;
    (void)rc_gzput1;
    (void)rc_gzput2;
    (void)rc_gzclose;
    (void)rc_inf_init;
    (void)rc_sync;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}