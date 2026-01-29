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
//<ID> 770
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data, version and allocate buffers
    const char src[] = "zlib API sequence payload for gzwrite, gzrewind, gzclearerr and inflateBackInit_";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compProduced = (uLongf)compAlloc;
    int rc_compress = compress2(compBuf, &compProduced, source, sourceLen, 6);

    // step 2: Persist original data to a gz file, rewind and read it back
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, source, (unsigned int)sourceLen);
    int rc_gz_rewind = gzrewind(gf);
    uLong readAlloc = sourceLen + 16;
    Bytef *readBuf = (Bytef *)malloc((size_t)readAlloc);
    memset(readBuf, 0, (size_t)readAlloc);
    int rc_gz_read = gzread(gf, readBuf, (unsigned int)readAlloc);
    gzclearerr(gf);

    // step 3: Initialize inflate-back with a window buffer and then teardown
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(z_stream));
    int windowBits = 15;
    unsigned int windowSize = 1u << windowBits;
    unsigned char *window = (unsigned char *)malloc((size_t)windowSize);
    memset(window, 0, (size_t)windowSize);
    int rc_back_init = inflateBackInit_(&bstrm, windowBits, window, version, (int)sizeof(z_stream));
    int rc_back_end = inflateBackEnd(&bstrm);

    // step 4: Cleanup resources and close gz file
    int rc_gz_close = gzclose(gf);
    free(compBuf);
    free(readBuf);
    free(window);
    (void)rc_compress;
    (void)compProduced;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_read;
    (void)rc_back_init;
    (void)rc_back_end;
    (void)rc_gz_close;
    // API sequence test completed successfully
    return 66;
}