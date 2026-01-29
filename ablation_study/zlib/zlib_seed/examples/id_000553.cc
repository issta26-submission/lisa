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
//<ID> 553
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char src[] = "zlib API sequence payload: deflateInit_ -> deflateBound -> deflate -> deflateCopy -> gzwrite -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure stream, query bound and allocate output buffer
    uLong outBound = deflateBound(&dstrm, sourceLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;

    // step 3: Operate: perform deflate, copy the deflate stream, and write compressed output to gzFile
    int rc_deflate = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    z_stream copystrm;
    memset(&copystrm, 0, sizeof(z_stream));
    int rc_def_copy = deflateCopy(&copystrm, &dstrm);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, outBuf, (unsigned int)dstrm.total_out);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup: finalize deflate streams and free buffers
    int rc_def_end_orig = deflateEnd(&dstrm);
    int rc_def_end_copy = deflateEnd(&copystrm);
    free(outBuf);
    (void)version;
    (void)rc_def_init;
    (void)outBound;
    (void)rc_deflate;
    (void)rc_def_copy;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_def_end_orig;
    (void)rc_def_end_copy;
    // API sequence test completed successfully
    return 66;
}