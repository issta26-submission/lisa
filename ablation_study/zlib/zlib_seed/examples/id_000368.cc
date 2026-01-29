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
//<ID> 368
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload + version
    const char payload[] = "zlib API sequence payload: compress2 -> crc32_z -> deflate -> gzclose usage";
    const Bytef *source = (const Bytef *)payload;
    uLong sourceLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();

    // step 2: Compress using compress2 and compute CRC of compressed output
    uLong compBound = compressBound(sourceLen);
    uLongf destLen = (uLongf)compBound;
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    int rc_compress = compress2(compBuf, &destLen, source, sourceLen, 6);
    uLong compressed_crc = crc32_z(0UL, compBuf, (z_size_t)destLen);

    // step 3: Initialize a deflate stream, perform a deflate call into a separate buffer
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    Bytef *streamOut = (Bytef *)malloc((size_t)compBound);
    memset(streamOut, 0, (size_t)compBound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = streamOut;
    dstrm.avail_out = (uInt)compBound;
    int rc_deflate = deflate(&dstrm, 0);

    // step 4: Write compressed data to a gzip file and cleanup resources
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)destLen);
    int rc_gzclose = gzclose(gzf);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(streamOut);

    (void)version;
    (void)rc_compress;
    (void)compressed_crc;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_def_end;

    // API sequence test completed successfully
    return 66;
}