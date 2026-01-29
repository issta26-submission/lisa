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
//<ID> 771
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data, version and allocate buffers
    const char src[] = "zlib API sequence payload for compress2, uncompress, inflateBackInit_, gzwrite and gzrewind";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compProduced = (uLongf)compAlloc;
    int rc_compress = compress2(compBuf, &compProduced, source, sourceLen, 6);

    // step 2: Persist original data via gzopen, write, rewind, clear errors and close
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, source, (unsigned int)sourceLen);
    int rc_gz_rewind = gzrewind(gf);
    gzclearerr(gf);
    int rc_gz_close = gzclose(gf);

    // step 3: Decompress to verify compressed data and initialize inflateBack state
    Bytef *outBuf = (Bytef *)malloc((size_t)(sourceLen + 16));
    memset(outBuf, 0, (size_t)(sourceLen + 16));
    uLongf outLen = (uLongf)(sourceLen + 16);
    int rc_uncompress = uncompress(outBuf, &outLen, compBuf, (uLong)compProduced);

    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window, 0, (size_t)(1 << 15));
    int rc_infback_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    int rc_infback_end = inflateBackEnd(&istrm);

    // step 4: Cleanup
    free(compBuf);
    free(outBuf);
    free(window);
    (void)rc_compress;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_close;
    (void)rc_uncompress;
    (void)outLen;
    (void)rc_infback_init;
    (void)rc_infback_end;
    // API sequence test completed successfully
    return 66;
}