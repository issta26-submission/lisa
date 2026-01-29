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
//<ID> 1122
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers
    const uLong LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)LEN);
    memset(orig, 'A', (size_t)LEN);
    orig[LEN - 1] = '\0';
    const char *ver = zlibVersion();
    uLong bound = compressBound(LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size = (uLongf)bound;
    Bytef *decomp = (Bytef *)malloc((size_t)LEN);
    memset(decomp, 0, (size_t)LEN);

    // step 2: Compress original data
    compress2(comp, &comp_size, orig, LEN, 6);

    // step 3: Initialize inflate, reset with specific windowBits, perform inflate, and end
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit2_(&istrm, 15, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)LEN;
    inflateReset2(&istrm, 15);
    inflate(&istrm, Z_FINISH);
    inflateEnd(&istrm);

    // step 4: Write decompressed data to a gzip file, cleanup resources
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gf, (const char *)decomp);
    gzclose(gf);
    free(orig);
    free(comp);
    free(decomp);

    // API sequence test completed successfully
    return 66;
}