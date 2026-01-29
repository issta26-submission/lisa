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
//<ID> 556
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source data, query zlib version, initialize deflate stream and allocate output buffers
    const char src[] = "zlib API sequence payload: deflateInit_ -> deflateBound -> deflateCopy -> gzwrite -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *out1 = (Bytef *)malloc((size_t)bound);
    memset(out1, 0, (size_t)bound);
    Bytef *out2 = (Bytef *)malloc((size_t)bound);
    memset(out2, 0, (size_t)bound);

    // step 2: Configure deflate stream input/output pointers, perform an initial deflate call, then copy the stream
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = out1;
    dstrm.avail_out = (uInt)bound;
    int rc_deflate_part = deflate(&dstrm, 0); /* Z_NO_FLUSH == 0 */
    z_stream dcopy;
    memset(&dcopy, 0, sizeof(z_stream));
    int rc_def_copy = deflateCopy(&dcopy, &dstrm);
    dcopy.next_out = out2;
    dcopy.avail_out = (uInt)bound;

    // step 3: Finish compression on both original and copied streams, write original and compressed outputs to a gzFile
    int rc_deflate_finish_orig = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    int rc_deflate_finish_copy = deflate(&dcopy, 4); /* Z_FINISH == 4 */
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite_src = gzwrite(gf, source, (unsigned int)sourceLen);
    unsigned int len_out1 = (unsigned int)(dstrm.total_out);
    unsigned int len_out2 = (unsigned int)(dcopy.total_out);
    int rc_gzwrite_out1 = 0;
    int rc_gzwrite_out2 = 0;
    rc_gzwrite_out1 = gzwrite(gf, out1, len_out1);
    rc_gzwrite_out2 = gzwrite(gf, out2, len_out2);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup deflate streams and free buffers
    int rc_def_end_orig = deflateEnd(&dstrm);
    int rc_def_end_copy = deflateEnd(&dcopy);
    free(out1);
    free(out2);
    (void)version;
    (void)rc_def_init;
    (void)rc_deflate_part;
    (void)rc_def_copy;
    (void)rc_deflate_finish_orig;
    (void)rc_deflate_finish_copy;
    (void)rc_gzwrite_src;
    (void)rc_gzwrite_out1;
    (void)rc_gzwrite_out2;
    (void)rc_gzclose;
    (void)rc_def_end_orig;
    (void)rc_def_end_copy;
    // API sequence test completed successfully
    return 66;
}