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
//<ID> 295
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare source, compute bound and compress
    const char src[] = "zlib API sequence payload: demonstrate compress -> write -> stream params -> tell";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize a deflate stream and adjust parameters with deflateParams
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_params = deflateParams(&dstrm, 5, 0);

    // step 3: Write compressed buffer to a gz file, then query positions with gztell and gzoffset64
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int gz_written = gzwrite(gzf, compBuf, (unsigned int)compLen);
    off_t pos = gztell(gzf);
    off64_t pos64 = gzoffset64(gzf);

    // step 4: Cleanup streams, file and memory
    int rc_def_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gzf);
    free(compBuf);

    (void)rc_comp;
    (void)compBound;
    (void)compLen;
    (void)rc_def_init;
    (void)rc_def_params;
    (void)gz_written;
    (void)pos;
    (void)pos64;
    (void)rc_def_end;
    (void)rc_gzclose;
    (void)version;

    // API sequence test completed successfully
    return 66;
}