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
//<ID> 510
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and produce a compressed buffer using compress2
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> deflateInit_ -> uncompress -> gzwrite -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize a deflate stream (configuration) using deflateInit_ and retain version info
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 3: Uncompress the compressed buffer to validate, obtain any error string with zError, and write result to a gzFile
    uLongf destBound = (uLongf)(sourceLen + 256);
    Bytef *dest = (Bytef *)malloc((size_t)destBound);
    memset(dest, 0, (size_t)destBound);
    uLongf destLen = destBound;
    int rc_un = uncompress(dest, &destLen, comp, (uLong)compLen);
    const char *errstr = zError(rc_un);
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)dest, (unsigned int)destLen);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup: end deflate stream and free allocated memory
    int rc_def_end = deflateEnd(&dstrm);
    free(comp);
    free(dest);

    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_un;
    (void)errstr;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_def_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}