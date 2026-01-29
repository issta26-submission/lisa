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
//<ID> 738
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute CRC
    const char src[] = "zlib API sequence straight-line test payload";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong crc = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Initialize deflate stream and allocate output buffer
    z_stream defl;
    memset(&defl, 0, sizeof(z_stream));
    uLong outAlloc = sourceLen + 64;
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    defl.next_in = (Bytef *)source;
    defl.avail_in = (uInt)sourceLen;
    defl.next_out = outBuf;
    defl.avail_out = (uInt)outAlloc;
    int rc_def_init = deflateInit_(&defl, 6, zlibVersion(), (int)sizeof(z_stream));
    int rc_def_end = deflateEnd(&defl);

    // step 3: Open a gzFile and write a byte derived from the CRC
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int write_byte = (int)(crc & 0xFF);
    int rc_gzput = gzputc(gf, write_byte);
    int rc_gzclose = gzclose(gf);

    // step 4: Initialize inflate stream, check sync point, and cleanup
    z_stream instrm;
    memset(&instrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&instrm, zlibVersion(), (int)sizeof(z_stream));
    int rc_sync = inflateSyncPoint(&instrm);
    int rc_inf_end = inflateEnd(&instrm);
    free(outBuf);

    (void)crc;
    (void)rc_def_init;
    (void)rc_def_end;
    (void)rc_gzput;
    (void)rc_gzclose;
    (void)rc_inf_init;
    (void)rc_sync;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}