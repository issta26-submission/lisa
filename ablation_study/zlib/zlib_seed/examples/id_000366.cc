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
//<ID> 366
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload, version, and checksums
    const char payload[] = "zlib API sequence payload: compress2 -> deflate -> gzwrite with crc checks";
    const Bytef *source = (const Bytef *)payload;
    uLong sourceLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLongf compBufLen = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufLen);
    memset(compBuf, 0, (size_t)compBufLen);
    uLong crc_source = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Compress the source with compress2
    int rc_comp = compress2(compBuf, &compBufLen, source, sourceLen, 6);

    // step 3: Initialize deflate stream, deflate the compressed data into an output buffer
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compBufLen;
    uLong outBound = deflateBound(&strm, (uLong)compBufLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    strm.next_out = outBuf;
    strm.avail_out = (uInt)outBound;
    int rc_deflate = deflate(&strm, Z_FINISH);
    uLong finalOut = strm.total_out;
    int rc_def_end = deflateEnd(&strm);

    // step 4: Write deflated output to a gzip file, compute crc of output, cleanup
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)outBuf, (unsigned int)finalOut);
    int rc_gzclose = gzclose(gzf);
    uLong crc_out = crc32_z(0UL, outBuf, (z_size_t)finalOut);
    free(compBuf);
    free(outBuf);

    (void)version;
    (void)crc_source;
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)crc_out;

    // API sequence test completed successfully
    return 66;
}