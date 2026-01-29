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
//<ID> 778
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, version and compress source into a buffer
    const char src[] = "zlib API sequence payload for compress, gzwrite, gzrewind, gzclearerr and inflateBackInit_";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compProduced = (uLongf)compAlloc;
    int rc_compress = compress(compBuf, &compProduced, source, sourceLen);

    // step 2: Persist the compressed buffer to a gzip file, rewind, clear errors and close
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, compBuf, (unsigned int)compProduced);
    int rc_gz_rewind = gzrewind(gf);
    gzclearerr(gf);
    int rc_gz_close = gzclose(gf);

    // step 3: Initialize an inflate-back stream with a window buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_inf_back_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));

    // step 4: Tear down inflate-back, free buffers and finish
    int rc_inf_back_end = inflateBackEnd(&istrm);
    free(compBuf);
    free(window);
    (void)rc_compress;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_close;
    (void)rc_inf_back_init;
    (void)rc_inf_back_end;
    // API sequence test completed successfully
    return 66;
}