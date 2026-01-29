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
//<ID> 291
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char src_c[] = "zlib API sequence payload: compute bound -> compress -> deflateParams -> write gz offsets";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Compress the source into buffer
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 3: Initialize deflate stream and configure parameters, then write compressed data to a gz file and query offsets
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_params = deflateParams(&dstrm, 9, 0);

    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, compBuf, (unsigned int)compLen);
    off_t pos32 = gztell(gzf);
    off64_t pos64 = gzoffset64(gzf);

    // step 4: Cleanup and finalization
    int rc_def_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gzf);
    free(compBuf);

    (void)rc_comp;
    (void)compLen;
    (void)rc_def_init;
    (void)rc_def_params;
    (void)rc_gzwrite;
    (void)pos32;
    (void)pos64;
    (void)rc_def_end;
    (void)rc_gzclose;
    (void)version;
    (void)bound;
    // API sequence test completed successfully
    return 66;
}