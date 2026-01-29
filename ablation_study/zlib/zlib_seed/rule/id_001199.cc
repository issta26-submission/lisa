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
//<ID> 1199
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare buffers and compute checksum
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    uLong ad = adler32_z(0UL, orig, (z_size_t)ORIG_LEN);

    uLongf comp_bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    uLongf comp_size_f = comp_bound;
    compress(comp, &comp_size_f, orig, ORIG_LEN);

    // step 2: Initialize streams and copies
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));

    z_stream dcopy;
    memset(&dcopy, 0, sizeof(dcopy));
    dcopy.zalloc = NULL;
    dcopy.zfree = NULL;
    dcopy.opaque = NULL;

    z_stream icopy;
    memset(&icopy, 0, sizeof(icopy));
    icopy.zalloc = NULL;
    icopy.zfree = NULL;
    icopy.opaque = NULL;

    // step 3: Configure streams, copy states and prime inflater
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size_f;
    deflateCopy(&dcopy, &dstrm);
    inflateCopy(&icopy, &istrm);
    inflatePrime(&istrm, 3, 5);

    // step 4: Decompress using uncompress and cleanup
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLongf decomp_len = ORIG_LEN;
    uncompress(decomp, &decomp_len, comp, (uLong)comp_size_f);

    deflateEnd(&dstrm);
    deflateEnd(&dcopy);
    inflateEnd(&istrm);
    inflateEnd(&icopy);
    free(orig);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}