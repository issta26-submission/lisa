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
//<ID> 1190
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong orig_adler = adler32_z(0UL, orig, (z_size_t)ORIG_LEN);

    z_stream dsrc;
    memset(&dsrc, 0, sizeof(dsrc));
    dsrc.zalloc = NULL;
    dsrc.zfree = NULL;
    dsrc.opaque = NULL;
    deflateInit_(&dsrc, 6, ver, (int)sizeof(z_stream));

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));

    // step 2: Create copies of the streams (configuration/duplication)
    z_stream dcopy;
    memset(&dcopy, 0, sizeof(dcopy));
    dcopy.zalloc = NULL;
    dcopy.zfree = NULL;
    dcopy.opaque = NULL;
    deflateCopy(&dcopy, &dsrc);

    z_stream icopy;
    memset(&icopy, 0, sizeof(icopy));
    icopy.zalloc = NULL;
    icopy.zfree = NULL;
    icopy.opaque = NULL;
    inflateCopy(&icopy, &istrm);

    // step 3: Operate: compress orig -> uncompress back, and prime inflate
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size_f = (uLongf)bound;
    compress2(comp, &comp_size_f, orig, ORIG_LEN, 6);
    uLong comp_size = (uLong)comp_size_f;

    icopy.next_in = comp;
    icopy.avail_in = (uInt)comp_size;
    inflatePrime(&icopy, 3, 5);

    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLongf decomp_len = ORIG_LEN;
    uncompress(decomp, &decomp_len, comp, comp_size);

    uLong decomp_adler = adler32_z(0UL, decomp, (z_size_t)decomp_len);

    // step 4: Validate-like cleanup: end streams and free buffers
    deflateEnd(&dcopy);
    deflateEnd(&dsrc);
    inflateEnd(&icopy);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);

    return 66;
    // API sequence test completed successfully
}