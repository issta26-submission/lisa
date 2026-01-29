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
//<ID> 294
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and source initialization
    const char src[] = "zlib API sequence payload: compress -> deflateParams -> gz write -> offsets";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();

    // step 2: Compute compression bound and compress the source into a buffer
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 3: Initialize a deflate stream, tune parameters, and clean it up
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_params = deflateParams(&dstrm, 5, 1);
    int rc_def_end = deflateEnd(&dstrm);

    // step 4: Write original data to a gz file, obtain both 32/64-bit offsets, then cleanup
    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, source, (unsigned int)sourceLen);
    int rc_gzflush = gzflush(gzf, 2);
    off_t pos32 = gztell(gzf);
    off64_t pos64 = gzoffset64(gzf);
    int rc_gzclose = gzclose(gzf);

    free(compBuf);

    (void)rc_comp;
    (void)compLen;
    (void)compBound;
    (void)rc_def_init;
    (void)rc_def_params;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)pos32;
    (void)pos64;
    (void)rc_gzclose;
    (void)version;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}