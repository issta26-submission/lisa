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
//<ID> 1191
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare buffers and compute adler checksum
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong ad = adler32_z(0UL, orig, (z_size_t)ORIG_LEN);

    // step 2: Initialize and copy deflate/inflate streams, prime inflate
    z_stream dsrc;
    memset(&dsrc, 0, sizeof(dsrc));
    dsrc.zalloc = NULL;
    dsrc.zfree = NULL;
    dsrc.opaque = NULL;
    deflateInit_(&dsrc, 6, ver, (int)sizeof(z_stream));
    dsrc.adler = ad;
    z_stream ddest;
    memset(&ddest, 0, sizeof(ddest));
    deflateCopy(&ddest, &dsrc);

    z_stream isrc;
    memset(&isrc, 0, sizeof(isrc));
    isrc.zalloc = NULL;
    isrc.zfree = NULL;
    isrc.opaque = NULL;
    inflateInit_(&isrc, ver, (int)sizeof(z_stream));
    isrc.adler = ad;
    z_stream idest;
    memset(&idest, 0, sizeof(idest));
    inflateCopy(&idest, &isrc);
    inflatePrime(&isrc, 1, 1);

    // step 3: Compress original, then uncompress to verify round-trip
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len_f = (uLongf)bound;
    compress2(comp, &comp_len_f, orig, ORIG_LEN, 6);
    uLong comp_len = (uLong)comp_len_f;

    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLongf decomp_len = (uLongf)ORIG_LEN;
    uncompress(decomp, &decomp_len, comp, comp_len);

    // step 4: Cleanup streams and buffers
    deflateEnd(&dsrc);
    deflateEnd(&ddest);
    inflateEnd(&isrc);
    inflateEnd(&idest);
    free(orig);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}