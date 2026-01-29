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
//<ID> 1198
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare buffers and compute checksum
    const uLong ORIG_LEN = 1024UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong ad = adler32_z(0UL, orig, (z_size_t)ORIG_LEN);

    // step 2: Initialize deflate streams and copy state
    z_stream dsrc;
    memset(&dsrc, 0, sizeof(dsrc));
    dsrc.zalloc = NULL;
    dsrc.zfree = NULL;
    dsrc.opaque = NULL;
    deflateInit_(&dsrc, 6, ver, (int)sizeof(z_stream));
    z_stream ddst;
    memset(&ddst, 0, sizeof(ddst));
    ddst.zalloc = NULL;
    ddst.zfree = NULL;
    ddst.opaque = NULL;
    deflateCopy(&ddst, &dsrc);

    // step 3: Compress, decompress, initialize inflate streams, copy and prime
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size_f = bound;
    compress2(comp, &comp_size_f, orig, ORIG_LEN, 6);
    uLong comp_size = (uLong)comp_size_f;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLongf decomp_len = ORIG_LEN;
    uncompress(decomp, &decomp_len, comp, comp_size);

    z_stream isrc;
    memset(&isrc, 0, sizeof(isrc));
    isrc.zalloc = NULL;
    isrc.zfree = NULL;
    isrc.opaque = NULL;
    inflateInit_(&isrc, ver, (int)sizeof(z_stream));
    z_stream idst;
    memset(&idst, 0, sizeof(idst));
    idst.zalloc = NULL;
    idst.zfree = NULL;
    idst.opaque = NULL;
    inflateCopy(&idst, &isrc);
    idst.next_in = comp;
    idst.avail_in = (uInt)comp_size;
    inflatePrime(&idst, 3, (int)(ad & 0xFF));

    // step 4: Cleanup
    deflateEnd(&ddst);
    deflateEnd(&dsrc);
    inflateEnd(&idst);
    inflateEnd(&isrc);
    free(orig);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}