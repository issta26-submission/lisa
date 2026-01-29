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
//<ID> 555
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data and initialize a deflate stream
    const char src[] = "zlib API sequence payload: init -> copy -> deflate -> gzwrite -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Allocate output buffers, compute bounds and prepare a copy of the deflate stream
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)bound);
    Bytef *copyBuf = (Bytef *)malloc((size_t)bound);
    memset(outBuf, 0, (size_t)bound);
    memset(copyBuf, 0, (size_t)bound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)bound;
    z_stream dcopy;
    memset(&dcopy, 0, sizeof(z_stream));
    int rc_def_copy = deflateCopy(&dcopy, &dstrm);
    dcopy.next_out = copyBuf;
    dcopy.avail_out = (uInt)bound;

    // step 3: Operate - perform deflate on both streams and write the original deflate output to a gzFile
    int rc_deflate_orig = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    int rc_deflate_copy = deflate(&dcopy, 4); /* compress same input in the copied stream */
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, outBuf, (unsigned int)dstrm.total_out);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup - finalize streams and free resources
    int rc_def_end_orig = deflateEnd(&dstrm);
    int rc_def_end_copy = deflateEnd(&dcopy);
    free(outBuf);
    free(copyBuf);
    (void)version;
    (void)rc_def_init;
    (void)bound;
    (void)rc_def_copy;
    (void)rc_deflate_orig;
    (void)rc_deflate_copy;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_def_end_orig;
    (void)rc_def_end_copy;
    // API sequence test completed successfully
    return 66;
}