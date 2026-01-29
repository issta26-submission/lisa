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
//<ID> 551
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and initialize a deflate stream
    const char src[] = "zlib API sequence payload: prepare -> deflate -> deflateCopy -> gzwrite -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream s1;
    memset(&s1, 0, sizeof(z_stream));
    int rc1 = deflateInit_(&s1, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&s1, sourceLen);

    // step 2: Allocate output buffers and configure the primary stream
    Bytef *out1 = (Bytef *)malloc((size_t)bound);
    memset(out1, 0, (size_t)bound);
    s1.next_in = (Bytef *)source;
    s1.avail_in = (uInt)sourceLen;
    s1.next_out = out1;
    s1.avail_out = (uInt)bound;

    // step 3: Create a copy of the deflate stream and prepare its output buffer
    z_stream s2;
    memset(&s2, 0, sizeof(z_stream));
    int rc_copy = deflateCopy(&s2, &s1);
    Bytef *out2 = (Bytef *)malloc((size_t)bound);
    memset(out2, 0, (size_t)bound);
    s2.next_out = out2;
    s2.avail_out = (uInt)bound;

    // step 4: Perform deflate on both streams, finalize them, write compressed results to a gzFile and cleanup
    int rc_def1 = deflate(&s1, 4); /* Z_FINISH == 4 */
    int rc_def2 = deflate(&s2, 4); /* Z_FINISH == 4 */
    int rc_end1 = deflateEnd(&s1);
    int rc_end2 = deflateEnd(&s2);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    uLong written1 = s1.total_out;
    uLong written2 = s2.total_out;
    int rc_gz1 = gzwrite(gf, out1, (unsigned int)written1);
    int rc_gz2 = gzwrite(gf, out2, (unsigned int)written2);
    int rc_gzclose = gzclose(gf);
    free(out1);
    free(out2);
    (void)version;
    (void)rc1;
    (void)bound;
    (void)rc_copy;
    (void)rc_def1;
    (void)rc_def2;
    (void)rc_end1;
    (void)rc_end2;
    (void)rc_gz1;
    (void)rc_gz2;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}