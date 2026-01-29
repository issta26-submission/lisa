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
//<ID> 773
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
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window, 0, (size_t)(1 << 15));

    // step 2: Compress the source into compBuf
    uLongf compProduced = (uLongf)compAlloc;
    int rc_compress = compress2(compBuf, &compProduced, source, sourceLen, 6);

    // step 3: Persist original source to a gz file, rewind, clear errors and close
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, source, (unsigned int)sourceLen);
    off_t rc_gz_rewind = gzrewind(gf);
    gzclearerr(gf);
    int rc_gz_close = gzclose(gf);

    // step 4: Initialize inflate-back, associate compressed data and cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_back_init = inflateBackInit_(&istrm, 15, window, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compProduced;
    int rc_inf_back_end = inflateBackEnd(&istrm);

    free(compBuf);
    free(window);
    (void)rc_compress;
    (void)compProduced;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_close;
    (void)rc_inf_back_init;
    (void)rc_inf_back_end;
    // API sequence test completed successfully
    return 66;
}