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
//<ID> 557
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source data and primary deflate stream
    const char src[] = "zlib API sequence payload: deflateInit_ -> deflateBound -> deflateCopy -> gzwrite -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream src_strm;
    memset(&src_strm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&src_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&src_strm, sourceLen);
    Bytef *out1 = (Bytef *)malloc((size_t)bound);
    memset(out1, 0, (size_t)bound);
    src_strm.next_in = (Bytef *)source;
    src_strm.avail_in = (uInt)sourceLen;
    src_strm.next_out = out1;
    src_strm.avail_out = (uInt)bound;

    // step 2: Create a copy of the initialized deflate stream for parallel compression
    z_stream copy_strm;
    memset(&copy_strm, 0, sizeof(z_stream));
    int rc_def_copy = deflateCopy(&copy_strm, &src_strm);
    Bytef *out2 = (Bytef *)malloc((size_t)bound);
    memset(out2, 0, (size_t)bound);
    copy_strm.next_in = (Bytef *)source;
    copy_strm.avail_in = (uInt)sourceLen;
    copy_strm.next_out = out2;
    copy_strm.avail_out = (uInt)bound;

    // step 3: Operate - perform deflate on both streams and write original data to a gz file
    int rc_def1 = deflate(&src_strm, 4); /* Z_FINISH == 4 */
    int rc_def2 = deflate(&copy_strm, 4); /* Z_FINISH == 4 */
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, source, (unsigned int)sourceLen);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate/cleanup - finalize deflate streams and free resources
    int rc_def_end1 = deflateEnd(&src_strm);
    int rc_def_end2 = deflateEnd(&copy_strm);
    free(out1);
    free(out2);
    (void)version;
    (void)rc_def_init;
    (void)bound;
    (void)rc_def_copy;
    (void)rc_def1;
    (void)rc_def2;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_def_end1;
    (void)rc_def_end2;
    // API sequence test completed successfully
    return 66;
}