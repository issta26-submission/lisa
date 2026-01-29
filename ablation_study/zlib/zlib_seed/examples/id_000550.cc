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
//<ID> 550
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char src[] = "zlib API sequence payload: initialize -> copy -> deflate -> gz write -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream sstrm;
    z_stream cstrm;
    memset(&sstrm, 0, sizeof(z_stream));
    memset(&cstrm, 0, sizeof(z_stream));
    int rc_init = deflateInit_(&sstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure streams and allocate output buffers based on deflateBound
    uLong outBound = deflateBound(&sstrm, sourceLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    Bytef *outBufCopy = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    memset(outBufCopy, 0, (size_t)outBound);
    sstrm.next_in = (Bytef *)source;
    sstrm.avail_in = (uInt)sourceLen;
    sstrm.next_out = outBuf;
    sstrm.avail_out = (uInt)outBound;

    // step 3: Copy deflate state, set up copy stream buffers, perform deflate on both and write outputs to a gzFile
    int rc_copy = deflateCopy(&cstrm, &sstrm);
    cstrm.next_in = (Bytef *)source;
    cstrm.avail_in = (uInt)sourceLen;
    cstrm.next_out = outBufCopy;
    cstrm.avail_out = (uInt)outBound;
    int rc_def_copy = deflate(&cstrm, 4); /* Z_FINISH == 4 */
    int rc_def_src = deflate(&sstrm, 4); /* Z_FINISH == 4 */
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite1 = gzwrite(gf, outBufCopy, (unsigned int)(cstrm.total_out));
    int rc_gzwrite2 = gzwrite(gf, outBuf, (unsigned int)(sstrm.total_out));
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup streams and buffers, finalize
    int rc_end_copy = deflateEnd(&cstrm);
    int rc_end_src = deflateEnd(&sstrm);
    free(outBufCopy);
    free(outBuf);
    (void)version;
    (void)rc_init;
    (void)outBound;
    (void)rc_copy;
    (void)rc_def_copy;
    (void)rc_def_src;
    (void)rc_gzwrite1;
    (void)rc_gzwrite2;
    (void)rc_gzclose;
    (void)rc_end_copy;
    (void)rc_end_src;
    // API sequence test completed successfully
    return 66;
}