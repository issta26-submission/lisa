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
//<ID> 622
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char src[] = "zlib API sequence test payload";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLongf compBufSize = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufSize);
    memset(compBuf, 0, (size_t)compBufSize);
    const char *version = zlibVersion();

    // step 2: Initialize and configure streams
    z_stream dsrc;
    z_stream dcopy;
    z_stream istrm;
    memset(&dsrc, 0, sizeof(z_stream));
    memset(&dcopy, 0, sizeof(z_stream));
    memset(&istrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dsrc, 6, version, (int)sizeof(z_stream));
    int rc_def_copy = deflateCopy(&dcopy, &dsrc);
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 3: Operate - compress source and write compressed bytes to a gzFile, query EOF state
    uLongf destLen = compBufSize;
    int rc_comp = compress(compBuf, &destLen, source, sourceLen);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)destLen);
    int rc_gzeof = gzeof(gf);

    // step 4: Validate & Cleanup
    int rc_def_end_src = deflateEnd(&dsrc);
    int rc_def_end_copy = deflateEnd(&dcopy);
    int rc_inf_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gf);
    free(compBuf);
    (void)rc_def_init;
    (void)rc_def_copy;
    (void)rc_inf_init;
    (void)rc_comp;
    (void)rc_gzwrite;
    (void)rc_gzeof;
    (void)rc_def_end_src;
    (void)rc_def_end_copy;
    (void)rc_inf_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}