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
//<ID> 776
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data, version and allocate buffers
    const char src[] = "zlib API sequence payload for compress, gzwrite, gzrewind and inflateBackInit_";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compProduced = (uLongf)compAlloc;
    int rc_compress = compress(compBuf, &compProduced, source, sourceLen);

    // step 2: Persist original data to a gz file, rewind and clear errors
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, source, (unsigned int)sourceLen);
    int rc_gz_rewind = gzrewind(gf);
    gzclearerr(gf);

    // step 3: Initialize an inflateBack stream with a sliding window and then teardown
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_inf_back_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    int rc_inf_back_end = inflateBackEnd(&istrm);

    // step 4: Cleanup resources and close gz file
    int rc_gz_close = gzclose(gf);
    free(compBuf);
    free(window);

    (void)rc_compress;
    (void)compProduced;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_inf_back_init;
    (void)rc_inf_back_end;
    (void)rc_gz_close;
    // API sequence test completed successfully
    return 66;
}