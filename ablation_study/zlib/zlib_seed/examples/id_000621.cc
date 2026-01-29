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
//<ID> 621
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into a temporary buffer
    const char src[] = "zlib API sequence test payload for combined APIs";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    uLongf destLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    int rc_compress = compress(compBuf, &destLen, source, sourceLen);

    // step 2: Initialize streams and copy deflate state
    z_stream dstrm;
    z_stream copy_strm;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(z_stream));
    memset(&copy_strm, 0, sizeof(z_stream));
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_defcopy = deflateCopy(&copy_strm, &dstrm);

    // step 3: Operate — write compressed bytes to a gzFile and query EOF state
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)compBuf, (unsigned int)destLen);
    int rc_gzeof = gzeof(gf);

    // step 4: Cleanup and validate teardown
    int rc_def_end1 = deflateEnd(&dstrm);
    int rc_def_end2 = deflateEnd(&copy_strm);
    int rc_inf_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gf);
    free(compBuf);
    (void)rc_compress;
    (void)rc_def_init;
    (void)rc_inf_init;
    (void)rc_defcopy;
    (void)rc_gzwrite;
    (void)rc_gzeof;
    (void)rc_def_end1;
    (void)rc_def_end2;
    (void)rc_inf_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}