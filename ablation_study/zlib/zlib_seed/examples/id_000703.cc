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
//<ID> 703
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and initialize a deflate stream
    const char src[] = "zlib API sequence test payload for inflateBackInit_, deflateBound, gz I/O and cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Compute compression bound from the deflate stream and compress into a buffer
    uLong compAlloc = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Initialize an inflateBack stream with a window, write compressed data to a gzFile, then close it
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int windowBits = 15;
    size_t winSize = ((size_t)1) << windowBits;
    unsigned char *window = (unsigned char *)malloc(winSize);
    memset(window, 0, winSize);
    int rc_ibinit = inflateBackInit_(&istrm, windowBits, window, version, (int)sizeof(z_stream));
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)compLen);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup inflate and deflate streams and free buffers
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(window);
    (void)rc_def_init;
    (void)compAlloc;
    (void)rc_comp;
    (void)rc_ibinit;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_inf_end;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}