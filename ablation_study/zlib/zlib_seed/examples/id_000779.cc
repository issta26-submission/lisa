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
//<ID> 779
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, version and allocate buffers
    const char src[] = "zlib API sequence payload for compress, gzwrite, gzrewind, inflateBackInit_ and inflateBackEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compProduced = (uLongf)compAlloc;
    int rc_compress = compress(compBuf, &compProduced, source, sourceLen);

    // step 2: Persist original source to a gz file, rewind, clear error and close
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, source, (unsigned int)sourceLen);
    int rc_gz_rewind = gzrewind(gf);
    gzclearerr(gf);
    int rc_gz_close = gzclose(gf);

    // step 3: Initialize inflateBack with a window and perform initialization and cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    Bytef *outBuf = (Bytef *)malloc((size_t)(sourceLen + 64));
    memset(outBuf, 0, (size_t)(sourceLen + 64));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compProduced;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)(sourceLen + 64);
    unsigned int windowBits = 15;
    size_t windowSize = (size_t)1 << windowBits;
    unsigned char *window = (unsigned char *)malloc(windowSize);
    memset(window, 0, windowSize);
    int rc_infback_init = inflateBackInit_(&istrm, (int)windowBits, window, version, (int)sizeof(z_stream));
    int rc_infback_end = inflateBackEnd(&istrm);

    // step 4: Cleanup
    free(compBuf);
    free(outBuf);
    free(window);
    (void)rc_compress;
    (void)compProduced;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_close;
    (void)rc_infback_init;
    (void)rc_infback_end;
    // API sequence test completed successfully
    return 66;
}