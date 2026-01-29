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
//<ID> 364
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload
    const char src[] = "zlib API sequence payload: compress2 -> crc32_z -> gzwrite/gzclose -> deflate";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(sourceLen);
    uLongf destLen = (uLongf)compBound;
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLong crc_src = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Compress with compress2 and write compressed data to a gzip file, then close it
    int rc_comp = compress2(compBuf, &destLen, source, sourceLen, 6);
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf, compBuf, (unsigned int)destLen);
    int rc_gzclose = gzclose(gzf);

    // step 3: Initialize a deflate stream, perform a streaming deflate of the original source, then end it
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    uLong outBound = compressBound(sourceLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_deflate = deflate(&dstrm, 0);
    uLong def_out_total = dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 4: Validation and cleanup
    uLong crc_comp = crc32_z(0UL, compBuf, (z_size_t)destLen);
    (void)version;
    (void)crc_src;
    (void)crc_comp;
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)def_out_total;
    (void)rc_def_end;
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    return 66;
}