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
//<ID> 213
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize source and buffers
    const char src_c[] = "zlib API sequence payload: version -> deflateInit_ -> compress2 -> inflateInit_ -> gzwrite -> cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Initialize deflate stream and compress using compress2
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Initialize inflate stream, write compressed data to a gzip file, then close
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(z_stream));
    int rc_infl_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, (voidpc)compBuf, (unsigned int)compLen);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup - end deflate and inflate streams, free buffers
    int rc_def_end = deflateEnd(&dstrm);
    int rc_infl_end = inflateEnd(&istrm);
    free(compBuf);

    (void)rc_def_init;
    (void)rc_comp2;
    (void)rc_infl_init;
    (void)rc_gz_write;
    (void)rc_gz_close;
    (void)rc_def_end;
    (void)rc_infl_end;
    (void)version;
    (void)sourceLen;
    // API sequence test completed successfully
    return 66;
}