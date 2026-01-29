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
//<ID> 558
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and initialize original deflate stream
    const char src[] = "zlib API sequence payload: prepare -> deflateInit_ -> deflateCopy -> deflate -> gzwrite -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dorig;
    memset(&dorig, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dorig, 6, version, (int)sizeof(z_stream));

    // step 2: Compute output bounds, allocate buffers and create a copy of the deflate stream
    uLong outBound = deflateBound(&dorig, sourceLen);
    Bytef *out1 = (Bytef *)malloc((size_t)outBound);
    memset(out1, 0, (size_t)outBound);
    Bytef *out2 = (Bytef *)malloc((size_t)outBound);
    memset(out2, 0, (size_t)outBound);
    z_stream dcopy;
    memset(&dcopy, 0, sizeof(z_stream));
    int rc_def_copy = deflateCopy(&dcopy, &dorig);

    // step 3: Configure both streams with the same input and perform deflate on each into separate buffers
    dorig.next_in = (Bytef *)source;
    dorig.avail_in = (uInt)sourceLen;
    dorig.next_out = out1;
    dorig.avail_out = (uInt)outBound;
    int rc_deflate1 = deflate(&dorig, 4); /* Z_FINISH == 4 */

    dcopy.next_in = (Bytef *)source;
    dcopy.avail_in = (uInt)sourceLen;
    dcopy.next_out = out2;
    dcopy.avail_out = (uInt)outBound;
    int rc_deflate2 = deflate(&dcopy, 4); /* Z_FINISH == 4 */

    // step 4: Write the first compressed result to a gzFile, finalize streams and free resources
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, out1, (unsigned int)dorig.total_out);
    int rc_gzclose = gzclose(gf);
    int rc_end1 = deflateEnd(&dorig);
    int rc_end2 = deflateEnd(&dcopy);
    free(out1);
    free(out2);
    (void)version;
    (void)rc_def_init;
    (void)outBound;
    (void)rc_def_copy;
    (void)rc_deflate1;
    (void)rc_deflate2;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_end1;
    (void)rc_end2;
    // API sequence test completed successfully
    return 66;
}