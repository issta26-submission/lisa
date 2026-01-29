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
//<ID> 768
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and allocate buffers
    const char src[] = "zlib API sequence payload: compress, gzwrite, gzseek, gzclose, inflateValidate";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;

    // step 2: Compress the source into compBuf
    int rc_compress = compress(compBuf, &compLen, source, sourceLen);

    // step 3: Persist the compressed data to a gz file, then seek within it and close
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, compBuf, (unsigned int)compLen);
    off_t rc_gz_seek = gzseek(gf, 0, 0);
    int rc_gz_close = gzclose(gf);

    // step 4: Initialize an inflate stream, validate it, then cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inf_validate = inflateValidate(&istrm, 1);
    int rc_inf_end = inflateEnd(&istrm);

    free(compBuf);
    (void)rc_compress;
    (void)rc_gz_write;
    (void)rc_gz_seek;
    (void)rc_gz_close;
    (void)rc_inf_init;
    (void)rc_inf_validate;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}