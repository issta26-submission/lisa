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
//<ID> 625
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialization
    const char src[] = "zlib API sequence test payload for combined APIs";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLongf compBufLen = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufLen);
    memset(compBuf, 0, (size_t)compBufLen);
    z_stream dsrc;
    z_stream ddst;
    z_stream istrm;
    memset(&dsrc, 0, sizeof(z_stream));
    memset(&ddst, 0, sizeof(z_stream));
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dsrc, 6, version, (int)sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    // step 2: Configure - copy deflate state to a destination stream
    int rc_def_copy = deflateCopy(&ddst, &dsrc);

    // step 3: Operate - compress with convenience API and write compressed bytes to a gzFile
    uLongf actualCompLen = compBufLen;
    int rc_compress = compress(compBuf, &actualCompLen, source, sourceLen);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)actualCompLen);
    int rc_gzeof = gzeof(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate & Cleanup
    int rc_def_end_src = deflateEnd(&dsrc);
    int rc_def_end_dst = deflateEnd(&ddst);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    (void)rc_def_init;
    (void)rc_inf_init;
    (void)rc_def_copy;
    (void)rc_compress;
    (void)rc_gzwrite;
    (void)rc_gzeof;
    (void)rc_gzclose;
    (void)rc_def_end_src;
    (void)rc_def_end_dst;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}