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
//<ID> 1194
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare buffers and compute adler checksum
    const uLong ORIGINAL_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIGINAL_LEN);
    memset(orig, 'Z', (size_t)ORIGINAL_LEN);
    uLong checksum_orig = adler32_z(0UL, orig, (z_size_t)ORIGINAL_LEN);
    const char *ver = zlibVersion();

    // step 2: Initialize deflate and inflate streams and make copies
    z_stream dsrc;
    memset(&dsrc, 0, sizeof(dsrc));
    dsrc.zalloc = NULL;
    dsrc.zfree = NULL;
    dsrc.opaque = NULL;
    deflateInit_(&dsrc, 6, ver, (int)sizeof(z_stream));
    z_stream ddest;
    memset(&ddest, 0, sizeof(ddest));
    ddest.zalloc = NULL;
    ddest.zfree = NULL;
    ddest.opaque = NULL;
    deflateCopy(&ddest, &dsrc);

    z_stream isrc;
    memset(&isrc, 0, sizeof(isrc));
    isrc.zalloc = NULL;
    isrc.zfree = NULL;
    isrc.opaque = NULL;
    inflateInit_(&isrc, ver, (int)sizeof(z_stream));
    z_stream idest;
    memset(&idest, 0, sizeof(idest));
    idest.zalloc = NULL;
    idest.zfree = NULL;
    idest.opaque = NULL;
    inflateCopy(&idest, &isrc);

    // step 3: Compress original buffer and then uncompress it; prime the inflate copy
    uLong bound = compressBound(ORIGINAL_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len_f = (uLongf)bound;
    compress2(comp, &comp_len_f, orig, ORIGINAL_LEN, 6);
    uLong comp_len = (uLong)comp_len_f;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIGINAL_LEN);
    memset(decomp, 0, (size_t)ORIGINAL_LEN);
    uLongf decomp_len_f = (uLongf)ORIGINAL_LEN;
    uncompress(decomp, &decomp_len_f, comp, comp_len);
    isrc.next_in = comp;
    isrc.avail_in = (uInt)comp_len;
    inflatePrime(&idest, 3, 5);

    // step 4: Validate checksum and cleanup streams and buffers
    uLong checksum_decomp = adler32_z(0UL, decomp, (z_size_t)decomp_len_f);
    (void)checksum_orig;
    (void)checksum_decomp;
    deflateEnd(&ddest);
    deflateEnd(&dsrc);
    inflateEnd(&idest);
    inflateEnd(&isrc);
    free(orig);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}