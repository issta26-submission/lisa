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
//<ID> 554
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data and initialize deflate stream
    const char src[] = "zlib API sequence payload: initialize -> copy -> deflate -> gzwrite -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong outBound = deflateBound(&dstrm, sourceLen);
    Bytef *outBuf1 = (Bytef *)malloc((size_t)outBound);
    Bytef *outBuf2 = (Bytef *)malloc((size_t)outBound);
    memset(outBuf1, 0, (size_t)outBound);
    memset(outBuf2, 0, (size_t)outBound);

    // step 2: Configure stream pointers and create a copy of the deflate stream
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = outBuf1;
    dstrm.avail_out = (uInt)outBound;
    z_stream dstrm_copy;
    memset(&dstrm_copy, 0, sizeof(z_stream));
    int rc_copy = deflateCopy(&dstrm_copy, &dstrm);
    dstrm_copy.next_out = outBuf2;
    dstrm_copy.avail_out = (uInt)outBound;

    // step 3: Operate: run deflate on both streams and write original data to a gz file
    int rc_def_copy = deflate(&dstrm_copy, 4);
    int rc_def_orig = deflate(&dstrm, 4);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, source, (unsigned int)sourceLen);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate-like cleanup: end deflate streams and free buffers
    int rc_def_end_copy = deflateEnd(&dstrm_copy);
    int rc_def_end_orig = deflateEnd(&dstrm);
    free(outBuf1);
    free(outBuf2);
    (void)version;
    (void)rc_def_init;
    (void)outBound;
    (void)rc_copy;
    (void)rc_def_copy;
    (void)rc_def_orig;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_def_end_copy;
    (void)rc_def_end_orig;
    // API sequence test completed successfully
    return 66;
}