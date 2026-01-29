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
//<ID> 917
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 1024UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    // step 2: Compress data with deflate to produce compressed stream
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong compressed_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate source, copy it with inflateCopy, then inflate from the copy
    z_stream istrm_src;
    memset(&istrm_src, 0, sizeof(istrm_src));
    istrm_src.zalloc = NULL;
    istrm_src.zfree = NULL;
    istrm_src.opaque = NULL;
    inflateInit_(&istrm_src, ver, (int)sizeof(z_stream));
    istrm_src.next_in = comp;
    istrm_src.avail_in = (uInt)compressed_size;
    istrm_src.next_out = decomp;
    istrm_src.avail_out = (uInt)ORIG_LEN;
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    inflateCopy(&istrm_copy, &istrm_src);
    inflate(&istrm_copy, 0);
    inflateEnd(&istrm_copy);
    inflateEnd(&istrm_src);

    // step 4: Validate with crc32, perform gzFile operation, cleanup
    uLong checksum = crc32(0UL, orig, (uInt)ORIG_LEN);
    gzFile gf = gzopen("/dev/null", "rb");
    gzclose(gf);
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)bound;
    (void)compressed_size;
    (void)checksum;
    // API sequence test completed successfully
    return 66;
}