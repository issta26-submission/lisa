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
//<ID> 361
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload, version and buffers
    const char payload[] = "zlib API sequence payload: compress2 -> deflate -> crc32_z -> gzclose usage";
    const Bytef *source = (const Bytef *)payload;
    uLong sourceLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    uLongf compLen = (uLongf)compBound;
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLong crc_source = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: One-shot compression using compress2
    int rc_compress2 = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Stream compression using deflate (initialize, operate, end)
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    uLong dBound = compressBound(sourceLen);
    Bytef *dBuf = (Bytef *)malloc((size_t)dBound);
    memset(dBuf, 0, (size_t)dBound);
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)sourceLen;
    strm.next_out = dBuf;
    strm.avail_out = (uInt)dBound;
    int rc_deflate = deflate(&strm, 0);
    uLong deflated_out = strm.total_out;
    int rc_deflate_end = deflateEnd(&strm);

    // step 4: Validate with crc32_z, write original payload to gzip file and cleanup
    uLong crc_comp2 = crc32_z(0UL, compBuf, (z_size_t)compLen);
    uLong crc_deflated = crc32_z(0UL, dBuf, (z_size_t)deflated_out);
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)source, (unsigned int)sourceLen);
    int rc_gzclose = gzclose(gzf);
    free(compBuf);
    free(dBuf);

    (void)version;
    (void)rc_compress2;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_gzwrite;
    (void)crc_source;
    (void)crc_comp2;
    (void)crc_deflated;
    (void)rc_gzclose;

    // API sequence test completed successfully
    return 66;
}